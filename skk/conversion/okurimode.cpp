#include "okurimode.h"

namespace skk {

Result
OkuriMode::input(uint8_t c)
{
  // auto& OkuriInputKeymap = m_keymaps[KeymapTypes::OkuriInput];
  // OkuriInputKeymap.DefaultFunc = nulcmd;
  // OkuriInputKeymap.Keymap = {
  //   { CTRL_G, cancelOkuri },
  //   { CTRL_H, okuriBS },
  //   { CTRL_M, okfFixThru },
  //   { ' ', okfFix },
  //   { 'A', okKanaV },
  //   { 'B', okKanaC },
  //   { 'C', okKanaC },
  //   { 'D', okKanaC },
  //   { 'E', okKanaV },
  //   { 'F', okKanaC },
  //   { 'G', okKanaC },
  //   { 'H', okKanaC },
  //   { 'I', okKanaV },
  //   { 'J', okKanaC },
  //   { 'K', okKanaC },
  //   { 'L', okfFixToZenA },
  //   { 'M', okKanaC },
  //   { 'N', okKanaC },
  //   { 'O', okKanaV },
  //   { 'P', okKanaC },
  //   //
  //   { 'R', okKanaC },
  //   { 'S', okKanaC },
  //   { 'T', okKanaC },
  //   { 'U', okKanaV },
  //   { 'V', okKanaC },
  //   { 'W', okKanaC },
  //   { 'X', okKanaC },
  //   { 'Y', okKanaC },
  //   { 'Z', okKanaC },
  //   { 'a', okKanaV },
  //   { 'b', okKanaC },
  //   { 'c', okKanaC },
  //   { 'd', okKanaC },
  //   { 'e', okKanaV },
  //   { 'f', okKanaC },
  //   { 'g', okKanaC },
  //   { 'h', okKanaC },
  //   { 'i', okKanaV },
  //   { 'j', okKanaC },
  //   { 'k', okKanaC },
  //   { 'l', okfFixToAsc },
  //   { 'm', okKanaC },
  //   { 'n', okKanaC },
  //   { 'o', okKanaV },
  //   { 'p', okKanaC },
  //   //
  //   { 'r', okKanaC },
  //   { 's', okKanaC },
  //   { 't', okKanaC },
  //   { 'u', okKanaV },
  //   { 'v', okKanaC },
  //   { 'w', okKanaC },
  //   { 'x', okKanaC },
  //   { 'y', okKanaC },
  //   { 'z', okKanaC },
  // };
  return {
    .Output = InputMode->putc(c),
  };
}

} // namespace
