#include "terms.h"
#include "statusline.h"
#include "stty.h"
#include "terminal.h"
#include "termsize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termcap.h>
#include <unistd.h>

Terminal g_term;

bool
getTCstr()
{
  return g_term.load(getenv("TERM"));
}

void
toMsg()
{
  if (ReverseStatus)
    standout(1);
  if (status::type() == StatusType::HaveStatusLine) {
    writes(g_term.ts.xy(0, 0));
  } else {
    writes(g_term.sc.seq);
    writes(g_term.cm.xy(0, current_termsize().Rows - 1));
  }
}

void
fromMsg()
{
  if (status::type() == StatusType::HaveStatusLine) {
    writes(g_term.fs.seq);
  } else {
    writes(g_term.rc.seq);
  }
  if (ReverseStatus)
    standout(0);
}

void
initFep()
{
  if (status::type() == StatusType::HaveStatusLine) {
    if (g_term.ss) {
      writes(g_term.ss.seq);
    }
  } else if (status::type() == StatusType::UseBottomLine) {
    writes("\r\n");
    writes(g_term.cs.xy(current_termsize().Rows - 2, 0));
    writes(g_term.cm.xy(0, current_termsize().Rows - 2));
  }
}

void
termFep()
{
  if (status::type() == StatusType::HaveStatusLine) {
    if (g_term.hs) {
      writes(g_term.hs.seq);
    }
  } else if (status::type() == StatusType::UseBottomLine) {
    writes(g_term.cs.xy(current_termsize().Rows - 1, 0));
    writes(g_term.cm.xy(0, current_termsize().Rows - 1));
  }
}

void
underline(int ctl)
{
  if (ctl)
    writes(g_term.us.seq);
  else
    writes(g_term.ue.seq);
}

void
standout(int ctl)
{
  if (ctl)
    writes(g_term.so.seq);
  else
    writes(g_term.se.seq);
}

void
saveCsr()
{
  writes(g_term.sc.seq);
}

void
restoreCsr()
{
  writes(g_term.rc.seq);
}

void
clearToEnd()
{
  writes(g_term.ce.seq);
}

void
csrLeft(int n)
{
  while (n) {
    writes(g_term.kl.seq);
    n--;
  }
}

void
csrRight(int n)
{
  while (n) {
    writes(g_term.kr.seq);
    n--;
  }
}

void
cls()
{
  writes(g_term.cl.seq);
}

void
erase(int n)
{
  while (n) {
    write1(' ');
    n--;
  }
}

void
rubout(int n)
{
  csrLeft(n);
  erase(n);
  csrLeft(n);
}

void
bell()
{
  write1('\007');
}

void
showmessage(const char* s)
{
  if (status::type() != StatusType::NoStatusLine) {
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
