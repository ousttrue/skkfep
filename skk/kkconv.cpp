#include "kkconv.h"
#include "connsh.h"
#include "dictinary.h"
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
kkBeg(char)
{
  g_skk.setKeymap(SkkModes::KINPUT_MODE);
  g_skk.showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  WordBufLen = 0;
}

void
kkBegV(char c)
{
  kkBeg({});
  romkan::inputKanaVowel(tolower(c), bufferedInput);
}

void
kkBegC(char c)
{
  kkBeg({});
  romkan::inputKanaConso(tolower(c), bufferedInput, NULL);
}

void
kkBegA(char c)
{
  romkan::flushKana();
  g_skk.setKeymap(KeymapTypes::KAlphaInput);
  g_skk.showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  WordBufLen = 0;
}

void
kalpha(char c)
{
  terminal::write1(c);
  WordBuf[WordBufLen++] = c;
}

void
kaBS(char c)
{
  if (WordBufLen > 0) {
    kanjiInputEffect(0);
    rubout(1);
    WordBufLen--;
    kanjiInputEffect(1);
  }
}

void
kKanaV(char c)
{
  romkan::inputKanaVowel(c, bufferedInput);
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
okKanaV(char c /*, char first*/)
{
  char first = false;
  if (first) {
    WordBuf[WordBufLen++] = c;
  }
  rubout(1);
  romkan::inputKanaVowel(tolower(c), putOkuri);
  kkconv({});
}

void
kKanaC(char c)
{
  romkan::inputKanaConso(c, bufferedInput, NULL);
}

void
okKanaC(char c /*, char first*/)
{
  char first = false;
  if (Nconso == 0) {
    WordBuf[WordBufLen++] = c;
  } else if (first) {
    romkan::flushLastConso('\0', bufferedInput, NULL);
    WordBuf[WordBufLen++] = c;
  }
  romkan::inputKanaConso(tolower(c), putOkuri, NULL);
}

void
kZenAl(char c)
{
  bufferedInput(zenkakualpha::inputZenkakuAlpha(c));
}

void
kZenEx(char c)
{
  bufferedInput(zenkakualpha::inputZenkakuEx(c));
}

void
kfthru(char c)
{
  static char buf[2];
  buf[0] = c;
  buf[1] = '\0';
  bufferedInput(buf);
}

void
fxthru(char c)
{
  /* fix and through */
  fixIt({});
  g_skk.input(c);
}

void
kfCancel(char c)
{
  kanjiInputEffect(0);
  romkan::cancelConso();
  rubout(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  endKanjiInput();
}

void
kfFix(char c)
{
  csrLeft(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  kanjiInputEffect(0);
  child::writeShells(WordBuf);
  child::flushOut(WordBufLen);
  endKanjiInput();
}

void
kfFixToAsc(char c)
{
  kfFix(c);
  g_skk.toAsc();
}

void
kfFixToZenA(char c)
{
  kfFix(c);
  g_skk.toZenA();
}

void
kfFixThru(char c)
{
  kfFix(c);
  child::thru(c);
}

void
thruKfFixToAsc(char c)
{
  kfFix(c);
  g_skk.toAsc();
  child::thru(c);
}

void
thruKfFixToEsc(char c)
{
  kfFix(c);
  g_skk.toEsc();
  child::thru(c);
}

void
okfFix(char c)
{
  cancelOkuri({});
  if (g_skk.is_okuri_input())
    cancelOkuri({});
  kfFix(0);
}

void
okfFixToAsc(char c)
{
  okfFix(c);
  g_skk.toAsc();
}

void
okfFixToZenA(char c)
{
  okfFix(c);
  g_skk.toZenA();
}

void
okfFixThru(char c)
{
  okfFix(c);
  child::thru(c);
}

void
thruOkfFixToAsc(char c)
{
  okfFix(c);
  g_skk.toAsc();
  child::thru(c);
}

void
thruOkfFixToEsc(char c)
{
  okfFix(c);
  g_skk.toEsc();
  child::thru(c);
}

void
kfBS(char c)
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
okuriBS(char c)
{
  int i, n;
  char con[MAX_CONSO];

  kanjiInputEffect(0);
  n = Nconso;
  for (i = 0; i < n; i++)
    con[i] = LastConso[i];
  if (n <= 1) {
    cancelOkuri({});
  } else {
    romkan::cancelConso();
    for (i = 0; i < n; i++)
      okKanaC(con[i] /*, 0*/);
  }
  kanjiInputEffect(1);
}

void
kkconv(char c)
{
  int l;

  kanjiInputEffect(0);
  romkan::flushLastConso('\0', bufferedInput, NULL);
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput();
  }
  g_skk.showmode(KSELECT_MODE);

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
  g_skk.setKeymap(KeymapTypes::Selection);

  if (Current.IsEnabled()) {
    showCand();
    BlockTty = 1;
  } else {
    /* Really, enter register mode */
    if (PreserveOnFailure) {
      bell();
      backToKanjiInput();
    } else {
      endKanjiInput();
    }
  }
}

void
toOkuri()
{
  OkuriInput = 1;
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  g_skk.setKeymap(KeymapTypes::OkuriInput);
}

void
kOkuri(char c)
{
  char okuri = tolower(c);

  if (WordBufLen == 0) {
    /* Recover chattering effect */
    if (VOWEL(okuri)) {
      kKanaV(okuri);
      return;
    }
    romkan::cancelConso();
    endKanjiInput();
    kanjiInputEffect(0);
    return;
  }
  toOkuri();
  terminal::write1('*');
  g_skk.input(okuri, 1);
}

void
stSuffix(char c)
{
  fixIt({});
  kkBeg({});
  kfthru(c);
}

void
stPrefixCv(char c)
{
  if (WordBufLen == 0) {
    kfthru(c);
  } else {
    kfthru(c);
    kkconv(' ');
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
nxCand(char)
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
pvCand(char)
{
  auto l = clearCurrent();

  if (Current.Decrement()) {
    csrLeft(l);
    kanjiSelectionEffect(1);
    terminal::writes(Current.Cand->c_str());

    if (OkuriInput)
      terminal::writes(OkuriBuf);

  } else {
    backToKanjiInput();
  }
}

/* back to kanji input mode */
void
backToKanjiInput()
{
  g_skk.restoreKeymap();
  g_skk.showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  if (OkuriInput) {
    clearOkuri();
    terminal::writes(WordBuf);
    terminal::write1('*');
    OkuriFirst = 1;
  } else
    terminal::writes(WordBuf);
}

void
cancelOkuri(char)
{
  kanjiInputEffect(0);
  if (Nconso == 0) {
    rubout(1);
    OkuriInput = 0;
    g_skk.setKeymap(KeymapTypes::KanjiInput);
  } else {
    romkan::cancelConso();
    clearOkuri();
    OkuriFirst = 1;
  }
  kanjiInputEffect(1);
}

void
clearOkuri()
{
  WordBufLen--;
  WordBuf[WordBufLen] = '\0';
  toOkuri();
}

void
fixIt(char c)
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
    child::flushOut(l);
  }
  endKanjiInput();
}

void
thruFixItToAsc(char c)
{
  fixIt(c);
  g_skk.toAsc();
  child::thru(c);
}

void
thruFixItToEsc(char c)
{
  fixIt(c);
  g_skk.toEsc();
  child::thru(c);
}

void
cancelSel(char c)
{
  kanjiSelectionEffect(0);
  if (Current.IsEnabled()) {
    auto l = Current.Cand->size();
    if (OkuriInput)
      l += strlen(OkuriBuf);
    rubout(l);
  }
  backToKanjiInput();
}

void
endKanjiInput()
{
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  OkuriInput = 0;
  g_skk.toKana();
  BlockTty = 0;
}

void
h2kkana(char c)
{
  int l;

  kanjiInputEffect(0);
  romkan::flushLastConso('\0', bufferedInput, NULL);
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput();
  }
  WordBuf[WordBufLen] = '\0';

  l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  romkan::hira2kata(WordBuf);
  child::writeShells(WordBuf);
  endKanjiInput();
}
