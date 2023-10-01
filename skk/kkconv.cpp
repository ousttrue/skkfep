#include "kkconv.h"
#include "dictinary.h"
#include "romkan.h"
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

static SkkResult
bufferedInput(std::string_view s)
{
  for (auto c : s) {
    WordBuf[WordBufLen++] = c;
  }
  return { .Output = { .Unconfirmed = { s.begin(), s.end() } } };
}

static SkkResult
showCand()
{
  kanjiSelectionEffect(1);
  SkkResult output;
  output.Output.Unconfirmed += *Current.Cand;
  if (OkuriInput) {
    output.Output.Unconfirmed += OkuriBuf;
  }
  return output;
}

static SkkResult
toOkuri()
{
  OkuriInput = 1;
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  return {
    .NextKeymap = KeymapTypes::OkuriInput,
  };
}

static SkkResult
clearOkuri()
{
  WordBufLen--;
  WordBuf[WordBufLen] = '\0';
  return toOkuri();
}

/* back to kanji input mode */
static SkkResult
backToKanjiInput()
{
  SkkResult output;
  output.RestoreKeymap = true;
  output.NextMode = KINPUT_MODE;
  kanjiInputEffect(1);
  if (OkuriInput) {
    output += clearOkuri();
    OkuriFirst = 1;
    output.Output.Unconfirmed += WordBuf;
    output.Output.Unconfirmed += '*';
  } else {
    output.Output.Unconfirmed += WordBuf;
  }
  return output;
}

void
kkClearBuf()
{
  WordBufLen = 0;
}

SkkResult
kkBegV(char c, bool)
{
  SkkResult output;
  output.NextMode = SkkModes::KINPUT_MODE;
  kanjiInputEffect(1);
  kkClearBuf();
  output += bufferedInput(romkan::inputKanaVowel(tolower(c)));
  return output;
}

SkkResult
kkBegC(char c, bool)
{
  SkkResult output;
  output.NextMode = SkkModes::KINPUT_MODE;
  kanjiInputEffect(1);
  kkClearBuf();
  output += bufferedInput(romkan::inputKanaConso(tolower(c)));
  return output;
}

SkkResult
kkBegA(char c, bool)
{
  romkan::flushKana();
  SkkResult output;
  output.NextKeymap = KeymapTypes::KAlphaInput;
  output.NextMode = KINPUT_MODE;
  kanjiInputEffect(1);
  WordBufLen = 0;
  return output;
}

SkkResult
kalpha(char c, bool)
{
  SkkResult output;
  output.Output.Unconfirmed += c;
  WordBuf[WordBufLen++] = c;
  return output;
}

SkkResult
kaBS(char c, bool)
{
  if (WordBufLen > 0) {
    kanjiInputEffect(0);
    rubout(1);
    WordBufLen--;
    kanjiInputEffect(1);
  }
  return {};
}

SkkResult
kKanaV(char c, bool)
{
  return bufferedInput(romkan::inputKanaVowel(c));
}

static SkkResult
putOkuri(std::string_view s)
{
  SkkResult output;
  int l = OkuriBufLen;
  output.Output.Unconfirmed += s;
  for (auto c : s) {
    OkuriBuf[OkuriBufLen++] = c;
    if (OkuriBufLen >= OKURI_LEN) {
      bell();
      OkuriBuf[l] = '\0';
      OkuriBufLen = l;
      rubout(s.size());
      return output;
    }
  }
  OkuriBuf[OkuriBufLen] = '\0';
  return output;
}

SkkResult
okKanaV(char c, bool first)
{
  if (first) {
    WordBuf[WordBufLen++] = c;
  }
  rubout(1);
  auto output = putOkuri(romkan::inputKanaVowel(tolower(c)));
  output += kkconv(c);
  return output;
}

SkkResult
kKanaC(char c, bool)
{
  return bufferedInput(romkan::inputKanaConso(c));
}

