#include "skk.h"
#include "connsh.h"
#include "ctrlcode.h"
#include "etc.h"
#include "kkconv.h"
#include "romkan.h"
#include <assert.h>
#include <stdio.h>

Skk g_skk;

void
nulcmd(char c)
{
}

#define DEFAULT_KANAKEY "^j"

Skk::Skk()
{
  KanaKey = DEFAULT_KANAKEY;
  initializeKeymap();
}

Skk::~Skk() {}

void
Skk::setKeymap(KeymapPtr _new)
{
  lastKeymap = CurrentKeymap;
  CurrentKeymap = _new;
}

void
Skk::restoreKeymap()
{
  CurrentKeymap = lastKeymap;
}

SkkResult
Skk::input(uint8_t c, bool okuri)
{
  auto keyFunc = (*CurrentKeymap)[c];
  keyFunc(c /*, o*/);
  return {};
}

static int
code(const char* p)
{
  int k;
  if (p[0] == '\\') {
    switch (p[1]) {
      case 'a':
        return '\007';
      case 'b':
        return '\b';
      case 'f':
        return '\f';
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case 't':
        return '\t';
      case 'v':
        return '\v';
      case 'x':
        sscanf(p + 2, "%x", &k);
        return k;
      default:
        if (('0' <= p[1]) && (p[1] <= '9')) {
          sscanf(p + 1, "%o", &k);
          return k;
        } else {
          p++;
        }
        break;
    }
  } else if (p[0] == '^') {
    p++;
  }
  k = p[0];
  if (k >= '`')
    k -= ('a' - 'A');
  if (k >= ' ')
    k ^= 0x40;
  return k;
}

void
Skk::setKanaKey()
{
  auto k = code(KanaKey.c_str());
  printf("KanaKey=^%c\n", k ^ 0x40);
  changeKey(
    &NormalKeymap, [](auto) { g_skk.toKana(); }, k);
  changeKey(&SelectionKeymap, fixIt, k);
  changeKey(
    &CodeInputKeymap, [](auto) { g_skk.toKana(); }, k);
  KanaKeymap[k] = nulcmd;
  ZenkakuKeymap[k] = [](auto) { g_skk.toKana(); };
  KanjiInputKeymap[k] = kfFix;
  OkuriInputKeymap[k] = okfFix;
  KAlphaInputKeymap[k] = kfFix;
}

void
Skk::setEscape(EscapeBehavior b, bool init)
{
  if (init) {
    LastEscapeBehavior = NoEsc;
  }

  if (b == ToggleEsc)
    return;
  if (b == NoEsc) {
    changeKey(&SelectionKeymap, thruFixItToAsc, EXTRA_CODE);
    KanaKeymap[ESC_CODE] = romkan::flthru;
    ZenkakuKeymap[ESC_CODE] = thru;
    KanjiInputKeymap[ESC_CODE] = nulcmd;
    KAlphaInputKeymap[ESC_CODE] = nulcmd;
    OkuriInputKeymap[ESC_CODE] = nulcmd;
  } else if (b == SimpleEsc) {
    changeKey(&SelectionKeymap, thruFixItToAsc, ESC_CODE);
    KanaKeymap[ESC_CODE] = thruToAsc;
    ZenkakuKeymap[ESC_CODE] = thruToAsc;
    KanjiInputKeymap[ESC_CODE] = thruKfFixToAsc;
    KAlphaInputKeymap[ESC_CODE] = thruKfFixToAsc;
    OkuriInputKeymap[ESC_CODE] = thruOkfFixToAsc;
  } else {
    changeKey(&SelectionKeymap, thruFixItToEsc, ESC_CODE);
    KanaKeymap[ESC_CODE] = thruToEsc;
    ZenkakuKeymap[ESC_CODE] = thruToEsc;
    KanjiInputKeymap[ESC_CODE] = thruKfFixToEsc;
    KAlphaInputKeymap[ESC_CODE] = thruKfFixToEsc;
    OkuriInputKeymap[ESC_CODE] = thruOkfFixToEsc;
    if (b == ViEsc) {
      EscapedKeymap.defaultfunc = thru;
      EscapedKeymap.keymap = _ViEscKeymap;
    } else {
      EscapedKeymap.defaultfunc = thru1;
      EscapedKeymap.keymap = _EmacsEscKeymap;
    }
  }
  CurrentEscapeBehavior = b;
}

void
Skk::toggleEscape(EscapeBehavior b)
{
  EscapeBehavior t = CurrentEscapeBehavior;
  if (b == ToggleEsc || (b == NoEsc && LastEscapeBehavior == NoEsc)) {
    setEscape(LastEscapeBehavior);
    LastEscapeBehavior = t;
  } else {
    setEscape(b);
    if (t == NoEsc)
      LastEscapeBehavior = t;
  }

  switch (CurrentEscapeBehavior) {
    case NoEsc:
      showmessage("Escape mode off");
      break;
    case SimpleEsc:
      showmessage("Simple escape mode");
      break;
    case ViEsc:
      showmessage("Vi escape mode");
      break;
    case EmacsEsc:
      showmessage("Emacs escape mode");
      break;
  }
}

KeymapPtr
Skk::convertKeymap(const SparseKeymap& skm)
{
  static Keymap keymap_body;
  keymap_body.setall(skm.defaultfunc);
  keymap_body.overrideKeymap(skm.keymap);
  return &keymap_body;
}

void
Skk::changeKey(SparseKeymap* skm, KeyFunc func, uint8_t newkey)
{
  // TODO: remove old map
  // for (auto [k, v] : skm->keymap) {
  //   if (v == func) {
  //     keymap.key = newkey;
  //     return;
  //   }
  // }

  skm->keymap[newkey] = func;
}

bool
Skk::is_okuri_input()
{
  return CurrentKeymap == &OkuriInputKeymap;
}

void
Skk::toKana()
{
  setKeymap(&KanaKeymap);
  romkan::toggleKana();
}

void
Skk::cancelCode()
{
  setKeymap(&KanaKeymap);
  showmode(KANA_MODE);
}

void
Skk::toAsc()
{
  romkan::flushKana();
  setKeymap(convertKeymap(NormalKeymap));
  showmode(SKK_MODE);
}

void
Skk::toZenA()
{
  romkan::flushKana();
  setKeymap(&ZenkakuKeymap);
  showmode(ZENEI_MODE);
}

void
Skk::toEsc()
{
  romkan::flushKana();
  setKeymap(convertKeymap(EscapedKeymap));
  showmode(SKK_MODE);
}

void
thruToAsc(char c)
{
  g_skk.toAsc();
  thru(c);
}

void
thruToEsc(char c)
{
  g_skk.toEsc();
  thru(c);
}

void
thruBack(char c)
{
  thru(c);
  g_skk.toKana();
}

void
thru1(char c)
{
  thru(c);
  g_skk.restoreKeymap();
  showlastmode();
}
