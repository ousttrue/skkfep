#include "codeinput.h"
#include "connsh.h"
#include "romkan.h"
#include "skk.h"
#include "statusline.h"
#include "stty.h"
#include "terms.h"

void
KanjiCodeInput::codeinMsg()
{
  toMsg();
  terminal::writes("JIS or EUC code: ");
}

skk::Result
KanjiCodeInput::inputCode(Skk* skk)
{
  skk::Result result;
  // result.Output.Confirmed = romkan::flushKana();
  if (status::type() != StatusType::NoStatusLine) {
    codeinMsg();
    result.NextInputMode = skk::InputType::Code;
    codecol = 0;
  }
  return result;
}

skk::Result
KanjiCodeInput::codein(char c)
{
  if (codecol == 4) {
    codecol = 0;
    codeinMsg();
  }
  skk::Result result;
  result.Output.Unconfirmed += c;
  codebuf[codecol] = tolower(c);
  codecol++;
  return result;
}

#define HEX1(x) ((x) > '9' ? ((x) - 'a' + 10) : ((x) - '0'))
#define HEX2INT(a, b) ((HEX1(a) << 4) + HEX1(b))

// 確定。かな入力に戻る
skk::Result
KanjiCodeInput::enterCode()
{
  fromMsg();
  skk::Result result;
  if (codecol == 4) {
    char kbuf[3];
    kbuf[0] = (HEX2INT(codebuf[0], codebuf[1]) | 0x80);
    kbuf[1] = (HEX2INT(codebuf[2], codebuf[3]) | 0x80);
    kbuf[2] = '\0';
    result.Output.Confirmed += kbuf;
  }
  result.NextInputMode = skk::InputType::Kana;
  return result;
}

KanjiCodeInput g_codeinput;
