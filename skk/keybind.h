#pragma once
#include <stdint.h>

extern const char* KanaKey;

enum EscapeBehavior
{
  NoEsc,
  SimpleEsc,
  ViEsc,
  EmacsEsc,
  ToggleEsc
};

using KeymapPtr = void (**)(char);
typedef void (*Keymap[128])(char);
struct SparseKeymapBody
{
  uint8_t key = 0;
  void (*function)(char) = nullptr;
};
extern SparseKeymapBody _ViEscKeymap[];
extern SparseKeymapBody _EmacsEscKeymap[];

struct SparseKeymap
{
  void (*defaultfunc)(char);
  SparseKeymapBody* keymap;
};
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
convertKeymap(SparseKeymap* skm);

void
overrideKeymap(KeymapPtr km, SparseKeymap* skm);

int
changeKey(SparseKeymap* skm, void (*func)(char), char newkey);

void
setKeymap(KeymapPtr* current, KeymapPtr _new);

void
restoreKeymap(KeymapPtr* current);

KeymapPtr
convertKeymap();
