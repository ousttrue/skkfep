#pragma once
#include <assert.h>
#include <span>
#include <stdint.h>
#include <string>
#include <vector>

enum SkkModes
{
  SKK_MODE,
  KANA_MODE,
  ZENEI_MODE,
  KKANA_MODE,
  KINPUT_MODE,
  KSELECT_MODE,
};
inline const char*
mode_string(SkkModes m)
{
  switch (m) {
    case SKK_MODE:
      return "SKK";
    case KANA_MODE:
      return "かな";
    case ZENEI_MODE:
      return "全英";
    case KKANA_MODE:
      return "カナ";
    case KINPUT_MODE:
      return "単語入力";
    case KSELECT_MODE:
      return "単語選択";
  }
}

struct PreEdit
{
  // internal raw input
  std::string Input;
  // show user interface
  std::string Display;
};

struct SkkResult
{
  // write child pty
  std::string Output;
  // write stdout
  PreEdit Predit;
  SkkModes Mode;
};

enum class KeymapTypes
{
  Normal,
  Selection,
  CodeInput,
  Escaped,
  KAlphaInput,
  OkuriInput,
  KanjiInput,
};

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

void
nulcmd(char c);

class Skk
{
  SparseKeymap NormalKeymap;
  SparseKeymap SelectionKeymap;
  SparseKeymap CodeInputKeymap;
  SparseKeymap EscapedKeymap;
  EscapeBehavior CurrentEscapeBehavior = NoEsc;
  EscapeBehavior LastEscapeBehavior = SimpleEsc;
  Keymap KanaKeymap;
  Keymap ZenkakuKeymap;
  Keymap KanjiInputKeymap;
  Keymap OkuriInputKeymap;
  Keymap KAlphaInputKeymap;
  KeymapPtr CurrentKeymap;
  KeymapPtr lastKeymap;
  std::vector<SparseKeymapBody> _ViEscKeymap;
  std::vector<SparseKeymapBody> _EmacsEscKeymap;

public:
  std::string KanaKey;

  Skk();
  ~Skk();
  Skk(const Skk&) = delete;
  Skk& operator=(const Skk&) = delete;

  void setKanaKey();
  void setKeymap(KeymapPtr _new);
  void setKeymap(SkkModes m)
  {
    switch (m) {
      case KINPUT_MODE:
        setKeymap(&KanjiInputKeymap);
        break;

      case SKK_MODE:
      case KANA_MODE:
      case ZENEI_MODE:
      case KKANA_MODE:
      case KSELECT_MODE:
        assert(false);
        break;
    }
  }
  void setKeymap(KeymapTypes t)
  {
    switch (t) {
      case KeymapTypes::Selection:
        setKeymap(convertKeymap(SelectionKeymap));
        break;

      case KeymapTypes::Normal:
      case KeymapTypes::CodeInput:
      case KeymapTypes::Escaped:
      case KeymapTypes::KAlphaInput:
      case KeymapTypes::OkuriInput:
      case KeymapTypes::KanjiInput:
        assert(false);
        break;
    }
  }
  void changeKey(SparseKeymap* skm, KeyFunc func, uint8_t newkey);
  void restoreKeymap();

  void setEscape(EscapeBehavior b, bool init = false);
  void toggleEscape(EscapeBehavior b);
  bool is_okuri_input();

  void toKana();
  void cancelCode();
  void toAsc();
  void toZenA();
  void toEsc();

  SkkResult input(uint8_t c, bool okuri = false);
  void putc(char c);

private:
  void initializeKeymap();
  KeymapPtr convertKeymap(const SparseKeymap& skm);
};
extern Skk g_skk;

void
thruToAsc(char c);

void
thruToEsc(char c);

void
thruBack(char c);

void
thru1(char c);
