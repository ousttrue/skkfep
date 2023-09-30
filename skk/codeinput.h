#pragma once
#include "keymap.h"

struct KanjiCodeInput
{
  char codebuf[5];
  int codecol;
  void codeinMsg();
  SkkOutput inputCode(class Skk* skk);
  SkkOutput codein(char c);
  // 入力確定
  void enterCode(class Skk* skk);
};
extern KanjiCodeInput g_codeinput;
