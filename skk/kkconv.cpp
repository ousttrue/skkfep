#include "kkconv.h"
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

static SkkOutput
bufferedInput(std::string_view s)
{
  for (auto c : s) {
    WordBuf[WordBufLen++] = c;
  }
  return { .Predit = { s.begin(), s.end() } };
}

static SkkOutput
showCand()
{
  kanjiSelectionEffect(1);
  SkkOutput output;
  output.Predit += *Current.Cand;
  if (OkuriInput) {
    output.Predit += OkuriBuf;
  }
  return output;
}

static SkkOutput
toOkuri()
{
  OkuriInput = 1;
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  return {
    .NextKeymap = KeymapTypes::OkuriInput,
  };
}

static SkkOutput
clearOkuri()
{
  WordBufLen--;
  WordBuf[WordBufLen] = '\0';
  return toOkuri();
}

/* back to kanji input mode */
static SkkOutput
backToKanjiInput()
{
  SkkOutput output;
  output.RestoreKeymap = true;
  output.NextMode = KINPUT_MODE;
  kanjiInputEffect(1);
  if (OkuriInput) {
    output += clearOkuri();
    OkuriFirst = 1;
    output.Predit += WordBuf;
    output.Predit += '*';
  } else {
    output.Predit += WordBuf;
  }
  return output;
}

void
kkClearBuf()
{
  WordBufLen = 0;
}

SkkOutput
kkBegV(char c, bool)
{
  SkkOutput output;
  output.NextMode = SkkModes::KINPUT_MODE;
  kanjiInputEffect(1);
  kkClearBuf();
  output += bufferedInput(romkan::inputKanaVowel(tolower(c)));
  return output;
}

SkkOutput
kkBegC(char c, bool)
{
  SkkOutput output;
  output.NextMode = SkkModes::KINPUT_MODE;
  kanjiInputEffect(1);
  kkClearBuf();
  output += bufferedInput(romkan::inputKanaConso(tolower(c)));
  return output;
}

SkkOutput
kkBegA(char c, bool)
{
  romkan::flushKana();
  SkkOutput output;
  output.NextKeymap = KeymapTypes::KAlphaInput;
  output.NextMode = KINPUT_MODE;
  kanjiInputEffect(1);
  WordBufLen = 0;
  return output;
}

SkkOutput
kalpha(char c, bool)
{
  SkkOutput output;
  output.Predit += c;
  WordBuf[WordBufLen++] = c;
  return output;
}

SkkOutput
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

SkkOutput
kKanaV(char c, bool)
{
  return bufferedInput(romkan::inputKanaVowel(c));
}

