#pragma once
#include "conversion/directmode.h"
#include "conversion/entrymode.h"
#include "conversion/okurimode.h"
#include "conversion/selectionmode.h"
#include "input/romkan.h"
#include "input/zenkaku.h"
#include <assert.h>

namespace skk {

struct Skk
{
  std::shared_ptr<DirectMode> m_DirectMode;
  std::shared_ptr<EntryMode> m_EntryMode;
  std::shared_ptr<OkuriMode> m_OkuriMode;
  std::shared_ptr<SelectionMode> m_SelectionMode;
  std::shared_ptr<AsciiInput> m_AsciiInput;
  std::shared_ptr<ZenkakuInput> m_ZenkakuInput;
  std::shared_ptr<KanaInput> m_KanaInput;
  std::shared_ptr<CodeInput> m_CodeInput;
  std::shared_ptr<ConversionMode> CurrentMode;

  Skk(std::string_view kanaKey = "^j");
  ~Skk();
  Skk(const Skk&) = delete;
  Skk& operator=(const Skk&) = delete;

  ConversionType conversionmode() const { return CurrentMode->Type; }
  InputType inputmode() const { return CurrentMode->InputMode->Type; }
  std::string statusline() const
  {
    return CurrentMode->Statusline + CurrentMode->InputMode->Statusline;
  }

  void open_dictionary(std::string_view path = "~/.skk/SKK-JISYO.L");

  Output input(uint8_t c);

private:
  void apply(const Result& result);
};

} // namespace
