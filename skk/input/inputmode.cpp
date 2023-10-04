#include "inputmode.h"

namespace skk {

AsciiInput::
AsciiInput()
  : InputMode(InputType::Ascii)
{
}

Output
AsciiInput::input(uint8_t c)
{
  char str[]{ (char)c, 0 };
  return Output{
    .Confirmed = str,
  };
}

//
KanaInput::
KanaInput()
  : InputMode(InputType::Kana)
{
}

Output
KanaInput::input(uint8_t c)
{
  return Output{};
}

//
CodeInput::
CodeInput()
  : InputMode(InputType::Code)
{
}

Output
CodeInput::input(uint8_t c)
{
  return Output{};
}

} // namespace