SkkResult
okKanaC(char c, bool first)
{
  SkkResult output;
  if (Nconso == 0) {
    WordBuf[WordBufLen++] = c;
  } else if (first) {
    output += bufferedInput(romkan::flushLastConso('\0'));
    WordBuf[WordBufLen++] = c;
  }
  output += putOkuri(romkan::inputKanaConso(tolower(c)));
  return output;
}

SkkResult
kZenAl(char c, bool)
{
  return bufferedInput(zenkakualpha::inputZenkakuAlpha(c));
}

SkkResult
kZenEx(char c, bool)
{
  return bufferedInput(zenkakualpha::inputZenkakuEx(c));
}

SkkResult
kfthru(char c, bool)
{
  static char buf[2];
  buf[0] = c;
  buf[1] = '\0';
  return bufferedInput(buf);
}

SkkResult
fxthru(char, bool)
{
  /* fix and through */
  auto output = fixIt();
  output.ReInput = true;
  return output;
}

static void
endKanjiInput()
{
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  OkuriInput = 0;
  BlockTty = 0;
}

SkkResult
kfCancel(char c, bool)
{
  kanjiInputEffect(0);
  romkan::cancelConso();
  rubout(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  endKanjiInput();
  return SkkResult{ .NextKeymap = KeymapTypes::Kana };
}

SkkResult
kfFix(char c, bool)
{
  SkkResult output;
  csrLeft(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  kanjiInputEffect(0);
  output.Output.Confirmed += WordBuf;
  endKanjiInput();
  output.NextKeymap = KeymapTypes::Kana;
  return output;
}

SkkResult
kfFixToAsc(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  return output;
}

SkkResult
kfFixToZenA(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Zenkaku;
  output.NextMode = ZENEI_MODE;
  return output;
}

SkkResult
kfFixThru(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += c;
  return output;
}

SkkResult
thruKfFixToAsc(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  output.Output.Confirmed += c;
  return output;
}

SkkResult
okfFix(char c, bool)
{
  auto output = cancelOkuri();
  // if (skk->is_okuri_input())
  //   output += cancelOkuri();
  output += kfFix(0);
  return output;
}

SkkResult
okfFixToAsc(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  return output;
}

SkkResult
okfFixToZenA(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Zenkaku;
  output.NextMode = ZENEI_MODE;
  return output;
}

SkkResult
okfFixThru(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += c;
  return output;
}

SkkResult
thruOkfFixToAsc(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  output.Output.Confirmed += c;
  return output;
}

SkkResult
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
  return {};
}

SkkResult
okuriBS(char c, bool)
{
  kanjiInputEffect(0);
  int n = Nconso;
  char con[MAX_CONSO];
  for (int i = 0; i < n; i++)
    con[i] = LastConso[i];

  SkkResult output;
  if (n <= 1) {
    output += cancelOkuri();
  } else {
    romkan::cancelConso();
    for (int i = 0; i < n; i++)
      output += okKanaC(con[i], 0);
  }
  kanjiInputEffect(1);
  return output;
}

SkkResult
kkconv(char c, bool)
{
  kanjiInputEffect(0);
  auto output = bufferedInput(romkan::flushLastConso('\0'));
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput();
    output.NextKeymap = KeymapTypes::Kana;
  }
  output.NextMode = KSELECT_MODE;

  WordBuf[WordBufLen] = '\0';
  Current.List = UserDic->getCand(WordBuf);
  if (Current.List) {
    Current.Cand = Current.List->begin();
  } else {
    Current.Cand = {};
  }

  int l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  output.NextKeymap = KeymapTypes::Selection;

  if (Current.IsEnabled()) {
    BlockTty = 1;
    output += showCand();
    return output;
  } else {
    /* Really, enter register mode */
    if (PreserveOnFailure) {
      bell();
      output += backToKanjiInput();
      return output;
    } else {
      endKanjiInput();
      output.NextKeymap = KeymapTypes::Kana;
      return output;
    }
  }
}

