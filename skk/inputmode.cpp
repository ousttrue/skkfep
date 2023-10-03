#include "inputmode.h"

namespace skk {

AsciiInput::AsciiInput()
{
  Default = [](char c, auto) {
    char str[]{ c, 0 };
    return Result{
      .Output{
        .Confirmed = str,
      },
    };
  };
}

ZenkakuInput::ZenkakuInput() {}

KanaInput::KanaInput() {}

CodeInput::CodeInput() {}

} // namespace
