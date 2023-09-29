#include "kkconv.h"
#include "app.h"
#include "connsh.h"
#include "dictinary.h"
#include "etc.h"
#include "fep.h"
#include "keybind.h"
#include "romkan.h"
#include "stty.h"
#include "terms.h"
#include <ctype.h>
#include <optional>
#include <string.h>

#define WORD_BUF_LEN 128
#define OKURI_LEN 8

#define PRESERVE_ON_FAILURE 1 /* preserve candidate on conversion failure */
char PreserveOnFailure = PRESERVE_ON_FAILURE;
short BlockTty;
char OkuriFirst;

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
bufferedInput(const char* s)
{
  writes(s);
  for (; *s != '\0'; s++)
    WordBuf[WordBufLen++] = *s;
}

void
kkBeg(char)
{
  setKeymap(&CurrentKeymap, &KanjiInputKeymap);
  showmode(KINPUT_MODE);
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
  setKeymap(&CurrentKeymap, &KAlphaInputKeymap);
  showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  WordBufLen = 0;
}

void
kalpha(char c)
{
  write1(c);
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
putOkuri(const char* s)
{
  int l = OkuriBufLen;
  writes(s);
  for (auto p = s; *p; p++) {
    OkuriBuf[OkuriBufLen++] = *p;
    if (OkuriBufLen >= OKURI_LEN) {
      bell();
      OkuriBuf[l] = '\0';
      OkuriBufLen = l;
      rubout(strlen(s));
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
  romkan::inputZenkakuAlpha(c, bufferedInput);
}

void
kZenEx(char c)
{
  romkan::inputZenkakuEx(c, bufferedInput);
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
  (*CurrentKeymap)[c](c);
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
  writeShells(WordBuf);
  flushOut(WordBufLen);
  endKanjiInput();
}

void
kfFixToAsc(char c)
{
  kfFix(c);
  toAsc(c);
}

void
kfFixToZenA(char c)
{
  kfFix(c);
  toZenA(c);
}

void
kfFixThru(char c)
{
  kfFix(c);
  thru(c);
}

void
thruKfFixToAsc(char c)
{
  kfFix(c);
  toAsc(c);
  thru(c);
}

void
thruKfFixToEsc(char c)
{
  kfFix(c);
  toEsc(c);
  thru(c);
}

void
okfFix(char c)
{
  cancelOkuri({});
  if (CurrentKeymap == &OkuriInputKeymap)
    cancelOkuri({});
  kfFix(0);
}

void
okfFixToAsc(char c)
{
  okfFix(c);
  toAsc(c);
}

void
okfFixToZenA(char c)
{
  okfFix(c);
  toZenA(c);
}

void
okfFixThru(char c)
{
  okfFix(c);
  thru(c);
}

void
thruOkfFixToAsc(char c)
{
  okfFix(c);
  toAsc(c);
  thru(c);
}

void
thruOkfFixToEsc(char c)
{
  okfFix(c);
  toEsc(c);
  thru(c);
}

void
kfBS(char c)
{
  int i, n;
  char con[MAX_CONSO];

  kanjiInputEffect(0);
  if (Nconso) {
    rubout(Nconso);
    for (i = 1; i < Nconso; i++)
      con[i] = LastConso[i];
    n = Nconso;
    Nconso = 0;
    Kindex = 0;
    SmallTU = 0;
    for (i = 1; i < n; i++)
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
  showmode(KSELECT_MODE);

  WordBuf[WordBufLen] = '\0';
  Current.List = App::Instance().UserDic->getCand(WordBuf);
  if (Current.List) {
    Current.Cand = Current.List->begin();
  } else {
    Current.Cand = {};
  }

  l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  setKeymap(&CurrentKeymap, convertKeymap(SelectionKeymap));

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
  setKeymap(&CurrentKeymap, &OkuriInputKeymap);
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
  write1('*');
  (*CurrentKeymap)[okuri](okuri /*, 1*/);
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
  writes(Current.Cand->c_str());
  if (OkuriInput) {
    writes(OkuriBuf);
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
  writes(Current.Cand->c_str());

  if (OkuriInput)
    writes(OkuriBuf);
}

void
pvCand(char)
{
  auto l = clearCurrent();

  if (Current.Decrement()) {
    csrLeft(l);
    kanjiSelectionEffect(1);
    writes(Current.Cand->c_str());

    if (OkuriInput)
      writes(OkuriBuf);

  } else {
    backToKanjiInput();
  }
}

/* back to kanji input mode */
void
backToKanjiInput()
{
  restoreKeymap(&CurrentKeymap);
  showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  if (OkuriInput) {
    clearOkuri();
    writes(WordBuf);
    write1('*');
    OkuriFirst = 1;
  } else
    writes(WordBuf);
}

void
cancelOkuri(char)
{
  kanjiInputEffect(0);
  if (Nconso == 0) {
    rubout(1);
    OkuriInput = 0;
    setKeymap(&CurrentKeymap, &KanjiInputKeymap);
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
    writeShells(Current.Cand->c_str());
    if (OkuriInput) {
      writeShells(OkuriBuf);
    }
    flushOut(l);
  }
  endKanjiInput();
}

void
thruFixItToAsc(char c)
{
  fixIt(c);
  toAsc(c);
  thru(c);
}

void
thruFixItToEsc(char c)
{
  fixIt(c);
  toEsc(c);
  thru(c);
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
  romkan::toKana({});
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
  writeShells(WordBuf);
  endKanjiInput();
}
