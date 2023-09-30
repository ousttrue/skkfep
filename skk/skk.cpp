#include "skk.h"
// #include "connsh.h"
#include "ctrlcode.h"
#include "kkconv.h"
#include "romkan.h"
#include "terms.h"
#include <assert.h>
#include <stdio.h>

void
nulcmd(char, bool)
{
}

#define DEFAULT_KANAKEY "^j"

Skk::Skk()
{
  KanaKey = DEFAULT_KANAKEY;
  initialize([self = this](auto s) { self->m_buffer += s; });
  setKanaKey();
  toAsc();
}

Skk::~Skk() {}

void
Skk::showmode(SkkModes s)
{
  lastmode = curmode;
  showmessage(mode_string(s));
  curmode = s;
}

void
Skk::showcurmode()
{
  showmessage(mode_string(curmode));
}

void
Skk::showlastmode()
{
  showmessage(mode_string(lastmode));
  curmode = lastmode;
}

void
Skk::setKeymap(KeymapPtr _new)
{
  lastKeymap = CurrentKeymap;
  CurrentKeymap = _new;
}

void
Skk::restoreKeymap()
{
  CurrentKeymap = lastKeymap;
}

SkkResult
Skk::input(uint8_t c, bool okuri)
{
  auto res = CurrentKeymap->input(c, okuri);
  res.Output = m_buffer;
  m_buffer.clear();
  return res;
}

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
Skk::setKanaKey()
{
  auto k = code(KanaKey.c_str());
  printf("KanaKey=^%c\n", k ^ 0x40);
  NormalKeymap().Keymap[k] = [self = this](auto, auto) { self->toKana(); };
  SelectionKeymap().Keymap[k] = [self = this](auto, auto) { fixIt(self); };
  CodeInputKeymap().Keymap[k] = [self = this](auto, auto) { self->toKana(); };
  KanaKeymap().Keymap[k] = nulcmd;
  ZenkakuKeymap().Keymap[k] = [self = this](auto, auto) { self->toKana(); };
  KanjiInputKeymap().Keymap[k] = [self = this](auto c, auto) {
    kfFix(self, c);
  };
  OkuriInputKeymap().Keymap[k] = [self = this](auto c, auto) {
    okfFix(self, c);
  };
  KAlphaInputKeymap().Keymap[k] = [self = this](auto c, auto) {
    kfFix(self, c);
  };
}

bool
Skk::is_okuri_input()
{
  return CurrentKeymap == &OkuriInputKeymap();
}

void
Skk::toKana()
{
  setKeymap(KeymapTypes::Kana);
  toggleKana();
}

void
Skk::cancelCode()
{
  setKeymap(KeymapTypes::Kana);
  showmode(KANA_MODE);
}

void
Skk::toAsc()
{
  romkan::flushKana();
  setKeymap(KeymapTypes::Normal);
  showmode(SKK_MODE);
}

void
Skk::toZenA()
{
  romkan::flushKana();
  setKeymap(KeymapTypes::Zenkaku);
  showmode(ZENEI_MODE);
}

void
Skk::thruToAsc(char c)
{
  toAsc();
  thru(c);
}

void
Skk::thruBack(char c)
{
  thru(c);
  toKana();
}

void
Skk::thru1(char c)
{
  thru(c);
  restoreKeymap();
  showlastmode();
}

void
Skk::kkBeg()
{
  setKeymap(SkkModes::KINPUT_MODE);
  showmode(KINPUT_MODE);
  kanjiInputEffect(1);
  kkClearBuf();
}

void
Skk::toggleKana()
{
  if (romkan::isHiragana())
    showmode(KANA_MODE);
  else
    showmode(KKANA_MODE);
}
