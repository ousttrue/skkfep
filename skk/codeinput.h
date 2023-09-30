#pragma once

struct KanjiCodeInput
{
  char codebuf[5];
  int codecol;
  void codeinMsg();
  void inputCode(class Skk* skk);
  void codein(char c);
  // 入力確定
  void enterCode(class Skk* skk);
};
extern KanjiCodeInput g_codeinput;
