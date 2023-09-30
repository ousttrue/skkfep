#include "codeinput.h"
#include "ctrlcode.h"
#include "kkconv.h"
#include "romkan.h"
#include "skk.h"
#include "zenkakualpha.h"
#include <stdio.h>

void
Skk::initialize(const std::function<void(std::string_view)>& output)
{
  m_output = output;

  auto thru = [self = this](char c, bool) { self->thru(c); };

  auto& NormalKeymap = m_keymaps[KeymapTypes::Normal];
  NormalKeymap.DefaultFunc = thru;
  NormalKeymap.Keymap[CTRL_T] = [self = this](auto, auto) { self->toKana(); };

  auto& KanaKeymap = m_keymaps[KeymapTypes::Kana];
  KanaKeymap.DefaultFunc = romkan::flthru;
  KanaKeymap.Keymap = {
    { CTRL_H,
      [self = this](auto s, auto) { self->m_output(romkan::kanaBS(s)); } },
    { '!',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenAl(c));
      } },
    { ',',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenAl(c));
      } },
    { '-',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenEx(c));
      } },
    { '.',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenAl(c));
      } },
    { '/', [self = this](auto c, auto) { kkBegA(self, c); } },
    { ':',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenAl(c));
      } },
    { ';',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenAl(c));
      } },
    { '?',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenAl(c));
      } },
    { 'A', [self = this](auto c, auto) { kkBegV(self, c); } },
    { 'B', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'C', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'D', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'E', [self = this](auto c, auto) { kkBegV(self, c); } },
    { 'F', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'G', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'H', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'I', [self = this](auto c, auto) { kkBegV(self, c); } },
    { 'J', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'K', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'L', [self = this](auto, auto) { self->toZenA(); } },
    { 'M', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'N', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'O', [self = this](auto c, auto) { kkBegV(self, c); } },
    { 'P', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'Q', [self = this](auto, auto) { self->kkBeg(); } },
    { 'R', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'S', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'T', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'U', [self = this](auto c, auto) { kkBegV(self, c); } },
    { 'V', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'W', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'X', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'Y', [self = this](auto c, auto) { kkBegC(self, c); } },
    { 'Z', [self = this](auto c, auto) { kkBegC(self, c); } },
    { '[',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenEx(c));
      } },
    { '\\', [self = this](auto, auto) { g_codeinput.inputCode(self); } },
    { ']',
      [self = this](auto c, auto) {
        self->m_output(zenkakualpha::iZenEx(c));
      } },
    { 'a', [self = this](auto s, bool) { self->m_output(romkan::iKanaV(s)); } },
    { 'b', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'c', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'd', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'e', [self = this](auto s, bool) { self->m_output(romkan::iKanaV(s)); } },
    { 'f', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'g', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'h', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'i', [self = this](auto s, bool) { self->m_output(romkan::iKanaV(s)); } },
    { 'j', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'k', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'l', [self = this](auto, auto) { self->toAsc(); } },
    { 'm', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'n', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'o', [self = this](auto s, bool) { self->m_output(romkan::iKanaV(s)); } },
    { 'p', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'q',
      [self = this](auto, auto) {
        romkan::tglK(self);
        ;
      } },
    { 'r', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 's', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 't', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'u', [self = this](auto s, bool) { self->m_output(romkan::iKanaV(s)); } },
    { 'v', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'w', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'x', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'y', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
    { 'z', [self = this](auto s, bool) { self->m_output(romkan::iKanaC(s)); } },
  };

  auto& ZenkakuKeymap = m_keymaps[KeymapTypes::Zenkaku];
  ZenkakuKeymap.DefaultFunc = thru;

  auto iZenAl = [self = this](uint8_t c, bool) {
    self->m_output(zenkakualpha::iZenAl(c));
  };

  ZenkakuKeymap.Keymap = {
    { ' ', iZenAl },  { '!', iZenAl }, { '"', iZenAl }, { '#', iZenAl },
    { '$', iZenAl },  { '%', iZenAl }, { '&', iZenAl }, { '\'', iZenAl },
    { '(', iZenAl },  { ')', iZenAl }, { '*', iZenAl }, { '+', iZenAl },
    { '}', iZenAl },  { '-', iZenAl }, { '.', iZenAl }, { '/', iZenAl },
    { '0', iZenAl },  { '1', iZenAl }, { '2', iZenAl }, { '3', iZenAl },
    { '4', iZenAl },  { '5', iZenAl }, { '6', iZenAl }, { '7', iZenAl },
    { '8', iZenAl },  { '9', iZenAl }, { ':', iZenAl }, { ';', iZenAl },
    { '<', iZenAl },  { '=', iZenAl }, { '>', iZenAl }, { '?', iZenAl },
    { '@', iZenAl },  { 'A', iZenAl }, { 'B', iZenAl }, { 'C', iZenAl },
    { 'D', iZenAl },  { 'E', iZenAl }, { 'F', iZenAl }, { 'G', iZenAl },
    { 'H', iZenAl },  { 'I', iZenAl }, { 'J', iZenAl }, { 'K', iZenAl },
    { 'L', iZenAl },  { 'M', iZenAl }, { 'N', iZenAl }, { 'O', iZenAl },
    { 'P', iZenAl },  { 'Q', iZenAl }, { 'R', iZenAl }, { 'S', iZenAl },
    { 'T', iZenAl },  { 'U', iZenAl }, { 'V', iZenAl }, { 'W', iZenAl },
    { 'X', iZenAl },  { 'Y', iZenAl }, { 'Z', iZenAl }, { '[', iZenAl },
    { '\\', iZenAl }, { ']', iZenAl }, { '^', iZenAl }, { '_', iZenAl },
    { '`', iZenAl },  { 'a', iZenAl }, { 'b', iZenAl }, { 'c', iZenAl },
    { 'd', iZenAl },  { 'e', iZenAl }, { 'f', iZenAl }, { 'g', iZenAl },
    { 'h', iZenAl },  { 'i', iZenAl }, { 'j', iZenAl }, { 'k', iZenAl },
    { 'l', iZenAl },  { 'm', iZenAl }, { 'n', iZenAl }, { 'o', iZenAl },
    { 'p', iZenAl },  { 'q', iZenAl }, { 'r', iZenAl }, { 's', iZenAl },
    { 't', iZenAl },  { 'u', iZenAl }, { 'v', iZenAl }, { 'w', iZenAl },
    { 'x', iZenAl },  { 'y', iZenAl }, { 'z', iZenAl }, { '{', iZenAl },
    { '|', iZenAl },  { '}', iZenAl }, { '~', iZenAl },
  };

  auto& KanjiInputKeymap = m_keymaps[KeymapTypes::KanjiInput];
  KanjiInputKeymap.DefaultFunc = nulcmd;
  KanjiInputKeymap.Keymap = {
    { CTRL_G, [self = this](auto c, auto) { kfCancel(self, c); } },
    { CTRL_H, kfBS },
    { CTRL_M, [self = this](auto c, auto) { kfFixThru(self, c); } },
    { ' ', [self = this](auto c, auto) { kkconv(self, c); } },
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
    { '<', [self = this](auto c, auto) { stPrefixCv(self, c); } },
    { '=', kfthru },
    { '>', [self = this](auto c, auto) { stPrefixCv(self, c); } },
    { '?', [self = this](auto c, auto) { stPrefixCv(self, c); } },
    { '@', kfthru },
    { 'A', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'B', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'C', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'D', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'E', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'F', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'G', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'H', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'I', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'J', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'K', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'L', [self = this](auto c, auto) { kfFixToZenA(self, c); } },
    { 'M', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'N', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'O', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'P', [self = this](auto c, auto) { kOkuri(self, c); } },
    //
    { 'R', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'S', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'T', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'U', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'V', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'W', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'X', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'Y', [self = this](auto c, auto) { kOkuri(self, c); } },
    { 'Z', [self = this](auto c, auto) { kOkuri(self, c); } },
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
    { 'l', [self = this](auto c, auto) { kfFixToAsc(self, c); } },
    { 'm', kKanaC },
    { 'n', kKanaC },
    { 'o', kKanaV },
    { 'p', kKanaC },
    { 'q', [self = this](auto c, auto) { h2kkana(self, c); } },
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
    { CTRL_G, [self = this](auto, auto) { cancelOkuri(self); } },
    { CTRL_H, [self = this](auto c, auto) { okuriBS(self, c); } },
    { CTRL_M, [self = this](auto c, auto) { okfFixThru(self, c); } },
    { ' ', [self = this](auto c, auto) { okfFix(self, c); } },
    { 'A', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'B', [](auto c, auto o) { okKanaC(c, o); } },
    { 'C', [](auto c, auto o) { okKanaC(c, o); } },
    { 'D', [](auto c, auto o) { okKanaC(c, o); } },
    { 'E', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'F', [](auto c, auto o) { okKanaC(c, o); } },
    { 'G', [](auto c, auto o) { okKanaC(c, o); } },
    { 'H', [](auto c, auto o) { okKanaC(c, o); } },
    { 'I', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'J', [](auto c, auto o) { okKanaC(c, o); } },
    { 'K', [](auto c, auto o) { okKanaC(c, o); } },
    { 'L', [self = this](auto c, auto o) { okfFixToZenA(self, c); } },
    { 'M', [](auto c, auto o) { okKanaC(c, o); } },
    { 'N', [](auto c, auto o) { okKanaC(c, o); } },
    { 'O', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'P', [](auto c, auto o) { okKanaC(c, o); } },
    //
    { 'R', [](auto c, auto o) { okKanaC(c, o); } },
    { 'S', [](auto c, auto o) { okKanaC(c, o); } },
    { 'T', [](auto c, auto o) { okKanaC(c, o); } },
    { 'U', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'V', [](auto c, auto o) { okKanaC(c, o); } },
    { 'W', [](auto c, auto o) { okKanaC(c, o); } },
    { 'X', [](auto c, auto o) { okKanaC(c, o); } },
    { 'Y', [](auto c, auto o) { okKanaC(c, o); } },
    { 'Z', [](auto c, auto o) { okKanaC(c, o); } },
    { 'a', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'b', [](auto c, auto o) { okKanaC(c, o); } },
    { 'c', [](auto c, auto o) { okKanaC(c, o); } },
    { 'd', [](auto c, auto o) { okKanaC(c, o); } },
    { 'e', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'f', [](auto c, auto o) { okKanaC(c, o); } },
    { 'g', [](auto c, auto o) { okKanaC(c, o); } },
    { 'h', [](auto c, auto o) { okKanaC(c, o); } },
    { 'i', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'j', [](auto c, auto o) { okKanaC(c, o); } },
    { 'k', [](auto c, auto o) { okKanaC(c, o); } },
    { 'l', [self = this](auto c, auto o) { okfFixToAsc(self, c); } },
    { 'm', [](auto c, auto o) { okKanaC(c, o); } },
    { 'n', [](auto c, auto o) { okKanaC(c, o); } },
    { 'o', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'p', [](auto c, auto o) { okKanaC(c, o); } },
    //
    { 'r', [](auto c, auto o) { okKanaC(c, o); } },
    { 's', [](auto c, auto o) { okKanaC(c, o); } },
    { 't', [](auto c, auto o) { okKanaC(c, o); } },
    { 'u', [self = this](auto c, auto o) { okKanaV(self, c, o); } },
    { 'v', [](auto c, auto o) { okKanaC(c, o); } },
    { 'w', [](auto c, auto o) { okKanaC(c, o); } },
    { 'x', [](auto c, auto o) { okKanaC(c, o); } },
    { 'y', [](auto c, auto o) { okKanaC(c, o); } },
    { 'z', [](auto c, auto o) { okKanaC(c, o); } },
  };

  auto& KAlphaInputKeymap = m_keymaps[KeymapTypes::KAlphaInput];
  KAlphaInputKeymap.DefaultFunc = nulcmd;
  KAlphaInputKeymap.Keymap = {
    { CTRL_G, [self = this](auto c, auto) { kfCancel(self, c); } },
    { CTRL_H, kaBS },
    { ' ', [self = this](auto c, auto) { kkconv(self, c); } },
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
    fxthru(self, c);
  };
  SelectionKeymap.Keymap = {
    { CTRL_G, [self = this](auto c, auto) { cancelSel(self, c); } },
    { CTRL_T, [self = this](auto, auto) { fixIt(self); } },
    { ' ', [](auto, auto) { nxCand(); } },
    { 'x', [self = this](auto, auto) { pvCand(self); } },
    { '>', [self = this](auto c, auto) { stSuffix(self, c); } },
    { '?', [self = this](auto c, auto) { stSuffix(self, c); } },
    { '<', [self = this](auto c, auto) { stSuffix(self, c); } },
    { EXTRA_CODE, [self = this](auto c, auto) { thruFixItToAsc(self, c); } },
  };

  auto& CodeInputKeymap = m_keymaps[KeymapTypes::CodeInput];
  CodeInputKeymap.DefaultFunc = nulcmd;
  CodeInputKeymap.Keymap = {
    { CTRL_C, [self = this](auto, auto) { self->cancelCode(); } },
    { CTRL_G, [self = this](auto, auto) { self->cancelCode(); } },
    { CTRL_J, [self = this](auto, auto) { g_codeinput.enterCode(self); } },
    { CTRL_M, [self = this](auto, auto) { g_codeinput.enterCode(self); } },
    { CTRL_T, [self = this](auto, auto) { self->toKana(); } },
    { '0', [](auto c, auto) { g_codeinput.codein(c); } },
    { '1', [](auto c, auto) { g_codeinput.codein(c); } },
    { '2', [](auto c, auto) { g_codeinput.codein(c); } },
    { '3', [](auto c, auto) { g_codeinput.codein(c); } },
    { '4', [](auto c, auto) { g_codeinput.codein(c); } },
    { '5', [](auto c, auto) { g_codeinput.codein(c); } },
    { '6', [](auto c, auto) { g_codeinput.codein(c); } },
    { '7', [](auto c, auto) { g_codeinput.codein(c); } },
    { '8', [](auto c, auto) { g_codeinput.codein(c); } },
    { '9', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'A', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'B', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'C', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'D', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'E', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'F', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'a', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'b', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'c', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'd', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'e', [](auto c, auto) { g_codeinput.codein(c); } },
    { 'f', [](auto c, auto) { g_codeinput.codein(c); } },
  };
}
