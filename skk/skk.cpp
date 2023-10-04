#include "skk.h"
#include "conversion/dictionary.h"
#include "conversion/kkconv.h"
#include <assert.h>

namespace skk {

Skk::
Skk(std::string_view kanaKey)
{
  m_AsciiInput = std::make_shared<AsciiInput>();
  m_ZenkakuInput = std::make_shared<ZenkakuInput>();
  m_KanaInput = std::make_shared<KanaInput>();
  m_CodeInput = std::make_shared<CodeInput>();

  m_DirectMode = std::make_shared<DirectMode>();
  m_EntryMode = std::make_shared<EntryMode>();
  m_OkuriMode = std::make_shared<OkuriMode>();
  m_SelectionMode = std::make_shared<SelectionMode>();

  CurrentMode = m_DirectMode;
  CurrentMode->InputMode = m_AsciiInput;

  initialize(kanaKey);
}

Skk::~
Skk()
{
}

void
Skk::open_dictionary(std::string_view path)
{
  if (path.empty()) {
    return;
  }
  std::string UserDicName;
  for (auto c : path) {
    if (c == '~') {
      // expand
      UserDicName += getenv("HOME");
    } else {
      UserDicName += c;
    }
  }
  auto d = new Dictionary;
  if (d->load(UserDicName)) {
    UserDic = d;
  }
}

Output
Skk::input(uint8_t c)
{
  auto result = CurrentMode->input(c);
  apply(result);

  // process result
  if (result.ReInput) {
    result = CurrentMode->input(result.ReInput);
    apply(result);
  }

  return result.Output;
}

// update Keymap etc...
void
Skk::apply(const Result& result)
{
  if (result.NextConversinMode) {
    switch (*result.NextConversinMode) {
      case ConversionType::Direct:
        CurrentMode = m_DirectMode;
        break;

      case ConversionType::Entry:
        CurrentMode = m_EntryMode;
        break;

      case ConversionType::Okuri:
        CurrentMode = m_OkuriMode;
        break;

      case ConversionType::Selection:
        CurrentMode = m_SelectionMode;
        break;
    }
  }

  if (result.NextInputMode) {
    switch (*result.NextInputMode) {
      case InputType::Ascii:
        CurrentMode->InputMode = m_AsciiInput;
        break;

      case InputType::Kana:
        CurrentMode->InputMode = m_KanaInput;
        break;

      case InputType::Zenkaku:
        CurrentMode->InputMode = m_ZenkakuInput;
        break;

      case InputType::Code:
        CurrentMode->InputMode = m_CodeInput;
        break;
    }
  }
}

} // namespace
