#include "skk.h"
#include "ctrlcode.h"
#include "dictinary.h"
#include "kkconv.h"
#include "romkan.h"
#include "terms.h"
#include <assert.h>
#include <sstream>
#include <stdio.h>

#define USER_DIC_NAME ".skk/SKK-JISYO.L"

SkkResult
nulcmd(char, bool)
{
  return {};
}

#define DEFAULT_KANAKEY "^j"

Skk::Skk()
{
  KanaKey = DEFAULT_KANAKEY;
  initialize();
  setKeymap(KeymapTypes::Normal);
  showmode(SKK_MODE);
}

Skk::~Skk() {}

void
Skk::open_dictionary(std::string_view path)
{
  if (path.empty()) {
    std::stringstream ss;
    ss << getenv("HOME") << "/" << USER_DIC_NAME;
    UserDicName = ss.str();
  }
  auto d = new Dictionary;
  if (d->load(UserDicName)) {
    UserDic = d;
  }
}

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
Skk::setKeymap(KeymapTypes t)
{
  setKeymap(&m_keymaps[t]);
  if (t == KeymapTypes::Kana) {
    if (romkan::isHiragana())
      showmode(KANA_MODE);
    else
      showmode(KKANA_MODE);
  }
}

void
Skk::restoreKeymap()
{
  CurrentKeymap = lastKeymap;
}

SkkOutput
Skk::input(uint8_t c, bool okuri)
{
  auto result = CurrentKeymap->input(c, okuri);
  apply(result);

  // process result
  if (result.ReInput) {
    result = CurrentKeymap->input(result.ReInput, result.Okuri);
    apply(result);
  }

  return result.Output;
}

// update Keymap etc...
void
Skk::apply(const SkkResult& result)
{
  if (result.NextMode) {
    showmode(*result.NextMode);
    setKeymap((*result.NextMode));
  }

  if (result.RestoreKeymap) {
    restoreKeymap();
  } else if (result.NextKeymap) {
    setKeymap(*result.NextKeymap);
  }
}
