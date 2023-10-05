#include "entrymode.h"

namespace skk {

//
// void
// hira2kata(char* buf)
// {
//   int i = 0;
//   while (buf[i]) {
//     if (buf[0] & 0x80) {
//       for (int j = 0; j <= (int)CON::NN; j++) {
//         for (int k = 0; k < 5; k++) {
//           if (!strncmp(&buf[i], HiraTab[j][k], 2)) {
//             strncpy(&buf[i], KataTab[j][k], 2);
//             goto brk1;
//           }
//         }
//       }
//     brk1:
//       i += 2;
//     } else
//       i++;
//   }
// }

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

} // namespace
