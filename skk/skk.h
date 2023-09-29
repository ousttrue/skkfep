#pragma once
#include <stdint.h>
#include <string>

enum SkkModes
{
  SKK_MODE,
  KANA_MODE,
  ZENEI_MODE,
  KKANA_MODE,
  KINPUT_MODE,
  KSELECT_MODE,
};
inline const char*
mode_string(SkkModes m)
{
  switch (m) {
    case SKK_MODE:
      return "SKK";
    case KANA_MODE:
      return "かな";
    case ZENEI_MODE:
      return "全英";
    case KKANA_MODE:
      return "カナ";
    case KINPUT_MODE:
      return "単語入力";
    case KSELECT_MODE:
      return "単語選択";
  }
}

struct PreEdit
{
  // internal raw input
  std::string Input;
  // show user interface
  std::string Display;
};

struct SkkResult
{
  // write child pty
  std::string Output;
  // write stdout
  PreEdit Predit;
  SkkModes Mode;
};

class Skk
{
public:
  Skk();
  ~Skk();
  Skk(const Skk&) = delete;
  Skk& operator=(const Skk&) = delete;

  SkkResult input(uint8_t c);
};