static SkkOutput
putOkuri(std::string_view s)
{
  SkkOutput output;
  int l = OkuriBufLen;
  output.Predit += s;
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

SkkOutput
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

SkkOutput
kKanaC(char c, bool)
{
  return bufferedInput(romkan::inputKanaConso(c));
}

SkkOutput
okKanaC(char c, bool first)
{
  SkkOutput output;
  if (Nconso == 0) {
    WordBuf[WordBufLen++] = c;
  } else if (first) {
    output += bufferedInput(romkan::flushLastConso('\0'));
    WordBuf[WordBufLen++] = c;
  }
  output += putOkuri(romkan::inputKanaConso(tolower(c)));
  return output;
}

SkkOutput
kZenAl(char c, bool)
{
  return bufferedInput(zenkakualpha::inputZenkakuAlpha(c));
}

SkkOutput
kZenEx(char c, bool)
{
  return bufferedInput(zenkakualpha::inputZenkakuEx(c));
}

SkkOutput
kfthru(char c, bool)
{
  static char buf[2];
  buf[0] = c;
  buf[1] = '\0';
  return bufferedInput(buf);
}

SkkOutput
fxthru(Skk* skk, char c)
{
  /* fix and through */
  auto output = fixIt();
  output += skk->input(c);
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

SkkOutput
kfCancel(char c, bool)
{
  kanjiInputEffect(0);
  romkan::cancelConso();
  rubout(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  endKanjiInput();
  return SkkOutput{ .NextKeymap = KeymapTypes::Kana };
}

SkkOutput
kfFix(char c, bool)
{
  SkkOutput output;
  csrLeft(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  kanjiInputEffect(0);
  output.Through += WordBuf;
  endKanjiInput();
  output.NextKeymap = KeymapTypes::Kana;
  return output;
}

SkkOutput
kfFixToAsc(char c, bool)
{
  auto output = kfFix(c);
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  return output;
}

SkkOutput
kfFixToZenA(char c, bool)
{
  auto output = kfFix(c);
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Zenkaku;
  output.NextMode = ZENEI_MODE;
  return output;
}

SkkOutput
kfFixThru(Skk* skk, char c)
{
  auto output = kfFix(c);
  output.Through += c;
  return output;
}

SkkOutput
thruKfFixToAsc(char c, bool)
{
  auto output = kfFix(c);
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  output.Through += c;
  return output;
}

SkkOutput
okfFix(char c, bool)
{
  auto output = cancelOkuri();
  // if (skk->is_okuri_input())
  //   output += cancelOkuri();
  output += kfFix(0);
  return output;
}

SkkOutput
okfFixToAsc(char c, bool)
{
  auto output = okfFix(c);
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  return output;
}

SkkOutput
okfFixToZenA(char c, bool)
{
  auto output = okfFix(c);
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Zenkaku;
  output.NextMode = ZENEI_MODE;
  return output;
}

SkkOutput
okfFixThru(char c, bool)
{
  auto output = okfFix(c);
  output.Through += c;
  return output;
}

SkkOutput
thruOkfFixToAsc(char c, bool)
{
  auto output = okfFix(c);
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  output.Through += c;
  return output;
}

SkkOutput
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

SkkOutput
okuriBS(char c, bool)
{
  kanjiInputEffect(0);
  int n = Nconso;
  char con[MAX_CONSO];
  for (int i = 0; i < n; i++)
    con[i] = LastConso[i];

  SkkOutput output;
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

SkkOutput
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

SkkOutput
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
  output.Predit += '*';
  output.ReInput = okuri;
  output.Okuri = true;
  return output;
}

SkkOutput
stSuffix(char c, bool)
{
  auto output = fixIt();
  output.NextMode = SkkModes::KINPUT_MODE;
  kanjiInputEffect(1);
  kkClearBuf();
  output += kfthru(c);
  return output;
}

SkkOutput
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

SkkOutput
nxCand(char, bool)
{
  auto l = clearCurrent();

  Current.Increment();

  // draw
  SkkOutput output;
  csrLeft(l);
  kanjiSelectionEffect(1);
  output.Predit += *Current.Cand;
  if (OkuriInput) {
    output.Predit += OkuriBuf;
  }
  return output;
}

SkkOutput
pvCand(char, bool)
{
  auto l = clearCurrent();

  SkkOutput output;
  if (Current.Decrement()) {
    csrLeft(l);
    kanjiSelectionEffect(1);
    output.Predit += Current.Cand->c_str();
    if (OkuriInput) {
      output.Predit += OkuriBuf;
    }
  } else {
    output += backToKanjiInput();
  }
  return output;
}

SkkOutput
cancelOkuri(char, bool)
{
  SkkOutput output;
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

SkkOutput
fixIt(char, bool)
{
  kanjiSelectionEffect(0);
  SkkOutput output;
  if (Current.IsEnabled()) {
    auto l = Current.Cand->size();
    if (OkuriInput)
      l += strlen(OkuriBuf);
    csrLeft(l);
    output.Through += *Current.Cand;
    if (OkuriInput) {
      output.Through += OkuriBuf;
    }
  }
  endKanjiInput();
  output.NextKeymap = KeymapTypes::Kana;
  return output;
}

SkkOutput
thruFixItToAsc(char c, bool o)
{
  auto output = fixIt();
  output.Through += romkan::flushKana();
  output.NextKeymap = KeymapTypes::Normal;
  output.NextMode = SkkModes::SKK_MODE;
  output.Through += c;
  return output;
}

SkkOutput
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

SkkOutput
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
  output.Through += WordBuf;
  endKanjiInput();
  output.NextKeymap = KeymapTypes::Kana;
  return output;
}
