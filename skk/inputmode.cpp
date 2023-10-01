#include "inputmode.h"

SkkInputMode
SkkInputMode::ascii()
{
  SkkInputMode inputMode;
  inputMode.Default = [](char c, auto) {
    char str[]{ c, 0 };
    return SkkResult{
      .Output{
        .Confirmed = str,
      },
    };
  };
  return inputMode;
}

SkkInputMode
SkkInputMode::hirakana()
{
  SkkInputMode inputMode;

  return inputMode;
}
