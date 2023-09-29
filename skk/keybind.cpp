#include "keybind.h"
#include "ctrlcode.h"
#include "etc.h"
#include "fep.h"
#include "kkconv.h"
#include "romkan.h"
#include <stdio.h>

extern std::vector<SparseKeymapBody> _ViEscKeymap;
extern std::vector<SparseKeymapBody> _EmacsEscKeymap;

KeymapPtr CurrentKeymap;

#define DEFAULT_KANAKEY "^j"
const char* KanaKey = DEFAULT_KANAKEY;
static KeymapPtr lastKeymap;
EscapeBehavior CurrentEscapeBehavior = NoEsc;
EscapeBehavior LastEscapeBehavior = SimpleEsc;

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
setKanaKey()
{
  int k;
  k = code(KanaKey);
  printf("KanaKey=^%c\n", k ^ 0x40);
  changeKey(&NormalKeymap, romkan::toKana, k);
  changeKey(&SelectionKeymap, fixIt, k);
  changeKey(&CodeInputKeymap, romkan::toKana, k);
  KanaKeymap[k] = nulcmd;
  ZenkakuKeymap[k] = romkan::toKana;
  KanjiInputKeymap[k] = kfFix;
  OkuriInputKeymap[k] = okfFix;
  KAlphaInputKeymap[k] = kfFix;
}

void
setEscape(EscapeBehavior b, bool init)
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
toggleEscape(EscapeBehavior b)
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
convertKeymap(const SparseKeymap& skm)
{
  static Keymap keymap_body;
  keymap_body.setall(skm.defaultfunc);
  keymap_body.overrideKeymap(skm.keymap);
  return &keymap_body;
}

void
changeKey(SparseKeymap* skm, KeyFunc func, char newkey)
{
  for (auto& keymap : skm->keymap) {
    if (keymap.function == func) {
      keymap.key = newkey;
      return;
    }
  }
}

void
setKeymap(KeymapPtr _new)
{
  lastKeymap = CurrentKeymap;
  CurrentKeymap = _new;
}

void
restoreKeymap()
{
  CurrentKeymap = lastKeymap;
}

void
keyinput(char c, char o)
{
  (*CurrentKeymap)[c](c /*, o*/);
}

bool
is_okuri_input()
{
  return CurrentKeymap == &OkuriInputKeymap;
}
