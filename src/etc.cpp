#include "etc.h"
#include "config.h"
#include "fep.h"
#include "readwrite.h"
#include "terms.h"
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

static const char* lastmodestr;
static const char* curmodestr;

void
sleep_ms(int n)
{
  usleep(n * 1000);
}

const char* modeString[] = {
  "SKK",
  "\244\253\244\312",
  "\301\264\261\321",
  "\245\253\245\312",
  "\303\261\270\354\306\376\316\317",
  "\303\261\270\354\301\252\302\362",
};

void
showmode(int s)
{
  lastmodestr = curmodestr;
  showmessage(modeString[s]);
  curmodestr = modeString[s];
}

void
showcurmode()
{
  showmessage(curmodestr);
}

void
showlastmode()
{
  showmessage(lastmodestr);
  curmodestr = lastmodestr;
}

void
showmessage(const char* s)
{
  if (msgLine()) {
    toMsg();
    writes(s);
    clearToEnd();
    fromMsg();
  }
}

void
kanjiInputEffect(int on)
{
  underline(on);
}

void
kanjiSelectionEffect(int on)
{
  standout(on);
}
