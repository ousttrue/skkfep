#include "inputmode.h"

namespace skk {

AsciiInput::AsciiInput()
  : InputMode(InputType::Ascii)
{
}

Output
AsciiInput::putc(char8_t c)
{
  char str[]{ (char)c, 0 };
  return Output{
    .Confirmed = str,
  };
}

//
//
CodeInput::CodeInput()
  : InputMode(InputType::Code)
{
}

Output
CodeInput::putc(char8_t c)
{
  return Output{};
}

} // namespace
