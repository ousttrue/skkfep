#pragma once
#include "keymap.h"

namespace skk {

using InputFunc = std::function<Output(uint8_t)>;

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
  virtual Output input(uint8_t c) = 0;
};

struct AsciiInput : InputMode
{
  AsciiInput();
  Output input(uint8_t c) override;
};

struct CodeInput : InputMode
{
  CodeInput();
  Output input(uint8_t c) override;
};

} // namespace
