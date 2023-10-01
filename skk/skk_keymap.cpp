#include "codeinput.h"
#include "ctrlcode.h"
#include "kkconv.h"
#include "romkan.h"
#include "skk.h"
#include "zenkakualpha.h"
#include <stdio.h>

void
Skk::initialize()
{
  auto& NormalKeymap = m_keymaps[KeymapTypes::Normal];
  NormalKeymap.DefaultFunc = [](auto c, auto) {
    SkkOutput output;
    output.Through += c;
    return output;
  };
  NormalKeymap.Keymap[CTRL_T] = [](auto, auto) {
    return SkkOutput{ .NextKeymap = KeymapTypes::Kana };
  };

  auto& KanaKeymap = m_keymaps[KeymapTypes::Kana];
  KanaKeymap.DefaultFunc = romkan::flthru;
  KanaKeymap.Keymap = {
    { CTRL_H, romkan::kanaBS },
    { '!', zenkakualpha::iZenAl },
    { ',', zenkakualpha::iZenAl },
    { '-', zenkakualpha::iZenEx },
    { '.', zenkakualpha::iZenAl },
    { '/',
      [self = this](auto c, auto) {
        kkBegA(self, c);
        return SkkOutput{};
      } },
    { ':', zenkakualpha::iZenAl },
    { ';', zenkakualpha::iZenAl },
    { '?', zenkakualpha::iZenAl },
    { 'A',
      [self = this](auto c, auto) {
        kkBegV(self, c);
        return SkkOutput{};
      } },
    { 'B',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'C',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'D',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'E',
      [self = this](auto c, auto) {
        kkBegV(self, c);
        return SkkOutput{};
      } },
    { 'F',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'G',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'H',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'I',
      [self = this](auto c, auto) {
        kkBegV(self, c);
        return SkkOutput{};
      } },
    { 'J',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'K',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'L',
      [](auto, auto) {
        return SkkOutput{
          .Through = romkan::flushKana(),
          .NextMode = ZENEI_MODE,
          .NextKeymap = KeymapTypes::Zenkaku,
        };
      } },
    { 'M',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'N',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'O',
      [self = this](auto c, auto) {
        kkBegV(self, c);
        return SkkOutput{};
      } },
    { 'P',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'Q',
      [self = this](auto, auto) {
        self->kkBeg();
        return SkkOutput{};
      } },
    { 'R',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'S',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'T',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'U',
      [self = this](auto c, auto) {
        kkBegV(self, c);
        return SkkOutput{};
      } },
    { 'V',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'W',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'X',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'Y',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
    { 'Z',
      [self = this](auto c, auto) {
        kkBegC(self, c);
        return SkkOutput{};
      } },
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
        return SkkOutput{
          .Through = romkan::flushKana(),
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
        return SkkOutput{};
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
    SkkOutput output;
    output.Through += c;
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
    { CTRL_H,
      [](auto c, auto) {
        kfBS(c);
        return SkkOutput{};
      } },
    { CTRL_M, [self = this](auto c, auto) { return kfFixThru(self, c); } },
    { ' ', [self = this](auto c, auto) { return kkconv(self, c); } },
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
    { '<', [self = this](auto c, auto) { return stPrefixCv(self, c); } },
    { '=', kfthru },
    { '>', [self = this](auto c, auto) { return stPrefixCv(self, c); } },
    { '?', [self = this](auto c, auto) { return stPrefixCv(self, c); } },
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
    { CTRL_H, [self = this](auto c, auto) { return okuriBS(self, c); } },
    { CTRL_M, okfFixThru },
    { ' ', okfFix },
    { 'A', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'B', okKanaC },
    { 'C', okKanaC },
    { 'D', okKanaC },
    { 'E', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'F', okKanaC },
    { 'G', okKanaC },
    { 'H', okKanaC },
    { 'I', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'J', okKanaC },
    { 'K', okKanaC },
    { 'L', okfFixToZenA },
    { 'M', okKanaC },
    { 'N', okKanaC },
    { 'O', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'P', okKanaC },
    //
    { 'R', okKanaC },
    { 'S', okKanaC },
    { 'T', okKanaC },
    { 'U', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'V', okKanaC },
    { 'W', okKanaC },
    { 'X', okKanaC },
    { 'Y', okKanaC },
    { 'Z', okKanaC },
    { 'a', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'b', okKanaC },
    { 'c', okKanaC },
    { 'd', okKanaC },
    { 'e', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'f', okKanaC },
    { 'g', okKanaC },
    { 'h', okKanaC },
    { 'i', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'j', okKanaC },
    { 'k', okKanaC },
    { 'l', okfFixToAsc },
    { 'm', okKanaC },
    { 'n', okKanaC },
    { 'o', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'p', okKanaC },
    //
    { 'r', okKanaC },
    { 's', okKanaC },
    { 't', okKanaC },
    { 'u', [self = this](auto c, auto o) { return okKanaV(self, c, o); } },
    { 'v', okKanaC },
    { 'w', okKanaC },
    { 'x', okKanaC },
    { 'y', okKanaC },
    { 'z', okKanaC },
  };

  auto& KAlphaInputKeymap = m_keymaps[KeymapTypes::KAlphaInput];
  KAlphaInputKeymap.DefaultFunc = nulcmd;
  KAlphaInputKeymap.Keymap = {
    { CTRL_G, kfCancel },
    { CTRL_H,
      [](auto c, auto) {
        kaBS(c);
        return SkkOutput{};
      } },
    { ' ', [self = this](auto c, auto) { return kkconv(self, c); } },
    { '!', kalpha },
    { '"', kalpha },
    { '#', kalpha },
    { '$', kalpha },
    { '%', kalpha },
    { '^', kalpha },
    { '\'', kalpha },
    { '(', kalpha },
    { ')', kalpha },
    { '*', kalpha },
    { '+', kalpha },
    { '}', kalpha },
    { '-', kalpha },
    { '.', kalpha },
    { '/', kalpha },
    { '0', kalpha },
    { '1', kalpha },
    { '2', kalpha },
    { '3', kalpha },
    { '4', kalpha },
    { '5', kalpha },
    { '6', kalpha },
    { '7', kalpha },
    { '8', kalpha },
    { '9', kalpha },
    { ':', kalpha },
    { ';', kalpha },
    { '<', kalpha },
    { '=', kalpha },
    { '>', kalpha },
    { '?', kalpha },
    { '@', kalpha },
    { 'A', kalpha },
    { 'B', kalpha },
    { 'C', kalpha },
    { 'D', kalpha },
    { 'E', kalpha },
    { 'F', kalpha },
    { 'G', kalpha },
    { 'H', kalpha },
    { 'I', kalpha },
    { 'J', kalpha },
    { 'K', kalpha },
    { 'L', kalpha },
    { 'M', kalpha },
    { 'N', kalpha },
    { 'O', kalpha },
    { 'P', kalpha },
    { 'Q', kalpha },
    { 'R', kalpha },
    { 'S', kalpha },
    { 'T', kalpha },
    { 'U', kalpha },
    { 'V', kalpha },
    { 'W', kalpha },
    { 'X', kalpha },
    { 'Y', kalpha },
    { 'Z', kalpha },
    { '[', kalpha },
    { '\\', kalpha },
    { ']', kalpha },
    { '^', kalpha },
    { '_', kalpha },
    { '`', kalpha },
    { 'a', kalpha },
    { 'b', kalpha },
    { 'c', kalpha },
    { 'd', kalpha },
    { 'e', kalpha },
    { 'f', kalpha },
    { 'g', kalpha },
    { 'h', kalpha },
    { 'i', kalpha },
    { 'j', kalpha },
    { 'k', kalpha },
    { 'l', kalpha },
    { 'm', kalpha },
    { 'n', kalpha },
    { 'o', kalpha },
    { 'p', kalpha },
    { 'q', kalpha },
    { 'r', kalpha },
    { 's', kalpha },
    { 't', kalpha },
    { 'u', kalpha },
    { 'v', kalpha },
    { 'w', kalpha },
    { 'x', kalpha },
    { 'y', kalpha },
    { 'z', kalpha },
    { '{', kalpha },
    { '|', kalpha },
    { '}', kalpha },
    { DEL_CODE, kalpha },
  };

  auto& SelectionKeymap = m_keymaps[KeymapTypes::Selection];
  SelectionKeymap.DefaultFunc = [self = this](auto c, auto) {
    return fxthru(self, c);
  };
  SelectionKeymap.Keymap = {
    { CTRL_G, [self = this](auto c, auto) { return cancelSel(self, c); } },
    { CTRL_T, fixIt },
    { ' ',
      [](auto, auto) {
        nxCand();
        return SkkOutput{};
      } },
    { 'x',
      [self = this](auto, auto) {
        pvCand(self);
        return SkkOutput{};
      } },
    { '>', [self = this](auto c, auto) { return stSuffix(self, c); } },
    { '?', [self = this](auto c, auto) { return stSuffix(self, c); } },
    { '<', [self = this](auto c, auto) { return stSuffix(self, c); } },
    { EXTRA_CODE, thruFixItToAsc },
  };

  auto& CodeInputKeymap = m_keymaps[KeymapTypes::CodeInput];
  CodeInputKeymap.DefaultFunc = nulcmd;
  CodeInputKeymap.Keymap = {
    { CTRL_C,
      [](auto, auto) { return SkkOutput{ .NextKeymap = KeymapTypes::Kana }; } },
    { CTRL_G,
      [](auto, auto) { return SkkOutput{ .NextKeymap = KeymapTypes::Kana }; } },
    { CTRL_J, [](auto, auto) { return g_codeinput.enterCode(); } },
    { CTRL_M, [](auto, auto) { return g_codeinput.enterCode(); } },
    { CTRL_T,
      [](auto, auto) { return SkkOutput{ .NextKeymap = KeymapTypes::Kana }; } },
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
}
