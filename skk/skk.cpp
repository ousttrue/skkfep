#include "skk.h"
#include "conversion/dictionary.h"
#include <assert.h>

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
namespace skk {

Skk::Skk(std::string_view _kanaKey)
{
  std::string kanaKey{ _kanaKey.begin(), _kanaKey.end() };
  auto k = code(kanaKey.c_str());
  // printf("KanaKey=^%c\n", k ^ 0x40);
  // NormalKeymap.Keymap[k] = [](auto, auto) {
  //   return SkkResult{
  //     .NextKeymap = KeymapTypes::Kana,
  //   };
  // };
  // SelectionKeymap.Keymap[k] = fixIt;
  // CodeInputKeymap.Keymap[k] = [](auto, auto) {
  //   return SkkResult{
  //     .NextKeymap = KeymapTypes::Kana,
  //   };
  // };
  // KanaKeymap.Keymap[k] = nulcmd;
  // ZenkakuKeymap.Keymap[k] = [](auto, auto) {
  //   return SkkResult{
  //     .NextKeymap = KeymapTypes::Kana,
  //   };
  // };
  // KanjiInputKeymap.Keymap[k] = kfFix;
  // OkuriInputKeymap.Keymap[k] = okfFix;
  // KAlphaInputKeymap.Keymap[k] = kfFix;

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
}

Skk::~Skk()
{
  m_SelectionMode->save_dictionary();
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
  m_SelectionMode->open_dictionary(UserDicName);
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
