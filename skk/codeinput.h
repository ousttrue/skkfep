#pragma once
#include "keymap.h"

struct KanjiCodeInput
{
  char codebuf[5];
  int codecol;
  void codeinMsg();
  SkkResult inputCode(class Skk* skk);
  SkkResult codein(char c);
  // 入力確定
  SkkResult enterCode();
};
extern KanjiCodeInput g_codeinput;
