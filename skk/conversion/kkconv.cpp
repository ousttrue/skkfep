#include "kkconv.h"
#include "dictionary.h"
#include "input/romkan.h"
#include "input/zenkakualpha.h"
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

static skk::Result
bufferedInput(std::string_view s)
{
  for (auto c : s) {
    WordBuf[WordBufLen++] = c;
  }
  return { .Output = { .Unconfirmed = { s.begin(), s.end() } } };
}

static skk::Result
showCand()
{
  kanjiSelectionEffect(1);
  skk::Result output;
  output.Output.Unconfirmed += *Current.Cand;
  if (OkuriInput) {
    output.Output.Unconfirmed += OkuriBuf;
  }
  return output;
}

static skk::Result
toOkuri()
{
  OkuriInput = 1;
  *OkuriBuf = '\0';
  OkuriBufLen = 0;
  return {
    .NextConversinMode = skk::ConversionType::Okuri,
  };
}

static skk::Result
clearOkuri()
{
  WordBufLen--;
  WordBuf[WordBufLen] = '\0';
  return toOkuri();
}

/* back to kanji input mode */
static skk::Result
backToKanjiInput()
{
  skk::Result output;
  output.RestoreKeymap = true;
  output.NextConversinMode = skk::ConversionType::Entry;
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

skk::Result
kkBegV(char c, bool)
{
  skk::Result output;
  output.NextConversinMode = skk::ConversionType::Entry;
  kanjiInputEffect(1);
  kkClearBuf();
  output += bufferedInput(romkan::inputKanaVowel(tolower(c)));
  return output;
}

skk::Result
kkBegC(char c, bool)
{
  skk::Result output;
  output.NextConversinMode = skk::ConversionType::Entry;
  kanjiInputEffect(1);
  kkClearBuf();
  output += bufferedInput(romkan::inputKanaConso(tolower(c)));
  return output;
}

skk::Result
kkBegA(char c, bool)
{
  romkan::flushKana();
  skk::Result output;
  output.NextConversinMode = skk::ConversionType::Entry;
  output.NextInputMode = skk::InputType::Ascii;
  kanjiInputEffect(1);
  WordBufLen = 0;
  return output;
}

skk::Result
kalpha(char c, bool)
{
  skk::Result output;
  output.Output.Unconfirmed += c;
  WordBuf[WordBufLen++] = c;
  return output;
}

skk::Result
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

skk::Result
kKanaV(char c, bool)
{
  return bufferedInput(romkan::inputKanaVowel(c));
}

static skk::Result
putOkuri(std::string_view s)
{
  skk::Result output;
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

skk::Result
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

skk::Result
kKanaC(char c, bool)
{
  return bufferedInput(romkan::inputKanaConso(c));
}

skk::Result
okKanaC(char c, bool first)
{
  skk::Result output;
  if (Nconso == 0) {
    WordBuf[WordBufLen++] = c;
  } else if (first) {
    output += bufferedInput(romkan::flushLastConso('\0'));
    WordBuf[WordBufLen++] = c;
  }
  output += putOkuri(romkan::inputKanaConso(tolower(c)));
  return output;
}

skk::Result
kZenAl(char c, bool)
{
  return bufferedInput(zenkakualpha::inputZenkakuAlpha(c));
}

skk::Result
kZenEx(char c, bool)
{
  return bufferedInput(zenkakualpha::inputZenkakuEx(c));
}

skk::Result
kfthru(char c, bool)
{
  static char buf[2];
  buf[0] = c;
  buf[1] = '\0';
  return bufferedInput(buf);
}

skk::Result
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

// 漢字中止
skk::Result
kfCancel(char c, bool)
{
  kanjiInputEffect(0);
  romkan::cancelConso();
  rubout(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  endKanjiInput();
  return skk::Result{ .NextConversinMode = skk::ConversionType::Direct };
}

// 確定
skk::Result
kfFix(char c, bool)
{
  skk::Result output;
  csrLeft(WordBufLen);
  WordBuf[WordBufLen] = '\0';
  kanjiInputEffect(0);
  output.Output.Confirmed += WordBuf;
  endKanjiInput();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Kana;
  return output;
}

// 確定してAscii
skk::Result
kfFixToAsc(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Ascii;
  return output;
}

// 確定して全角
skk::Result
kfFixToZenA(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Zenkaku;
  return output;
}

// 確定して透過
skk::Result
kfFixThru(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += c;
  return output;
}

// 確定してAsciiモード
skk::Result
thruKfFixToAsc(char c, bool)
{
  auto output = kfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Ascii;
  output.Output.Confirmed += c;
  return output;
}

// 送り確定
skk::Result
okfFix(char c, bool)
{
  auto output = cancelOkuri();
  output += kfFix(0);
  return output;
}

// 送り確定 Ascii モード
skk::Result
okfFixToAsc(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Ascii;
  return output;
}

// 送り確定 全角 モード
skk::Result
okfFixToZenA(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Zenkaku;
  return output;
}

// 送り確定 透過
skk::Result
okfFixThru(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += c;
  return output;
}

skk::Result
thruOkfFixToAsc(char c, bool)
{
  auto output = okfFix(c);
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Ascii;
  output.Output.Confirmed += c;
  return output;
}

skk::Result
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

skk::Result
okuriBS(char c, bool)
{
  kanjiInputEffect(0);
  int n = Nconso;
  char con[MAX_CONSO];
  for (int i = 0; i < n; i++)
    con[i] = LastConso[i];

  skk::Result output;
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

// 検索して変換候補をを表示する
skk::Result
kkconv(char c, bool)
{
  kanjiInputEffect(0);
  auto output = bufferedInput(romkan::flushLastConso('\0'));
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput();
    // output.NextInputMode = KeymapTypes::Selection;
  }
  // output.NextConversinMode = skk::ConversionType::Selection;

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
  output.NextConversinMode = skk::ConversionType::Selection;

  if (Current.IsEnabled()) {
    BlockTty = 1;
    output += showCand();
    return output;
  } else {
    // 候補無かった
    if (PreserveOnFailure) {
      // Really, enter register mode
      bell();
      output += backToKanjiInput();
      return output;
    } else {
      endKanjiInput();
      output.NextConversinMode = skk::ConversionType::Direct;
      return output;
    }
  }
}

skk::Result
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
    return { .NextConversinMode = skk::ConversionType::Direct };
  }

  auto output = toOkuri();
  output.Output.Unconfirmed += '*';
  output.ReInput = okuri;
  output.Okuri = true;
  return output;
}

skk::Result
stSuffix(char c, bool)
{
  auto output = fixIt();
  output.NextConversinMode = skk::ConversionType::Entry;
  kanjiInputEffect(1);
  kkClearBuf();
  output += kfthru(c);
  return output;
}

skk::Result
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

skk::Result
nxCand(char, bool)
{
  auto l = clearCurrent();

  Current.Increment();

  // draw
  skk::Result output;
  csrLeft(l);
  kanjiSelectionEffect(1);
  output.Output.Unconfirmed += *Current.Cand;
  if (OkuriInput) {
    output.Output.Unconfirmed += OkuriBuf;
  }
  return output;
}

skk::Result
pvCand(char, bool)
{
  auto l = clearCurrent();

  skk::Result output;
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

skk::Result
cancelOkuri(char, bool)
{
  skk::Result output;
  kanjiInputEffect(0);
  if (Nconso == 0) {
    rubout(1);
    OkuriInput = 0;
    output.NextConversinMode = skk::ConversionType::Entry;
  } else {
    romkan::cancelConso();
    output += clearOkuri();
    OkuriFirst = 1;
  }
  kanjiInputEffect(1);
  return output;
}

skk::Result
fixIt(char, bool)
{
  kanjiSelectionEffect(0);
  skk::Result output;
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
  output.NextConversinMode = skk::ConversionType::Direct;
  return output;
}

skk::Result
thruFixItToAsc(char c, bool o)
{
  auto output = fixIt();
  output.Output.Confirmed += romkan::flushKana();
  output.NextConversinMode = skk::ConversionType::Direct;
  output.NextInputMode = skk::InputType::Ascii;
  output.Output.Confirmed += c;
  return output;
}

skk::Result
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

skk::Result
h2kkana(char c, bool)
{
  kanjiInputEffect(0);
  auto output = bufferedInput(romkan::flushLastConso('\0'));
  romkan::cancelConso();
  if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
    endKanjiInput();
    output.NextConversinMode = skk::ConversionType::Direct;
  }
  WordBuf[WordBufLen] = '\0';

  auto l = WordBufLen;
  if (OkuriInput)
    l += strlen(OkuriBuf) - 1;
  rubout(l);
  romkan::hira2kata(WordBuf);
  output.Output.Confirmed += WordBuf;
  endKanjiInput();
  output.NextInputMode = skk::InputType::Kana;
  return output;
}
