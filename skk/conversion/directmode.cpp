#include "directmode.h"
#include "ctrlcode.h"

namespace skk {

Result
DirectMode::input(uint8_t c)
{
  // auto& KanaKeymap = m_keymaps[KeymapTypes::Kana];
  // KanaKeymap.DefaultFunc = romkan::flthru;
  // KanaKeymap.Keymap = {
  //   { '!', zenkakualpha::iZenAl },
  //   { ',', zenkakualpha::iZenAl },
  //   { '-', zenkakualpha::iZenEx },
  //   { '.', zenkakualpha::iZenAl },
  //   { '/', kkBegA },
  //   { ':', zenkakualpha::iZenAl },
  //   { ';', zenkakualpha::iZenAl },
  //   { '?', zenkakualpha::iZenAl },
  //   { 'A', kkBegV },
  //   { 'B', kkBegC },
  //   { 'C', kkBegC },
  //   { 'D', kkBegC },
  //   { 'E', kkBegV },
  //   { 'F', kkBegC },
  //   { 'G', kkBegC },
  //   { 'H', kkBegC },
  //   { 'I', kkBegV },
  //   { 'J', kkBegC },
  //   { 'K', kkBegC },
  //   { 'L',
  //     [](auto, auto) {
  //       return SkkResult{
  //         .Output = { .Confirmed = romkan::flushKana() },
  //         .NextMode = ZENEI_MODE,
  //         .NextKeymap = KeymapTypes::Zenkaku,
  //       };
  //     } },
  //   { 'M', kkBegC },
  //   { 'N', kkBegC },
  //   { 'O', kkBegV },
  //   { 'P', kkBegC },
  //   { 'Q',
  //     [](auto, auto) {
  //       // self->kkBeg();
  //       SkkResult output;
  //       output.NextMode = SkkModes::KINPUT_MODE;
  //       // kanjiInputEffect(1);
  //       kkClearBuf();
  //       return output;
  //     } },
  //   { 'R', kkBegC },
  //   { 'S', kkBegC },
  //   { 'T', kkBegC },
  //   { 'U', kkBegV },
  //   { 'V', kkBegC },
  //   { 'W', kkBegC },
  //   { 'X', kkBegC },
  //   { 'Y', kkBegC },
  //   { 'Z', kkBegC },
  //   { '[', zenkakualpha::iZenEx },
  //   { '\\', [self = this](auto, auto) { return g_codeinput.inputCode(self); }
  //   }, { ']', zenkakualpha::iZenEx },

  return {
    .Output = InputMode->putc(c),
  };
}

} // namespace
