#include "conversionmode.h"
#include "ctrlcode.h"

namespace skk {

Result
DirectMode::input(uint8_t c)
{
  // auto& KanaKeymap = m_keymaps[KeymapTypes::Kana];
  // KanaKeymap.DefaultFunc = romkan::flthru;
  // KanaKeymap.Keymap = {
  //   { CTRL_H, romkan::kanaBS },
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

Result
EntryMode::input(uint8_t c)
{
  // auto& KanjiInputKeymap = m_keymaps[KeymapTypes::KanjiInput];
  // KanjiInputKeymap.DefaultFunc = nulcmd;
  // KanjiInputKeymap.Keymap = {
  //   { CTRL_G, kfCancel },
  //   { CTRL_H, kfBS },
  //   { CTRL_M, kfFixThru },
  //   { ' ', kkconv },
  //   { '!', kZenAl },
  //   { '"', kfthru },
  //   { '#', kfthru },
  //   { '$', kfthru },
  //   { '%', kfthru },
  //   { '^', kfthru },
  //   { '\'', kfthru },
  //   { '(', kfthru },
  //   { ')', kfthru },
  //   { '*', kfthru },
  //   { '+', kfthru },
  //   { '}', kZenAl },
  //   { '-', kZenEx },
  //   { '.', kZenAl },
  //   { '/', kfthru },
  //   { '0', kfthru },
  //   { '1', kfthru },
  //   { '2', kfthru },
  //   { '3', kfthru },
  //   { '4', kfthru },
  //   { '5', kfthru },
  //   { '6', kfthru },
  //   { '7', kfthru },
  //   { '8', kfthru },
  //   { '9', kfthru },
  //   { ':', kZenAl },
  //   { ';', kZenAl },
  //   { '<', stPrefixCv },
  //   { '=', kfthru },
  //   { '>', stPrefixCv },
  //   { '?', stPrefixCv },
  //   { '@', kfthru },
  //   { 'A', kOkuri },
  //   { 'B', kOkuri },
  //   { 'C', kOkuri },
  //   { 'D', kOkuri },
  //   { 'E', kOkuri },
  //   { 'F', kOkuri },
  //   { 'G', kOkuri },
  //   { 'H', kOkuri },
  //   { 'I', kOkuri },
  //   { 'J', kOkuri },
  //   { 'K', kOkuri },
  //   { 'L', kfFixToZenA },
  //   { 'M', kOkuri },
  //   { 'N', kOkuri },
  //   { 'O', kOkuri },
  //   { 'P', kOkuri },
  //   //
  //   { 'R', kOkuri },
  //   { 'S', kOkuri },
  //   { 'T', kOkuri },
  //   { 'U', kOkuri },
  //   { 'V', kOkuri },
  //   { 'W', kOkuri },
  //   { 'X', kOkuri },
  //   { 'Y', kOkuri },
  //   { 'Z', kOkuri },
  //   { '[', kZenEx },
  //   { '\\', kfthru },
  //   { ']', kZenEx },
  //   { '^', kfthru },
  //   { '_', kfthru },
  //   { '`', kfthru },
  //   { 'a', kKanaV },
  //   { 'b', kKanaC },
  //   { 'c', kKanaC },
  //   { 'd', kKanaC },
  //   { 'e', kKanaV },
  //   { 'f', kKanaC },
  //   { 'g', kKanaC },
  //   { 'h', kKanaC },
  //   { 'i', kKanaV },
  //   { 'j', kKanaC },
  //   { 'k', kKanaC },
  //   { 'l', kfFixToAsc },
  //   { 'm', kKanaC },
  //   { 'n', kKanaC },
  //   { 'o', kKanaV },
  //   { 'p', kKanaC },
  //   { 'q', h2kkana },
  //   { 'r', kKanaC },
  //   { 's', kKanaC },
  //   { 't', kKanaC },
  //   { 'u', kKanaV },
  //   { 'v', kKanaC },
  //   { 'w', kKanaC },
  //   { 'x', kKanaC },
  //   { 'y', kKanaC },
  //   { 'z', kKanaC },
  //   { '{', kfthru },
  //   { '|', kfthru },
  //   { '}', kfthru },
  //   { '~', kfthru },
  // };
  return {
    .Output = InputMode->putc(c),
  };
}

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

Result
SelectionMode::input(uint8_t c)
{
  // auto& SelectionKeymap = m_keymaps[KeymapTypes::Selection];
  // SelectionKeymap.DefaultFunc = fxthru;
  // SelectionKeymap.Keymap = {
  //   { CTRL_G, cancelSel }, { CTRL_T, fixIt },
  //   { ' ', nxCand },       { 'x', pvCand },
  //   { '>', stSuffix },     { '?', stSuffix },
  //   { '<', stSuffix },     { EXTRA_CODE, thruFixItToAsc },
  // };
  return {
    .Output = InputMode->putc(c),
  };
}

} // namespace
