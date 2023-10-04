#pragma once
#include "input/inputmode.h"
#include "keymap.h"
#include <memory>

namespace skk {

struct ConversionMode
{
  ConversionType Type;
  std::string Statusline;

protected:
  ConversionMode(ConversionType type, const std::string& statusline)
    : Type(type)
    , Statusline(statusline)
  {
  }

public:
  std::shared_ptr<::skk::InputMode> InputMode;
  virtual ~ConversionMode() {}
  virtual Result input(uint8_t c) = 0;
};

struct DirectMode : ConversionMode
{
  DirectMode()
    : ConversionMode(ConversionType::Direct, "直接")
  {
  }
  Result input(uint8_t c) override;
};

struct EntryMode : ConversionMode
{
  EntryMode()
    : ConversionMode(ConversionType::Entry, "漢字")
  {
  }
  Result input(uint8_t c) override;
};

struct OkuriMode : ConversionMode
{
  OkuriMode()
    : ConversionMode(ConversionType::Okuri, "送り")
  {
  }
  Result input(uint8_t c) override;
};

struct SelectionMode : ConversionMode
{
  SelectionMode()
    : ConversionMode(ConversionType::Selection, "選択")
  {
  }
  Result input(uint8_t c) override;
};

} // namespace