SkkResult
kOkuri(char c, bool)
{
  char okuri = tolower(c);

  if (WordBufLen == 0) {
    // Recover chattering effect
    if (vowel_from_char(okuri)) {
      return kKanaV(okuri);
    }
    romkan::cancelConso();
    endKanjiInput();
    kanjiInputEffect(0);
    return { .NextKeymap = KeymapTypes::Kana };
  }

  auto output = toOkuri();
  output.Output.Unconfirmed += '*';
  output.ReInput = okuri;
  output.Okuri = true;
  return output;
}

SkkResult
stSuffix(char c, bool)
{
  auto output = fixIt();
  output.NextMode = SkkModes::KINPUT_MODE;
  kanjiInputEffect(1);
  kkClearBuf();
  output += kfthru(c);
  return output;
}

SkkResult
stPrefixCv(char c, bool)
{
  if (WordBufLen == 0) {
    return kfthru(c);
  } else {
    auto output = kfthru(c);
    output += kkconv(' ');
    return output;
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

SkkResult
nxCand(char, bool)
{
  auto l = clearCurrent();

  Current.Increment();

  // draw
  SkkResult output;
  csrLeft(l);
  kanjiSelectionEffect(1);
  output.Output.Unconfirmed += *Current.Cand;
  if (OkuriInput) {
    output.Output.Unconfirmed += OkuriBuf;
  }
  return output;
}

SkkResult
pvCand(char, bool)
{
  auto l = clearCurrent();

  SkkResult output;
  if (Current.Decrement()) {
    csrLeft(l);
    kanjiSelectionEffect(1);
    output.Output.Unconfirmed += Current.Cand->c_str();
    if (OkuriInput) {
      output.Output.Unconfirmed += OkuriBuf;
    }
  } else {
    output += backToKanjiInput();
  }
  return output;
}

SkkResult
cancelOkuri(char, bool)
{
  SkkResult output;
  kanjiInputEffect(0);
  if (Nconso == 0) {
    rubout(1);
    OkuriInput = 0;
    output.NextKeymap = KeymapTypes::KanjiInput;
  } else {
    romkan::cancelConso();
    output += clearOkuri();
    OkuriFirst = 1;
  }
  kanjiInputEffect(1);
  return output;
}

SkkResult
fixIt(char, bool)
{
  kanjiSelectionEffect(0);
  SkkResult output;
  if (Current.IsEnabled()) {
    auto l = Current.Cand->size();
    if (OkuriInput)
      l += strlen(OkuriBuf);
    csrLeft(l);
    output.Output.Confirmed += *Current.Cand;
    if (OkuriInput) {
      output.Output.Confirmed += OkuriBuf;
    }
  }
  endKanjiInput();
  output.NextKeymap = KeymapTypes::Kana;
  return output;
}

SkkResult
thruFixItToAsc(char c, bool o)
{
  auto output = fixIt();
  output.Output.Confirmed += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  output.Output.Confirmed += c;
  return output;
}

SkkResult
cancelSel(char c, bool)
{
  kanjiSelectionEffect(0);
  if (Current.IsEnabled()) {
    auto l = Current.Cand->size();
    if (OkuriInput)
      l += strlen(OkuriBuf);
    rubout(l);
  }
  return backToKanjiInput();
}

SkkResult
h2kkana(char c, bool)
{
  kanjiInputEffect(0);
  auto output = bufferedInput(romkan::flushLastConso('\0'));
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput();
    output.NextKeymap = KeymapTypes::Kana;
  }
  WordBuf[WordBufLen] = '\0';

  auto l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  romkan::hira2kata(WordBuf);
  output.Output.Confirmed += WordBuf;
  endKanjiInput();
  output.NextKeymap = KeymapTypes::Kana;
  return output;
}
