#include "skk.h"
#include "dictinary.h"
#include "kkconv.h"
#include "romkan.h"
// #include "terms.h"
#include <assert.h>
#include <sstream>

#define USER_DIC_NAME ".skk/SKK-JISYO.L"

#define DEFAULT_KANAKEY "^j"

Skk::
Skk()
{
  KanaKey = DEFAULT_KANAKEY;
  initialize();
  setKeymap(KeymapTypes::Normal);
}

Skk::~
Skk()
{
}

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

SkkOutput
Skk::input(uint8_t c, bool okuri)
{
  SkkResult result;
  if (false) {

    result = ConversinMode->input(c);

  } else {
    // 旧
    result = CurrentKeymap->input(c, okuri);
  }

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
    setKeymap((*result.NextMode));
  }

  if (result.RestoreKeymap) {
    restoreKeymap();
  } else if (result.NextKeymap) {
    setKeymap(*result.NextKeymap);
  }
}

std::string
Skk::statusline() const
{
  switch (CurrentKeymap->Type) {
    case KeymapTypes::Normal:
      return "SKK";
    case KeymapTypes::Kana:
      if (romkan::isHiragana())
        return "かな";
      else
        return "カナ";
    case KeymapTypes::Zenkaku:
      return "全英";
    case KeymapTypes::KanjiInput:
      return "単語入力";
    case KeymapTypes::OkuriInput:
      return "送り";
    case KeymapTypes::Selection:
      return "単語選択";
    case KeymapTypes::KAlphaInput:
      return "ascii";
    case KeymapTypes::CodeInput:
      return "code";
  }

  assert(false);
  return "";
}
