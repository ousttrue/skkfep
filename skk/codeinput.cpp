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

SkkOutput
KanjiCodeInput::inputCode(Skk* skk)
{
  auto output = romkan::flushKana();
  if (status::type() != StatusType::NoStatusLine) {
    codeinMsg();
    skk->setKeymap(KeymapTypes::CodeInput);
    codecol = 0;
  }
  return { .Through = output };
}

SkkOutput
KanjiCodeInput::codein(char c)
{
  if (codecol == 4) {
    codecol = 0;
    codeinMsg();
  }
  SkkOutput output;
  output.Predit += c;
  codebuf[codecol] = tolower(c);
  codecol++;
  return output;
}

#define HEX1(x) ((x) > '9' ? ((x) - 'a' + 10) : ((x) - '0'))
#define HEX2INT(a, b) ((HEX1(a) << 4) + HEX1(b))

void
KanjiCodeInput::enterCode(Skk* skk)
{
  fromMsg();
  if (codecol == 4) {
    char kbuf[3];
    kbuf[0] = (HEX2INT(codebuf[0], codebuf[1]) | 0x80);
    kbuf[1] = (HEX2INT(codebuf[2], codebuf[3]) | 0x80);
    kbuf[2] = '\0';
    child::writeShells(kbuf);
  }
  skk->cancelCode();
}

KanjiCodeInput g_codeinput;
