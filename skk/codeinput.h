#pragma once

struct KanjiCodeInput
{
  char codebuf[5];
  int codecol;
  void codeinMsg();
  void inputCode();
  void codein(char c);
  // 入力確定
  void enterCode();
};
extern KanjiCodeInput g_codeinput;
