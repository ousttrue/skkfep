#pragma once

extern char* KanaKey;

enum EscapeBehavior
{
  NoEsc = 0,
  SimpleEsc,
  ViEsc,
  EmacsEsc,
  ToggleEsc
};

using KeymapPtr = void (**)(char);
typedef void (*Keymap[128])(char);
struct SparseKeymapBody
{
  int key;
  void (*function)(char);
};

struct SparseKeymap
{
  void (*defaultfunc)(char);
  SparseKeymapBody* keymap;
};

extern SparseKeymap NormalKeymap, SelectionKeymap, CodeInputKeymap;

extern SparseKeymap EscapedKeymap;
extern SparseKeymapBody _ViEscKeymap[], _EmacsEscKeymap[];

extern Keymap KanaKeymap, ZenkakuKeymap, KanjiInputKeymap, OkuriInputKeymap,
  KAlphaInputKeymap;

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
