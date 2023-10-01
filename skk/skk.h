#pragma once
#include "keymap.h"
#include <assert.h>
#include <unordered_map>

SkkOutput
nulcmd(char, bool);

class Skk
{
  SkkModes lastmode = {};
  SkkModes curmode = {};

  std::unordered_map<KeymapTypes, Keymap> m_keymaps{
    { KeymapTypes::Normal, Keymap{ KeymapTypes::Normal } },
    { KeymapTypes::Selection, Keymap{ KeymapTypes::Selection } },
    { KeymapTypes::CodeInput, Keymap{ KeymapTypes::CodeInput } },
    { KeymapTypes::Kana, Keymap{ KeymapTypes::Kana } },
    { KeymapTypes::Zenkaku, Keymap{ KeymapTypes::Zenkaku } },
    { KeymapTypes::KanjiInput, Keymap{ KeymapTypes::KanjiInput } },
    { KeymapTypes::OkuriInput, Keymap{ KeymapTypes::OkuriInput } },
    { KeymapTypes::KAlphaInput, Keymap{ KeymapTypes::KAlphaInput } },
  };
  Keymap& NormalKeymap() { return m_keymaps[KeymapTypes::Normal]; }
  Keymap& SelectionKeymap() { return m_keymaps[KeymapTypes::Selection]; }
  Keymap& CodeInputKeymap() { return m_keymaps[KeymapTypes::CodeInput]; }
  Keymap& KanaKeymap() { return m_keymaps[KeymapTypes::Kana]; }
  Keymap& ZenkakuKeymap() { return m_keymaps[KeymapTypes::Zenkaku]; }
  Keymap& KanjiInputKeymap() { return m_keymaps[KeymapTypes::KanjiInput]; }
  Keymap& OkuriInputKeymap() { return m_keymaps[KeymapTypes::OkuriInput]; }
  Keymap& KAlphaInputKeymap() { return m_keymaps[KeymapTypes::KAlphaInput]; }

  KeymapPtr CurrentKeymap = nullptr;
  KeymapPtr lastKeymap = nullptr;

public:
  std::string KanaKey;

  Skk();
  ~Skk();
  Skk(const Skk&) = delete;
  Skk& operator=(const Skk&) = delete;

  void initialize();

  Keymap& currentKeymap() { return *CurrentKeymap; }

  void showmode(SkkModes s);
  void showcurmode();
  void showlastmode();

  void setKanaKey();
  void setKeymap(KeymapPtr _new);
  void setKeymap(KeymapTypes t);
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

  bool is_okuri_input();

  void kkBeg();
  SkkOutput input(uint8_t c, bool okuri = false);
};
