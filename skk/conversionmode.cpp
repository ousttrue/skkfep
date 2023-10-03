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

  return InputMode->input(c);
}

Result
EntryMode::input(uint8_t c)
{
  return InputMode->input(c);
}

Result
OkuriMode::input(uint8_t c)
{
  return InputMode->input(c);
}

Result
SelectionMode::input(uint8_t c)
{
  return InputMode->input(c);
}

} // namespace
