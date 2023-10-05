#pragma once
#include "conversionmode.h"

namespace skk {

struct EntryMode : ConversionMode
{
  struct Dictionary* UserDic = nullptr;
  std::string WordBuf;
  std::string OkuriBuf;

  EntryMode()
    : ConversionMode(ConversionType::Entry, "漢字")
  {
  }
  Result input(uint8_t c) override;

  void open_dictionary(std::string_view UserDicName);
  void save_dictionary();
  void begin(std::string_view unconfirmed);

private:
  skk::Result kkconv();
};

} // namespace
