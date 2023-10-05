#pragma once
#include "conversionmode.h"

namespace skk {

struct EntryMode : ConversionMode
{
  EntryMode()
    : ConversionMode(ConversionType::Entry, "漢字")
  {
  }
  Result input(uint8_t c) override;
};

} // namespace
