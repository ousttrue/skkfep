#pragma once
#include "keymap.h"

namespace skk {

struct InputMode
{
  InputType Type;
  std::string Statusline;

protected:
  InputMode(InputType type) {}

public:
  virtual ~InputMode() {}
  InputMode(const InputMode&) = delete;
  InputMode& operator=(const InputMode&) = delete;
  virtual Output putc(char8_t c) = 0;

  // for test
  Output puts(std::u8string_view s)
  {
    std::string confirmed;
    Output o;
    for (auto c : s) {
      o = putc(c);
      confirmed += o.Confirmed;
    }
    o.Confirmed = confirmed;
    return o;
  }
};

struct AsciiInput : InputMode
{
  AsciiInput();
  Output putc(char8_t c) override;
};

struct CodeInput : InputMode
{
  CodeInput();
  Output putc(char8_t c) override;
};

} // namespace
