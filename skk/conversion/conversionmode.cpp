#include "conversionmode.h"
#include "ctrlcode.h"

namespace skk {

Result
DirectMode::input(uint8_t c)
{
  if (c == CTRL_J) {
    return Result{
      .NextConversinMode = ConversionType::Direct,
      .NextInputMode = InputType::Kana,
    };
  }

  return {
    .Output = InputMode->putc(c),
  };
}

Result
EntryMode::input(uint8_t c)
{
  return {
    .Output = InputMode->putc(c),
  };
}

Result
OkuriMode::input(uint8_t c)
{
  return {
    .Output = InputMode->putc(c),
  };
}

Result
SelectionMode::input(uint8_t c)
{
  return {
    .Output = InputMode->putc(c),
  };
}

} // namespace
