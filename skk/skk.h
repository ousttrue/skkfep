#pragma once
#include "keymap.h"
#include <assert.h>

enum EscapeBehavior
{
  NoEsc,
  SimpleEsc,
  ViEsc,
  EmacsEsc,
  ToggleEsc
};

void
nulcmd(char c);

enum class KeymapTypes
{
  Normal,
  Selection,
  CodeInput,
  Escaped,
  Kana,
  Zenkaku,
  KanjiInput,
  OkuriInput,
  KAlphaInput,
};

class Skk
{
  Keymap KeymapList[9];
  Keymap& NormalKeymap() { return KeymapList[(int)KeymapTypes::Normal]; }
  Keymap& SelectionKeymap() { return KeymapList[(int)KeymapTypes::Selection]; }
  Keymap& CodeInputKeymap() { return KeymapList[(int)KeymapTypes::CodeInput]; }
  Keymap& EscapedKeymap() { return KeymapList[(int)KeymapTypes::Escaped]; }
  Keymap& KanaKeymap() { return KeymapList[(int)KeymapTypes::Kana]; }
  Keymap& ZenkakuKeymap() { return KeymapList[(int)KeymapTypes::Zenkaku]; }
  Keymap& KanjiInputKeymap()
  {
    return KeymapList[(int)KeymapTypes::KanjiInput];
  }
  Keymap& OkuriInputKeymap()
  {
    return KeymapList[(int)KeymapTypes::OkuriInput];
  }
  Keymap& KAlphaInputKeymap()
  {
    return KeymapList[(int)KeymapTypes::KAlphaInput];
  }

  EscapeBehavior CurrentEscapeBehavior = NoEsc;
  EscapeBehavior LastEscapeBehavior = SimpleEsc;
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
  void setKeymap(KeymapTypes t) { setKeymap(&KeymapList[(int)t]); }
  void setKeymap(SkkModes m)
  {
    switch (m) {
      case KANA_MODE:
        setKeymap(KeymapTypes::Kana);
        break;

      case KINPUT_MODE:
        setKeymap(KeymapTypes::KanjiInput);
        break;

      case ZENEI_MODE:
        setKeymap(KeymapTypes::Zenkaku);
        break;

      case KSELECT_MODE:
        setKeymap(KeymapTypes::Selection);
        break;

      case SKK_MODE:
      case KKANA_MODE:
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
