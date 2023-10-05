#include "inputmode.h"
#include "ctrlcode.h"

namespace skk {

AsciiInput::AsciiInput()
  : InputMode(InputType::Ascii, "ASCII")
{
}

Result
AsciiInput::putc(char8_t c)
{
  if (c == CTRL_J) {
    return Result{
      .NextConversinMode = ConversionType::Direct,
      .NextInputMode = InputType::Kana,
    };
  }
  // auto& KAlphaInputKeymap = m_keymaps[KeymapTypes::KAlphaInput];
  // KAlphaInputKeymap.DefaultFunc = nulcmd;
  // KAlphaInputKeymap.Keymap = {
  //   { CTRL_G, kfCancel }, { CTRL_H, kaBS }, { ' ', kkconv },  { '!', kalpha
  //   }, { '"', kalpha },      { '#', kalpha },  { '$', kalpha },  { '%',
  //   kalpha }, { '^', kalpha },      { '\'', kalpha }, { '(', kalpha },  {
  //   ')', kalpha }, { '*', kalpha },      { '+', kalpha },  { '}', kalpha },
  //   { '-', kalpha }, { '.', kalpha },      { '/', kalpha },  { '0', kalpha },
  //   { '1', kalpha }, { '2', kalpha },      { '3', kalpha },  { '4', kalpha },
  //   { '5', kalpha }, { '6', kalpha },      { '7', kalpha },  { '8', kalpha },
  //   { '9', kalpha }, { ':', kalpha },      { ';', kalpha },  { '<', kalpha },
  //   { '=', kalpha }, { '>', kalpha },      { '?', kalpha },  { '@', kalpha },
  //   { 'A', kalpha }, { 'B', kalpha },      { 'C', kalpha },  { 'D', kalpha },
  //   { 'E', kalpha }, { 'F', kalpha },      { 'G', kalpha },  { 'H', kalpha },
  //   { 'I', kalpha }, { 'J', kalpha },      { 'K', kalpha },  { 'L', kalpha },
  //   { 'M', kalpha }, { 'N', kalpha },      { 'O', kalpha },  { 'P', kalpha },
  //   { 'Q', kalpha }, { 'R', kalpha },      { 'S', kalpha },  { 'T', kalpha },
  //   { 'U', kalpha }, { 'V', kalpha },      { 'W', kalpha },  { 'X', kalpha },
  //   { 'Y', kalpha }, { 'Z', kalpha },      { '[', kalpha },  { '\\', kalpha
  //   }, { ']', kalpha }, { '^', kalpha },      { '_', kalpha },  { '`', kalpha
  //   },  { 'a', kalpha }, { 'b', kalpha },      { 'c', kalpha },  { 'd',
  //   kalpha },  { 'e', kalpha }, { 'f', kalpha },      { 'g', kalpha },  {
  //   'h', kalpha },  { 'i', kalpha }, { 'j', kalpha },      { 'k', kalpha },
  //   { 'l', kalpha },  { 'm', kalpha }, { 'n', kalpha },      { 'o', kalpha },
  //   { 'p', kalpha },  { 'q', kalpha }, { 'r', kalpha },      { 's', kalpha },
  //   { 't', kalpha },  { 'u', kalpha }, { 'v', kalpha },      { 'w', kalpha },
  //   { 'x', kalpha },  { 'y', kalpha }, { 'z', kalpha },      { '{', kalpha },
  //   { '|', kalpha },  { '}', kalpha }, { DEL_CODE, kalpha },
  // };
  char str[]{ (char)c, 0 };
  return { .Output{
    .Confirmed = str,
  } };
}

//
//
CodeInput::CodeInput()
  : InputMode(InputType::Code, "文字コード")
{
}

Result
CodeInput::putc(char8_t c)
{
  // auto& CodeInputKeymap = m_keymaps[KeymapTypes::CodeInput];
  // CodeInputKeymap.DefaultFunc = nulcmd;
  // CodeInputKeymap.Keymap = {
  //   { CTRL_C,
  //     [](auto, auto) { return SkkResult{ .NextKeymap = KeymapTypes::Kana }; }
  //     },
  //   { CTRL_G,
  //     [](auto, auto) { return SkkResult{ .NextKeymap = KeymapTypes::Kana }; }
  //     },
  //   { CTRL_J, [](auto, auto) { return g_codeinput.enterCode(); } },
  //   { CTRL_M, [](auto, auto) { return g_codeinput.enterCode(); } },
  //   { CTRL_T,
  //     [](auto, auto) { return SkkResult{ .NextKeymap = KeymapTypes::Kana }; }
  //     },
  //   { '0', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '1', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '2', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '3', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '4', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '5', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '6', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '7', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '8', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { '9', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'A', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'B', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'C', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'D', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'E', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'F', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'a', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'b', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'c', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'd', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'e', [](auto c, auto) { return g_codeinput.codein(c); } },
  //   { 'f', [](auto c, auto) { return g_codeinput.codein(c); } },
  // };
  return {};
}

} // namespace
