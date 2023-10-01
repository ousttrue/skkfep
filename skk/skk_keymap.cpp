#include "codeinput.h"
#include "ctrlcode.h"
#include "kkconv.h"
#include "romkan.h"
#include "skk.h"
#include "zenkakualpha.h"
#include <stdio.h>

static int
code(const char* p)
{
  int k;
  if (p[0] == '\\') {
    switch (p[1]) {
      case 'a':
        return '\007';
      case 'b':
        return '\b';
      case 'f':
        return '\f';
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case 't':
        return '\t';
      case 'v':
        return '\v';
      case 'x':
        sscanf(p + 2, "%x", &k);
        return k;
      default:
        if (('0' <= p[1]) && (p[1] <= '9')) {
          sscanf(p + 1, "%o", &k);
          return k;
        } else {
          p++;
        }
        break;
    }
  } else if (p[0] == '^') {
    p++;
  }
  k = p[0];
  if (k >= '`')
    k -= ('a' - 'A');
  if (k >= ' ')
    k ^= 0x40;
  return k;
}
void
Skk::initialize()
{
  auto& NormalKeymap = m_keymaps[KeymapTypes::Normal];
  NormalKeymap.DefaultFunc = [](auto c, auto) {
    SkkResult output;
    output.Output.Confirmed += c;
    return output;
  };
  NormalKeymap.Keymap[CTRL_T] = [](auto, auto) {
    return SkkResult{ .NextKeymap = KeymapTypes::Kana };
  };

  auto& KanaKeymap = m_keymaps[KeymapTypes::Kana];
  KanaKeymap.DefaultFunc = romkan::flthru;
  KanaKeymap.Keymap = {
    { CTRL_H, romkan::kanaBS },
    { '!', zenkakualpha::iZenAl },
    { ',', zenkakualpha::iZenAl },
    { '-', zenkakualpha::iZenEx },
    { '.', zenkakualpha::iZenAl },
    { '/', kkBegA },
    { ':', zenkakualpha::iZenAl },
    { ';', zenkakualpha::iZenAl },
    { '?', zenkakualpha::iZenAl },
    { 'A', kkBegV },
    { 'B', kkBegC },
    { 'C', kkBegC },
    { 'D', kkBegC },
    { 'E', kkBegV },
    { 'F', kkBegC },
    { 'G', kkBegC },
    { 'H', kkBegC },
    { 'I', kkBegV },
    { 'J', kkBegC },
    { 'K', kkBegC },
    { 'L',
      [](auto, auto) {
        return SkkResult{
          .Output = { .Confirmed = romkan::flushKana() },
          .NextMode = ZENEI_MODE,
          .NextKeymap = KeymapTypes::Zenkaku,
        };
      } },
    { 'M', kkBegC },
    { 'N', kkBegC },
    { 'O', kkBegV },
    { 'P', kkBegC },
    { 'Q',
      [](auto, auto) {
        // self->kkBeg();
        SkkResult output;
        output.NextMode = SkkModes::KINPUT_MODE;
        // kanjiInputEffect(1);
        kkClearBuf();
        return output;
      } },
    { 'R', kkBegC },
    { 'S', kkBegC },
    { 'T', kkBegC },
    { 'U', kkBegV },
    { 'V', kkBegC },
    { 'W', kkBegC },
    { 'X', kkBegC },
    { 'Y', kkBegC },
    { 'Z', kkBegC },
    { '[', zenkakualpha::iZenEx },
    { '\\', [self = this](auto, auto) { return g_codeinput.inputCode(self); } },
    { ']', zenkakualpha::iZenEx },
    { 'a', romkan::iKanaV },
    { 'b', romkan::iKanaC },
    { 'c', romkan::iKanaC },
    { 'd', romkan::iKanaC },
    { 'e', romkan::iKanaV },
    { 'f', romkan::iKanaC },
    { 'g', romkan::iKanaC },
    { 'h', romkan::iKanaC },
    { 'i', romkan::iKanaV },
    { 'j', romkan::iKanaC },
    { 'k', romkan::iKanaC },
    { 'l',
      [](auto, auto) {
        return SkkResult{
          .Output = { .Confirmed = romkan::flushKana() },
          .NextMode = SkkModes::SKK_MODE,
          .NextKeymap = KeymapTypes::Normal,
        };
      } },
    { 'm', romkan::iKanaC },
    { 'n', romkan::iKanaC },
    { 'o', romkan::iKanaV },
    { 'p', romkan::iKanaC },
    { 'q',
      [self = this](auto, auto) {
        romkan::tglK(self);
        return SkkResult{};
      } },
    { 'r', romkan::iKanaC },
    { 's', romkan::iKanaC },
    { 't', romkan::iKanaC },
    { 'u', romkan::iKanaV },
    { 'v', romkan::iKanaC },
    { 'w', romkan::iKanaC },
    { 'x', romkan::iKanaC },
    { 'y', romkan::iKanaC },
    { 'z', romkan::iKanaC },
  };

  auto& ZenkakuKeymap = m_keymaps[KeymapTypes::Zenkaku];
  ZenkakuKeymap.DefaultFunc = [](auto c, auto) {
    SkkResult output;
    output.Output.Confirmed += c;
    return output;
  };

  ZenkakuKeymap.Keymap = {
    { ' ', zenkakualpha::iZenAl },  { '!', zenkakualpha::iZenAl },
    { '"', zenkakualpha::iZenAl },  { '#', zenkakualpha::iZenAl },
    { '$', zenkakualpha::iZenAl },  { '%', zenkakualpha::iZenAl },
    { '&', zenkakualpha::iZenAl },  { '\'', zenkakualpha::iZenAl },
    { '(', zenkakualpha::iZenAl },  { ')', zenkakualpha::iZenAl },
    { '*', zenkakualpha::iZenAl },  { '+', zenkakualpha::iZenAl },
    { '}', zenkakualpha::iZenAl },  { '-', zenkakualpha::iZenAl },
    { '.', zenkakualpha::iZenAl },  { '/', zenkakualpha::iZenAl },
    { '0', zenkakualpha::iZenAl },  { '1', zenkakualpha::iZenAl },
    { '2', zenkakualpha::iZenAl },  { '3', zenkakualpha::iZenAl },
    { '4', zenkakualpha::iZenAl },  { '5', zenkakualpha::iZenAl },
    { '6', zenkakualpha::iZenAl },  { '7', zenkakualpha::iZenAl },
    { '8', zenkakualpha::iZenAl },  { '9', zenkakualpha::iZenAl },
    { ':', zenkakualpha::iZenAl },  { ';', zenkakualpha::iZenAl },
    { '<', zenkakualpha::iZenAl },  { '=', zenkakualpha::iZenAl },
    { '>', zenkakualpha::iZenAl },  { '?', zenkakualpha::iZenAl },
    { '@', zenkakualpha::iZenAl },  { 'A', zenkakualpha::iZenAl },
    { 'B', zenkakualpha::iZenAl },  { 'C', zenkakualpha::iZenAl },
    { 'D', zenkakualpha::iZenAl },  { 'E', zenkakualpha::iZenAl },
    { 'F', zenkakualpha::iZenAl },  { 'G', zenkakualpha::iZenAl },
    { 'H', zenkakualpha::iZenAl },  { 'I', zenkakualpha::iZenAl },
    { 'J', zenkakualpha::iZenAl },  { 'K', zenkakualpha::iZenAl },
    { 'L', zenkakualpha::iZenAl },  { 'M', zenkakualpha::iZenAl },
    { 'N', zenkakualpha::iZenAl },  { 'O', zenkakualpha::iZenAl },
    { 'P', zenkakualpha::iZenAl },  { 'Q', zenkakualpha::iZenAl },
    { 'R', zenkakualpha::iZenAl },  { 'S', zenkakualpha::iZenAl },
    { 'T', zenkakualpha::iZenAl },  { 'U', zenkakualpha::iZenAl },
    { 'V', zenkakualpha::iZenAl },  { 'W', zenkakualpha::iZenAl },
    { 'X', zenkakualpha::iZenAl },  { 'Y', zenkakualpha::iZenAl },
    { 'Z', zenkakualpha::iZenAl },  { '[', zenkakualpha::iZenAl },
    { '\\', zenkakualpha::iZenAl }, { ']', zenkakualpha::iZenAl },
    { '^', zenkakualpha::iZenAl },  { '_', zenkakualpha::iZenAl },
    { '`', zenkakualpha::iZenAl },  { 'a', zenkakualpha::iZenAl },
    { 'b', zenkakualpha::iZenAl },  { 'c', zenkakualpha::iZenAl },
    { 'd', zenkakualpha::iZenAl },  { 'e', zenkakualpha::iZenAl },
    { 'f', zenkakualpha::iZenAl },  { 'g', zenkakualpha::iZenAl },
    { 'h', zenkakualpha::iZenAl },  { 'i', zenkakualpha::iZenAl },
    { 'j', zenkakualpha::iZenAl },  { 'k', zenkakualpha::iZenAl },
    { 'l', zenkakualpha::iZenAl },  { 'm', zenkakualpha::iZenAl },
    { 'n', zenkakualpha::iZenAl },  { 'o', zenkakualpha::iZenAl },
    { 'p', zenkakualpha::iZenAl },  { 'q', zenkakualpha::iZenAl },
    { 'r', zenkakualpha::iZenAl },  { 's', zenkakualpha::iZenAl },
    { 't', zenkakualpha::iZenAl },  { 'u', zenkakualpha::iZenAl },
    { 'v', zenkakualpha::iZenAl },  { 'w', zenkakualpha::iZenAl },
    { 'x', zenkakualpha::iZenAl },  { 'y', zenkakualpha::iZenAl },
    { 'z', zenkakualpha::iZenAl },  { '{', zenkakualpha::iZenAl },
    { '|', zenkakualpha::iZenAl },  { '}', zenkakualpha::iZenAl },
    { '~', zenkakualpha::iZenAl },
  };

  auto& KanjiInputKeymap = m_keymaps[KeymapTypes::KanjiInput];
  KanjiInputKeymap.DefaultFunc = nulcmd;
  KanjiInputKeymap.Keymap = {
    { CTRL_G, kfCancel },
    { CTRL_H, kfBS },
    { CTRL_M, kfFixThru },
    { ' ', kkconv },
    { '!', kZenAl },
    { '"', kfthru },
    { '#', kfthru },
    { '$', kfthru },
    { '%', kfthru },
    { '^', kfthru },
    { '\'', kfthru },
    { '(', kfthru },
    { ')', kfthru },
    { '*', kfthru },
    { '+', kfthru },
    { '}', kZenAl },
    { '-', kZenEx },
    { '.', kZenAl },
    { '/', kfthru },
    { '0', kfthru },
    { '1', kfthru },
    { '2', kfthru },
    { '3', kfthru },
    { '4', kfthru },
    { '5', kfthru },
    { '6', kfthru },
    { '7', kfthru },
    { '8', kfthru },
    { '9', kfthru },
    { ':', kZenAl },
    { ';', kZenAl },
    { '<', stPrefixCv },
    { '=', kfthru },
    { '>', stPrefixCv },
    { '?', stPrefixCv },
    { '@', kfthru },
    { 'A', kOkuri },
    { 'B', kOkuri },
    { 'C', kOkuri },
    { 'D', kOkuri },
    { 'E', kOkuri },
    { 'F', kOkuri },
    { 'G', kOkuri },
    { 'H', kOkuri },
    { 'I', kOkuri },
    { 'J', kOkuri },
    { 'K', kOkuri },
    { 'L', kfFixToZenA },
    { 'M', kOkuri },
    { 'N', kOkuri },
    { 'O', kOkuri },
    { 'P', kOkuri },
    //
    { 'R', kOkuri },
    { 'S', kOkuri },
    { 'T', kOkuri },
    { 'U', kOkuri },
    { 'V', kOkuri },
    { 'W', kOkuri },
    { 'X', kOkuri },
    { 'Y', kOkuri },
    { 'Z', kOkuri },
    { '[', kZenEx },
    { '\\', kfthru },
    { ']', kZenEx },
    { '^', kfthru },
    { '_', kfthru },
    { '`', kfthru },
    { 'a', kKanaV },
    { 'b', kKanaC },
    { 'c', kKanaC },
    { 'd', kKanaC },
    { 'e', kKanaV },
    { 'f', kKanaC },
    { 'g', kKanaC },
    { 'h', kKanaC },
    { 'i', kKanaV },
    { 'j', kKanaC },
    { 'k', kKanaC },
    { 'l', kfFixToAsc },
    { 'm', kKanaC },
    { 'n', kKanaC },
    { 'o', kKanaV },
    { 'p', kKanaC },
    { 'q', h2kkana },
    { 'r', kKanaC },
    { 's', kKanaC },
    { 't', kKanaC },
    { 'u', kKanaV },
    { 'v', kKanaC },
    { 'w', kKanaC },
    { 'x', kKanaC },
    { 'y', kKanaC },
    { 'z', kKanaC },
    { '{', kfthru },
    { '|', kfthru },
    { '}', kfthru },
    { '~', kfthru },
  };

  auto& OkuriInputKeymap = m_keymaps[KeymapTypes::OkuriInput];
  OkuriInputKeymap.DefaultFunc = nulcmd;
  OkuriInputKeymap.Keymap = {
    { CTRL_G, cancelOkuri },
    { CTRL_H, okuriBS },
    { CTRL_M, okfFixThru },
    { ' ', okfFix },
    { 'A', okKanaV },
    { 'B', okKanaC },
    { 'C', okKanaC },
    { 'D', okKanaC },
    { 'E', okKanaV },
    { 'F', okKanaC },
    { 'G', okKanaC },
    { 'H', okKanaC },
    { 'I', okKanaV },
    { 'J', okKanaC },
    { 'K', okKanaC },
    { 'L', okfFixToZenA },
    { 'M', okKanaC },
    { 'N', okKanaC },
    { 'O', okKanaV },
    { 'P', okKanaC },
    //
    { 'R', okKanaC },
    { 'S', okKanaC },
    { 'T', okKanaC },
    { 'U', okKanaV },
    { 'V', okKanaC },
    { 'W', okKanaC },
    { 'X', okKanaC },
    { 'Y', okKanaC },
    { 'Z', okKanaC },
    { 'a', okKanaV },
    { 'b', okKanaC },
    { 'c', okKanaC },
    { 'd', okKanaC },
    { 'e', okKanaV },
    { 'f', okKanaC },
    { 'g', okKanaC },
    { 'h', okKanaC },
    { 'i', okKanaV },
    { 'j', okKanaC },
    { 'k', okKanaC },
    { 'l', okfFixToAsc },
    { 'm', okKanaC },
    { 'n', okKanaC },
    { 'o', okKanaV },
    { 'p', okKanaC },
    //
    { 'r', okKanaC },
    { 's', okKanaC },
    { 't', okKanaC },
    { 'u', okKanaV },
    { 'v', okKanaC },
    { 'w', okKanaC },
    { 'x', okKanaC },
    { 'y', okKanaC },
    { 'z', okKanaC },
  };

  auto& KAlphaInputKeymap = m_keymaps[KeymapTypes::KAlphaInput];
  KAlphaInputKeymap.DefaultFunc = nulcmd;
  KAlphaInputKeymap.Keymap = {
    { CTRL_G, kfCancel }, { CTRL_H, kaBS }, { ' ', kkconv },  { '!', kalpha },
    { '"', kalpha },      { '#', kalpha },  { '$', kalpha },  { '%', kalpha },
    { '^', kalpha },      { '\'', kalpha }, { '(', kalpha },  { ')', kalpha },
    { '*', kalpha },      { '+', kalpha },  { '}', kalpha },  { '-', kalpha },
    { '.', kalpha },      { '/', kalpha },  { '0', kalpha },  { '1', kalpha },
    { '2', kalpha },      { '3', kalpha },  { '4', kalpha },  { '5', kalpha },
    { '6', kalpha },      { '7', kalpha },  { '8', kalpha },  { '9', kalpha },
    { ':', kalpha },      { ';', kalpha },  { '<', kalpha },  { '=', kalpha },
    { '>', kalpha },      { '?', kalpha },  { '@', kalpha },  { 'A', kalpha },
    { 'B', kalpha },      { 'C', kalpha },  { 'D', kalpha },  { 'E', kalpha },
    { 'F', kalpha },      { 'G', kalpha },  { 'H', kalpha },  { 'I', kalpha },
    { 'J', kalpha },      { 'K', kalpha },  { 'L', kalpha },  { 'M', kalpha },
    { 'N', kalpha },      { 'O', kalpha },  { 'P', kalpha },  { 'Q', kalpha },
    { 'R', kalpha },      { 'S', kalpha },  { 'T', kalpha },  { 'U', kalpha },
    { 'V', kalpha },      { 'W', kalpha },  { 'X', kalpha },  { 'Y', kalpha },
    { 'Z', kalpha },      { '[', kalpha },  { '\\', kalpha }, { ']', kalpha },
    { '^', kalpha },      { '_', kalpha },  { '`', kalpha },  { 'a', kalpha },
    { 'b', kalpha },      { 'c', kalpha },  { 'd', kalpha },  { 'e', kalpha },
    { 'f', kalpha },      { 'g', kalpha },  { 'h', kalpha },  { 'i', kalpha },
    { 'j', kalpha },      { 'k', kalpha },  { 'l', kalpha },  { 'm', kalpha },
    { 'n', kalpha },      { 'o', kalpha },  { 'p', kalpha },  { 'q', kalpha },
    { 'r', kalpha },      { 's', kalpha },  { 't', kalpha },  { 'u', kalpha },
    { 'v', kalpha },      { 'w', kalpha },  { 'x', kalpha },  { 'y', kalpha },
    { 'z', kalpha },      { '{', kalpha },  { '|', kalpha },  { '}', kalpha },
    { DEL_CODE, kalpha },
  };

  auto& SelectionKeymap = m_keymaps[KeymapTypes::Selection];
  SelectionKeymap.DefaultFunc = fxthru;
  SelectionKeymap.Keymap = {
    { CTRL_G, cancelSel }, { CTRL_T, fixIt },
    { ' ', nxCand },       { 'x', pvCand },
    { '>', stSuffix },     { '?', stSuffix },
    { '<', stSuffix },     { EXTRA_CODE, thruFixItToAsc },
  };

  auto& CodeInputKeymap = m_keymaps[KeymapTypes::CodeInput];
  CodeInputKeymap.DefaultFunc = nulcmd;
  CodeInputKeymap.Keymap = {
    { CTRL_C,
      [](auto, auto) { return SkkResult{ .NextKeymap = KeymapTypes::Kana }; } },
    { CTRL_G,
      [](auto, auto) { return SkkResult{ .NextKeymap = KeymapTypes::Kana }; } },
    { CTRL_J, [](auto, auto) { return g_codeinput.enterCode(); } },
    { CTRL_M, [](auto, auto) { return g_codeinput.enterCode(); } },
    { CTRL_T,
      [](auto, auto) { return SkkResult{ .NextKeymap = KeymapTypes::Kana }; } },
    { '0', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '1', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '2', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '3', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '4', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '5', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '6', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '7', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '8', [](auto c, auto) { return g_codeinput.codein(c); } },
    { '9', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'A', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'B', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'C', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'D', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'E', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'F', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'a', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'b', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'c', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'd', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'e', [](auto c, auto) { return g_codeinput.codein(c); } },
    { 'f', [](auto c, auto) { return g_codeinput.codein(c); } },
  };

  auto k = code(KanaKey.c_str());
  // printf("KanaKey=^%c\n", k ^ 0x40);
  NormalKeymap.Keymap[k] = [](auto, auto) {
    return SkkResult{
      .NextKeymap = KeymapTypes::Kana,
    };
  };
  SelectionKeymap.Keymap[k] = fixIt;
  CodeInputKeymap.Keymap[k] = [](auto, auto) {
    return SkkResult{
      .NextKeymap = KeymapTypes::Kana,
    };
  };
  KanaKeymap.Keymap[k] = nulcmd;
  ZenkakuKeymap.Keymap[k] = [](auto, auto) {
    return SkkResult{
      .NextKeymap = KeymapTypes::Kana,
    };
  };
  KanjiInputKeymap.Keymap[k] = kfFix;
  OkuriInputKeymap.Keymap[k] = okfFix;
  KAlphaInputKeymap.Keymap[k] = kfFix;
}
