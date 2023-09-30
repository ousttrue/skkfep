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
  writes("JIS or EUC code: ");
}

void
KanjiCodeInput::inputCode()
{
  romkan::flushKana();
  if (status::type() == StatusType::NoStatusLine)
    return;
  codeinMsg();
  g_skk.setKeymap(KeymapTypes::CodeInput);
  codecol = 0;
}

void
KanjiCodeInput::codein(char c)
{
  if (codecol == 4) {
    codecol = 0;
    codeinMsg();
  }
  write1(c);
  codebuf[codecol] = tolower(c);
  codecol++;
}

#define HEX1(x) ((x) > '9' ? ((x) - 'a' + 10) : ((x) - '0'))
#define HEX2INT(a, b) ((HEX1(a) << 4) + HEX1(b))

void
KanjiCodeInput::enterCode()
{
  fromMsg();
  if (codecol == 4) {
    char kbuf[3];
    kbuf[0] = (HEX2INT(codebuf[0], codebuf[1]) | 0x80);
    kbuf[1] = (HEX2INT(codebuf[2], codebuf[3]) | 0x80);
    kbuf[2] = '\0';
    writeShells(kbuf);
  }
  g_skk.cancelCode();
}

KanjiCodeInput g_codeinput;
