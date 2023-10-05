#pragma once
#include "keymap.h"

namespace skk {

struct InputMode
{
  InputType Type;
  std::string Statusline;

protected:
  InputMode(InputType type, std::string_view statusline)
    : Type(type)
    , Statusline(statusline)
  {
  }

public:
  virtual ~InputMode() {}
  InputMode(const InputMode&) = delete;
  InputMode& operator=(const InputMode&) = delete;
  virtual Result putc(char8_t c) = 0;

  // for test
  Result puts(std::u8string_view s)
  {
    std::string confirmed;
    Result res;
    for (auto c : s) {
      res = putc(c);
      confirmed += res.Output.Confirmed;
    }
    res.Output.Confirmed = confirmed;
    return res;
  }
};

struct AsciiInput : InputMode
{
  AsciiInput();
  Result putc(char8_t c) override;
};

struct CodeInput : InputMode
{
  CodeInput();
  Result putc(char8_t c) override;
};

} // namespace
