#pragma once
#include "keymap.h"

struct KanjiCodeInput
{
  char codebuf[5];
  int codecol;
  void codeinMsg();
  skk::Result inputCode(struct Skk* skk);
  skk::Result codein(char c);
  // 入力確定
  skk::Result enterCode();
};
extern KanjiCodeInput g_codeinput;
