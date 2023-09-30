#pragma once
#include "keymap.h"
#include <string>

namespace zenkakualpha {

SkkOutput
iZenAl(char c, bool o = {});

std::string
inputZenkakuAlpha(char c);

SkkOutput
iZenEx(char c, bool o = {});

std::string
inputZenkakuEx(char c);

} // namespace
