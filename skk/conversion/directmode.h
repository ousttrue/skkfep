#pragma once
#include "conversionmode.h"

namespace skk {

struct DirectMode : ConversionMode
{
  DirectMode()
    : ConversionMode(ConversionType::Direct, "直接")
  {
  }
  Result input(uint8_t c) override;

private:
  skk::Result beginEntryModeVowel(char c);
};

}
