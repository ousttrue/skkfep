#include "entrymode.h"
#include "dictionary.h"

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
  switch (c) {
    case ' ':
      return kkconv();
  }

  // auto& KanjiInputKeymap = m_keymaps[KeymapTypes::KanjiInput];
  // KanjiInputKeymap.DefaultFunc = nulcmd;
  // KanjiInputKeymap.Keymap = {
  //   { CTRL_G, kfCancel },
  //   { CTRL_H, kfBS },
  //   { CTRL_M, kfFixThru },
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
  return InputMode->putc(c);
}

// 検索して変換候補をを表示する
Result
EntryMode::kkconv()
{
  // kanjiInputEffect(0);
  // auto output = bufferedInput(romkan::flushLastConso('\0'));
  // romkan::cancelConso();

  // if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
  //   endKanjiInput();
  //   // output.NextInputMode = KeymapTypes::Selection;
  // }

  Result res;
  res.NextConversinMode = ConversionType::Selection;

  res.Context = CandidateSelector(UserDic->getCand(WordBuf));
  if (res.Context.List) {
    res.Context.Cand = res.Context.List->begin();
  } else {
    res.Context.Cand = {};
  }

  auto l = WordBuf.size();
  if (OkuriBuf.size()) {
    l += OkuriBuf.size();
  }

  // rubout(l);
  // static skk::Result
  // showCand()
  // {
  //   kanjiSelectionEffect(1);
  //   skk::Result output;
  //   return output;
  // }

  if (res.Context.IsEnabled()) {
    // BlockTty = 1;
    // output += showCand();
    res.Output.Unconfirmed += *res.Context.Cand;
    if (OkuriBuf.size()) {
      res.Output.Unconfirmed += OkuriBuf;
    }
    return res;
  }

  // 候補無かった
  res.NextConversinMode = skk::ConversionType::Direct;
  return res;
}

void
EntryMode::open_dictionary(std::string_view UserDicName)
{
  auto d = new Dictionary;
  if (d->load(UserDicName)) {
    UserDic = d;
  }
}

void
EntryMode::save_dictionary()
{
  if (UserDic) {
    printf("Saving JISYO...\n");
    delete UserDic;
  }
}

void
EntryMode::begin(std::string_view unconfirmed)
{
  WordBuf = unconfirmed;
}

} // namespace
