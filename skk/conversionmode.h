#pragma once
#include "inputmode.h"
#include "keymap.h"
#include <memory>

struct SkkConversionMode
{
  virtual ~SkkConversionMode() {}
  virtual SkkResult input(uint8_t c) = 0;
};

struct DirectMode : SkkConversionMode
{
  SkkInputMode InputMode = SkkInputMode::ascii();

  static std::shared_ptr<DirectMode> create()
  {
    return std::make_shared<DirectMode>();
  }
  SkkResult input(uint8_t c) override;
};
