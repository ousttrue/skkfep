#include "kkconv.h"
#include "connsh.h"
#include "dictinary.h"
#include "flushout.h"
#include "romkan.h"
#include "skk.h"
#include "stty.h"
#include "terms.h"
#include "zenkakualpha.h"
#include <ctype.h>
#include <optional>
#include <string.h>

#define WORD_BUF_LEN 128
#define OKURI_LEN 8

#define PRESERVE_ON_FAILURE 1 /* preserve candidate on conversion failure */
char PreserveOnFailure = PRESERVE_ON_FAILURE;
short BlockTty;
char OkuriFirst;
struct Dictionary* UserDic = nullptr;

#define VOWEL(c)                                                               \
  ((c) == 'a' || (c) == 'i' || (c) == 'u' || (c) == 'e' || (c) == 'o')

static char WordBuf[WORD_BUF_LEN];
static int WordBufLen;

struct Context
{
  const CandList* List = nullptr;
  std::list<std::string>::const_iterator Cand;

  Context() {}

  Context(const CandList* list)
    : List(list)
  {
    if (List) {
      Cand = List->begin();
    }
  }

  bool IsEnabled()
  {
    if (List && Cand != List->end()) {
      return true;
    }
    return false;
  }

  bool Increment()
  {
    if (IsEnabled()) {
      ++Cand;
      return true;
    }
    return false;
  }

  bool Decrement()
  {
    if (IsEnabled() && Cand != List->begin()) {
      --Cand;
      return true;
    }
    return false;
  }
};

static Context Current;
static short OkuriInput;
static char OkuriBuf[OKURI_LEN];
static int OkuriBufLen;

static void
bufferedInput(std::string_view s)
{
  terminal::writes(s);
  for (auto c : s)
    WordBuf[WordBufLen++] = c;
}

void
kkClearBuf()
{
  WordBufLen = 0;
}

void
kkBegV(Skk* skk, char c)
{
  skk->kkBeg();
  bufferedInput(romkan::inputKanaVowel(tolower(c)));
}

void
kkBegC(Skk* skk, char c)
{
  skk->kkBeg();
  bufferedInput(romkan::inputKanaConso(tolower(c), NULL));
}

void
kkBegA(Skk* skk, char c)
{
  romkan::flushKana();
  skk->setKeymap(KeymapTypes::KAlphaInput);
  skk->showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  WordBufLen = 0;
}

void
kalpha(char c, bool)
{
  terminal::write1(c);
  WordBuf[WordBufLen++] = c;
}

void
kaBS(char c, bool)
{
  if (WordBufLen > 0) {
    kanjiInputEffect(0);
    rubout(1);
    WordBufLen--;
    kanjiInputEffect(1);
  }
}

void
kKanaV(char c, bool)
{
  bufferedInput(romkan::inputKanaVowel(c));
}

static void
putOkuri(std::string_view s)
{
  int l = OkuriBufLen;
  terminal::writes(s);
  for (auto c : s) {
    OkuriBuf[OkuriBufLen++] = c;
    if (OkuriBufLen >= OKURI_LEN) {
      bell();
      OkuriBuf[l] = '\0';
      OkuriBufLen = l;
      rubout(s.size());
      return;
    }
  }
  OkuriBuf[OkuriBufLen] = '\0';
}

void
okKanaV(Skk* skk, char c, bool first)
{
  if (first) {
    WordBuf[WordBufLen++] = c;
  }
  rubout(1);
  putOkuri(romkan::inputKanaVowel(tolower(c)));
  kkconv(skk, c);
}

void
kKanaC(char c, bool)
{
  bufferedInput(romkan::inputKanaConso(c, NULL));
}

void
okKanaC(char c, bool first)
{
  if (Nconso == 0) {
    WordBuf[WordBufLen++] = c;
  } else if (first) {
    romkan::flushLastConso('\0', bufferedInput, NULL);
    WordBuf[WordBufLen++] = c;
  }
  putOkuri(romkan::inputKanaConso(tolower(c), NULL));
}

void
kZenAl(char c, bool)
{
  bufferedInput(zenkakualpha::inputZenkakuAlpha(c));
}

