#pragma once
#include "inputmode.h"

namespace skk {

struct ZenkakuInput : InputMode
{
  ZenkakuInput();
  Result putc(char8_t c) override;
};

} // namespace
