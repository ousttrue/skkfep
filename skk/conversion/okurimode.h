#pragma once
#include "conversionmode.h"

namespace skk {

struct OkuriMode : ConversionMode
{
  OkuriMode()
    : ConversionMode(ConversionType::Okuri, "送り")
  {
  }
  Result input(uint8_t c) override;
};

} // namespace