void
kZenEx(char c, bool)
{
  bufferedInput(zenkakualpha::inputZenkakuEx(c));
}

void
kfthru(char c, bool)
{
  static char buf[2];
  buf[0] = c;
  buf[1] = '\0';
  bufferedInput(buf);
}

void
fxthru(Skk* skk, char c)
{
  /* fix and through */
  fixIt(skk);
  skk->input(c);
}

void
kfCancel(Skk* skk, char c)
{
  kanjiInputEffect(0);
  romkan::cancelConso();
  rubout(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  endKanjiInput(skk);
}

void
kfFix(Skk* skk, char c)
{
  csrLeft(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  kanjiInputEffect(0);
  child::writeShells(WordBuf);
  flushOut(WordBufLen);
  endKanjiInput(skk);
}

void
kfFixToAsc(Skk* skk, char c)
{
  kfFix(skk, c);
  skk->toAsc();
}

void
kfFixToZenA(Skk* skk, char c)
{
  kfFix(skk, c);
  skk->toZenA();
}

void
kfFixThru(Skk* skk, char c)
{
  kfFix(skk, c);
  child::thru(c);
}

void
thruKfFixToAsc(Skk* skk, char c)
{
  kfFix(skk, c);
  skk->toAsc();
  child::thru(c);
}

void
okfFix(Skk* skk, char c)
{
  cancelOkuri(skk);
  if (skk->is_okuri_input())
    cancelOkuri(skk);
  kfFix(skk, 0);
}

void
okfFixToAsc(Skk* skk, char c)
{
  okfFix(skk, c);
  skk->toAsc();
}

void
okfFixToZenA(Skk* skk, char c)
{
  okfFix(skk, c);
  skk->toZenA();
}

void
okfFixThru(Skk* skk, char c)
{
  okfFix(skk, c);
  skk->thru(c);
}

void
thruOkfFixToAsc(Skk* skk, char c)
{
  okfFix(skk, c);
  skk->toAsc();
  skk->thru(c);
}

void
kfBS(char c, bool)
{
  kanjiInputEffect(0);
  if (Nconso) {
    rubout(Nconso);
    char con[MAX_CONSO];
    for (int i = 1; i < Nconso; i++)
      con[i] = LastConso[i];
    int n = Nconso;
    Nconso = 0;
    Kindex = {};
    SmallTU = 0;
    for (int i = 1; i < n; i++)
      kKanaC(con[i]);
  } else if (WordBufLen > 0) {
    if (WordBuf[WordBufLen - 1] & 0x80) { /* is KANJI */
      rubout(2);
      WordBufLen -= 2;
    } else {
      rubout(1);
      WordBufLen--;
    }
  }
  kanjiInputEffect(1);
}

void
okuriBS(Skk* skk, char c)
{
  int i, n;
  char con[MAX_CONSO];

  kanjiInputEffect(0);
  n = Nconso;
  for (i = 0; i < n; i++)
    con[i] = LastConso[i];
  if (n <= 1) {
    cancelOkuri(skk);
  } else {
    romkan::cancelConso();
    for (i = 0; i < n; i++)
      okKanaC(con[i], 0);
  }
  kanjiInputEffect(1);
}

void
kkconv(Skk* skk, char c)
{
  int l;

  kanjiInputEffect(0);
  romkan::flushLastConso('\0', bufferedInput, NULL);
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput(skk);
  }
  skk->showmode(KSELECT_MODE);

  WordBuf[WordBufLen] = '\0';
  Current.List = UserDic->getCand(WordBuf);
  if (Current.List) {
    Current.Cand = Current.List->begin();
  } else {
    Current.Cand = {};
  }

  l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  skk->setKeymap(KeymapTypes::Selection);

  if (Current.IsEnabled()) {
    showCand();
    BlockTty = 1;
  } else {
    /* Really, enter register mode */
    if (PreserveOnFailure) {
      bell();
      backToKanjiInput(skk);
    } else {
      endKanjiInput(skk);
    }
  }
}

void
toOkuri(Skk* skk)
{
  OkuriInput = 1;
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  skk->setKeymap(KeymapTypes::OkuriInput);
}

void
kOkuri(Skk* skk, char c)
{
  char okuri = tolower(c);

  if (WordBufLen == 0) {
    /* Recover chattering effect */
    if (VOWEL(okuri)) {
      kKanaV(okuri);
      return;
    }
    romkan::cancelConso();
    endKanjiInput(skk);
    kanjiInputEffect(0);
    return;
  }
  toOkuri(skk);
  terminal::write1('*');
  skk->input(okuri, 1);
}

void
stSuffix(Skk* skk, char c)
{
  fixIt(skk);
  skk->kkBeg();
  kfthru(c);
}

void
stPrefixCv(Skk* skk, char c)
{
  if (WordBufLen == 0) {
    kfthru(c);
  } else {
    kfthru(c);
    kkconv(skk, ' ');
  }
}

void
showCand()
{
  kanjiSelectionEffect(1);
  terminal::writes(Current.Cand->c_str());
  if (OkuriInput) {
    terminal::writes(OkuriBuf);
  }
}

int
clearCurrent()
{
  auto l = Current.Cand->size();
  if (OkuriInput)
    l += strlen(OkuriBuf);

  csrLeft(l);
  kanjiSelectionEffect(0);
  erase(l);
  return l;
}

void
nxCand()
{
  auto l = clearCurrent();

  Current.Increment();

  // draw
  csrLeft(l);
  kanjiSelectionEffect(1);
  terminal::writes(Current.Cand->c_str());

  if (OkuriInput)
    terminal::writes(OkuriBuf);
}

void
pvCand(Skk* skk)
{
  auto l = clearCurrent();

  if (Current.Decrement()) {
    csrLeft(l);
    kanjiSelectionEffect(1);
    terminal::writes(Current.Cand->c_str());

    if (OkuriInput)
      terminal::writes(OkuriBuf);

  } else {
    backToKanjiInput(skk);
  }
}

/* back to kanji input mode */
void
backToKanjiInput(Skk* skk)
{
  skk->restoreKeymap();
  skk->showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  if (OkuriInput) {
    clearOkuri(skk);
    terminal::writes(WordBuf);
    terminal::write1('*');
    OkuriFirst = 1;
  } else
    terminal::writes(WordBuf);
}

void
cancelOkuri(Skk* skk)
{
  kanjiInputEffect(0);
  if (Nconso == 0) {
    rubout(1);
    OkuriInput = 0;
    skk->setKeymap(KeymapTypes::KanjiInput);
  } else {
    romkan::cancelConso();
    clearOkuri(skk);
    OkuriFirst = 1;
  }
  kanjiInputEffect(1);
}

void
clearOkuri(Skk* skk)
{
  WordBufLen--;
  WordBuf[WordBufLen] = '\0';
  toOkuri(skk);
}

void
fixIt(Skk* skk)
{
  kanjiSelectionEffect(0);
  if (Current.IsEnabled()) {
    auto l = Current.Cand->size();
    if (OkuriInput)
      l += strlen(OkuriBuf);
    csrLeft(l);
    child::writeShells(Current.Cand->c_str());
    if (OkuriInput) {
      child::writeShells(OkuriBuf);
    }
    flushOut(l);
  }
  endKanjiInput(skk);
}

void
thruFixItToAsc(Skk* skk, char c)
{
  fixIt(skk);
  skk->toAsc();
  skk->thru(c);
}

void
cancelSel(Skk* skk, char c)
{
  kanjiSelectionEffect(0);
  if (Current.IsEnabled()) {
    auto l = Current.Cand->size();
    if (OkuriInput)
      l += strlen(OkuriBuf);
    rubout(l);
  }
  backToKanjiInput(skk);
}

void
endKanjiInput(Skk* skk)
{
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  OkuriInput = 0;
  skk->toKana();
  BlockTty = 0;
}

void
h2kkana(Skk* skk, char c)
{
  int l;

  kanjiInputEffect(0);
  romkan::flushLastConso('\0', bufferedInput, NULL);
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput(skk);
  }
  WordBuf[WordBufLen] = '\0';

  l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  romkan::hira2kata(WordBuf);
  child::writeShells(WordBuf);
  endKanjiInput(skk);
}
