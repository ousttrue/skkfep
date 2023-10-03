#pragma once
#include "conversionmode.h"
#include "keymap.h"
#include <assert.h>
#include <memory>
#include <unordered_map>

class Skk
{
  std::shared_ptr<SkkConversionMode> ConversinMode = DirectMode::create();

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
  std::string UserDicName;
  std::string KanaKey;

  Skk();
  ~Skk();
  Skk(const Skk&) = delete;
  Skk& operator=(const Skk&) = delete;

  void initialize();
  void open_dictionary(std::string_view path = {});

  Keymap& currentKeymap() { return *CurrentKeymap; }

  void setKeymap(KeymapPtr _new)
  {
    lastKeymap = CurrentKeymap;
    CurrentKeymap = _new;
  }
  void setKeymap(KeymapTypes t) { setKeymap(&m_keymaps[t]); }
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
  void restoreKeymap() { CurrentKeymap = lastKeymap; }

  SkkOutput input(uint8_t c, bool okuri = false);

  std::string statusline() const;

private:
  void apply(const SkkResult& result);
};
