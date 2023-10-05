#pragma once
#include "conversionmode.h"
#include "dictionary.h"

namespace skk {

struct SelectionMode : ConversionMode
{
  CandidateSelector Candiates;

  SelectionMode()
    : ConversionMode(ConversionType::Selection, "選択")
  {
  }
  Result input(uint8_t c) override;
  void begin(const CandidateSelector& candiates) { Candiates = candiates; }

private:
  skk::Result fixIt();
};

} // namespace
