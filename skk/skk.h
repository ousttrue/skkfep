#pragma once
#include <assert.h>
#include <functional>
#include <span>
#include <stdint.h>
#include <string>
#include <unordered_map>
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

using KeyFunc = std::function<void(uint8_t)>;

struct Keymap
{
  KeyFunc DefaultFunc = {};
  std::unordered_map<uint8_t, KeyFunc> Keymap;

  void setall(const KeyFunc& f)
  {
    DefaultFunc = f;
    Keymap.clear();
  }

  KeyFunc& operator[](uint8_t key)
  {
    auto found = Keymap.find(key);
    if (found != Keymap.end()) {
      return found->second;
    }
    return DefaultFunc;
  }
};
using KeymapPtr = Keymap*;

void
nulcmd(char c);

class Skk
{
  Keymap NormalKeymap;
  Keymap SelectionKeymap;
  Keymap CodeInputKeymap;
  Keymap EscapedKeymap;
  EscapeBehavior CurrentEscapeBehavior = NoEsc;
  EscapeBehavior LastEscapeBehavior = SimpleEsc;
  Keymap KanaKeymap;
  Keymap ZenkakuKeymap;
  Keymap KanjiInputKeymap;
  Keymap OkuriInputKeymap;
  Keymap KAlphaInputKeymap;
  KeymapPtr CurrentKeymap;
  KeymapPtr lastKeymap;
  std::unordered_map<uint8_t, KeyFunc> _ViEscKeymap;
  std::unordered_map<uint8_t, KeyFunc> _EmacsEscKeymap;

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
        setKeymap(&SelectionKeymap);
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
