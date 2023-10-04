#pragma once
#include "inputmode.h"
#include "keymap.h"
#include <string>

namespace zenkakualpha {

skk::Result
iZenAl(char c, bool o = {});

std::string
inputZenkakuAlpha(char c);

skk::Result
iZenEx(char c, bool o = {});

std::string
inputZenkakuEx(char c);

} // namespace

namespace skk {

struct ZenkakuInput : InputMode
{
  ZenkakuInput();
  Result putc(char8_t c) override;
};

} // namespace
