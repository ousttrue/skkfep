#pragma once
#include <functional>
#include <span>
#include <stddef.h>
#include <stdint.h>
#include <vector>

enum EscapeBehavior
{
  NoEsc,
  SimpleEsc,
  ViEsc,
  EmacsEsc,
  ToggleEsc
};

// using KeyFunc = std::function<void(char)>;
using KeyFunc = void (*)(char);

struct SparseKeymapBody
{
  uint8_t key = 0;
  KeyFunc function = {};
};

struct SparseKeymap
{
  KeyFunc defaultfunc = {};
  std::vector<SparseKeymapBody> keymap;
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

  void overrideKeymap(std::span<const SparseKeymapBody> body)
  {
    for (auto keymap : body) {
      int c = (unsigned char)keymap.key;
      if (c < 128) {
        Keymap[c] = keymap.function;
      }
    }
  }
};
using KeymapPtr = Keymap*;

extern const char* KanaKey;

extern SparseKeymap NormalKeymap;
extern SparseKeymap SelectionKeymap;
extern SparseKeymap CodeInputKeymap;
extern SparseKeymap EscapedKeymap;

extern Keymap KanaKeymap;
extern Keymap ZenkakuKeymap;
extern Keymap KanjiInputKeymap;
extern Keymap OkuriInputKeymap;
extern Keymap KAlphaInputKeymap;

void
setKanaKey();

void
setEscape(EscapeBehavior b, bool init = false);

void
toggleEscape(EscapeBehavior b);

KeymapPtr
convertKeymap(const SparseKeymap& skm);

void
changeKey(SparseKeymap* skm, KeyFunc func, char newkey);

void
setKeymap(KeymapPtr _new);

void
restoreKeymap();

void
keyinput(char c, char o = 0);

bool
is_okuri_input();
