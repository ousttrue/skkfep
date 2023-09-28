#include "etc.h"
#include "stty.h"
#include "terms.h"
#include <sys/time.h>
#include <unistd.h>

static SkkModes lastmode = {};
static SkkModes curmode = {};

void
sleep_ms(int n)
{
  usleep(n * 1000);
}

void
showmode(SkkModes s)
{
  lastmode = curmode;
  showmessage(mode_string(s));
  curmode = s;
}

void
showcurmode()
{
  showmessage(mode_string(curmode));
}

void
showlastmode()
{
  showmessage(mode_string(lastmode));
  curmode = lastmode;
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
