#include "directmode.h"
#include "ctrlcode.h"

namespace skk {

Result
DirectMode::input(uint8_t c)
{
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

  switch (c) {
    case 'A':
    case 'I':
    case 'U':
    case 'E':
    case 'O':
      return beginEntryModeVowel(c);
  }

  // case 'B', kkBegC },
  // case 'C', kkBegC },
  // case 'D', kkBegC },
  // case 'F', kkBegC },
  // case 'G', kkBegC },
  // case 'H', kkBegC },
  // case 'J', kkBegC },
  // case 'K', kkBegC },

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
  //   { 'V', kkBegC },
  //   { 'W', kkBegC },
  //   { 'X', kkBegC },
  //   { 'Y', kkBegC },
  //   { 'Z', kkBegC },
  //   { '[', zenkakualpha::iZenEx },
  //   { '\\', [self = this](auto, auto) { return g_codeinput.inputCode(self); }
  //   }, { ']', zenkakualpha::iZenEx },

  return InputMode->putc(c);
}

skk::Result
DirectMode::beginEntryModeVowel(char c)
{
  auto res = InputMode->putc(tolower(c));
  res.NextConversinMode = skk::ConversionType::Entry;
  res.NextInputMode = skk::InputType::Kana;
  res.Output.Unconfirmed += res.Output.Confirmed;
  res.Output.Confirmed.clear();
  return res;
}

} // namespace
