#pragma once
#include "conversionmode.h"

namespace skk {

struct SelectionMode : ConversionMode
{
  struct Dictionary* UserDic = nullptr;

  SelectionMode()
    : ConversionMode(ConversionType::Selection, "選択")
  {
  }
  Result input(uint8_t c) override;

  void open_dictionary(std::string_view UserDicName);
  void save_dictionary();
};

} // namespace
