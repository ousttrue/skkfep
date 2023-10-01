#include "conversionmode.h"
#include "ctrlcode.h"

SkkResult
DirectMode::input(uint8_t c)
{
  if (c == CTRL_J) {
    return SkkResult{
      .ConversinMode = SkkConversionModes::Direct,
      .InputMode = SkkInputModes::Hirakana,
    };
  }

  return InputMode.input(c);
}
