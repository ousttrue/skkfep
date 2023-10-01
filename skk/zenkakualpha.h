#pragma once
#include "keymap.h"
#include <string>

namespace zenkakualpha {

SkkResult
iZenAl(char c, bool o = {});

std::string
inputZenkakuAlpha(char c);

SkkResult
iZenEx(char c, bool o = {});

std::string
inputZenkakuEx(char c);

} // namespace
