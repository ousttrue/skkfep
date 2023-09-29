#pragma once
#include <stddef.h>
#include <stdint.h>

enum EscapeBehavior
{
  NoEsc,
  SimpleEsc,
  ViEsc,
  EmacsEsc,
  ToggleEsc
};

using KeyFunc = void (*)(char);

struct SparseKeymapBody
{
  uint8_t key = 0;
  KeyFunc function = nullptr;
};

struct SparseKeymap
{
  KeyFunc defaultfunc;
  SparseKeymapBody* keymap;
};
struct Keymap
{
  KeyFunc Keymap[128];

  void setall(const KeyFunc& f)
  {
    for (auto& func : Keymap) {
      func = f;
    }
  }

  KeyFunc& operator[](size_t index) { return Keymap[index]; }

  void overrideKeymap(const SparseKeymapBody* keymap)
  {
    for (; keymap->function; ++keymap) {
      int c = (unsigned char)keymap->key;
      if (c < 128) {
        Keymap[c] = keymap->function;
      }
    }
  }
};
using KeymapPtr = Keymap*;

extern const char* KanaKey;
extern SparseKeymapBody _ViEscKeymap[];
extern SparseKeymapBody _EmacsEscKeymap[];

extern SparseKeymap NormalKeymap;
extern SparseKeymap SelectionKeymap;
extern SparseKeymap CodeInputKeymap;
extern SparseKeymap EscapedKeymap;

extern Keymap KanaKeymap;
extern Keymap ZenkakuKeymap;
extern Keymap KanjiInputKeymap;
extern Keymap OkuriInputKeymap;
extern Keymap KAlphaInputKeymap;

extern const char* modeString[];

extern KeymapPtr CurrentKeymap;

extern EscapeBehavior CurrentEscapeBehavior, LastEscapeBehavior;

void
setKanaKey();

void
setEscape(EscapeBehavior b);

void
toggleEscape(EscapeBehavior b);

KeymapPtr
convertKeymap(const SparseKeymap& skm);

int
changeKey(SparseKeymap* skm, void (*func)(char), char newkey);

void
setKeymap(KeymapPtr* current, KeymapPtr _new);

void
restoreKeymap(KeymapPtr* current);
