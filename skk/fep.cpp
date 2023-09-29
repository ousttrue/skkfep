#include "fep.h"
#include "app.h"
#include "connsh.h"
#include "etc.h"
#include "keybind.h"
#include "romkan.h"
#include "statusline.h"
#include "stty.h"
#include "terms.h"
#include "termsize.h"
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define FLUSH_TIMEOUT 300000 /* timeout ( micro-sec ) */

KeymapPtr CurrentKeymap;

extern char* version;
extern char ShellName[];
extern char** ShellArg;

#ifndef NO_SUSPEND
void
suspend(int)
{
  reset_tty_without_close();
  signal(SIGTSTP, SIG_DFL);
  kill(0, SIGTSTP);
}

void
susp_cont(int)
{
  signal(SIGTSTP, suspend);
  reset_tty_without_close();
  if (!set_tty()) {
    App::Instance().Exit(-1);
  }
  kill(ShellPID, SIGCONT);
}
#endif

void
winchange(int)
{
  signal(SIGWINCH, SIG_IGN);
  auto size = get_termsize();
  if (status::type() == StatusType::UseBottomLine) {
    --size.Rows;
  }

  int tty;
  if (Shellout != NULL)
    tty = fileno(Shellout);
  else
    tty = 1; /* stdout */
  set_termsize(tty, size);
  initFep();
  showcurmode();
  signal(SIGWINCH, winchange);
}

void
sig_usr1(int)
{
  signal(SIGUSR1, SIG_IGN);
  initFep();
  toggleEscape(ViEsc);
  signal(SIGUSR1, sig_usr1);
}

void
sig_usr2(int)
{
  signal(SIGUSR2, SIG_IGN);
  initFep();
  toggleEscape(EmacsEsc);
  signal(SIGUSR2, sig_usr2);
}

void
sig_int(int)
{
  signal(SIGHUP, SIG_IGN);
  initFep();
  toggleEscape(NoEsc);
  signal(SIGHUP, sig_int);
}

void
init_signals()
{
  signal(SIGHUP, [](int) { App::Instance().Reset("Hungup"); });
  signal(SIGINT, sig_int);
  /*	signal(SIGQUIT,iot_exit); */
  signal(SIGILL, [](int) {
    reset_tty();
    signal(SIGCHLD, SIG_DFL);
    App::Instance().Exit(-1);
  });
  signal(SIGIOT, [](int) {
    reset_tty();
    fprintf(stderr, "Abort.\n");
    App::Instance().Abort();
  });
  signal(SIGFPE, [](int) {
    reset_tty();
    signal(SIGCHLD, SIG_DFL);
    App::Instance().Exit(-1);
  });
  signal(SIGSEGV, [](int) {
    reset_tty();
    fprintf(stderr, "SEGMENTATION VIOLATION\n");
    App::Instance().Abort();
  });
  signal(SIGPIPE, [](int) { App::Instance().Reset("Pipe down"); });
  signal(SIGTERM, [](int) { App::Instance().Reset("Terminate"); });
#ifndef NO_SUSPEND
  signal(SIGTSTP, suspend);
  signal(SIGCONT, susp_cont);
#endif
  signal(SIGWINCH, winchange);
  signal(SIGUSR1, sig_usr1);
  signal(SIGUSR2, sig_usr2);
}

void
flushOut(int minchar)
{
  int i, nchar, ntimeout;
  fd_set readfds;
  struct timeval timeout;

  FD_ZERO(&readfds);
  FD_SET(Shellfd, &readfds);
  timeout.tv_sec = FLUSH_TIMEOUT / 1000000;
  timeout.tv_usec = FLUSH_TIMEOUT % 1000000;
  ntimeout = 0;
  nchar = 0;

  char shellBuf[256];
  while (ntimeout < minchar) {
    i = select(Shellfd + 1, &readfds, NULL, NULL, &timeout);
    if (i == -1 && errno == EINTR)
      continue;
    if (i > 0) {
      i = read(Shellfd, shellBuf, std::size(shellBuf));
      writeShTty(shellBuf, i);
      nchar += i;
      if (nchar >= minchar)
        break;
    } else
      ntimeout++;
  }
}

void
nulcmd(char c)
{
}

void
thru(char c)
{
  writeTtyShell1(c);
}

void
toAsc(char c)
{
  romkan::flushKana();
  setKeymap(&CurrentKeymap, convertKeymap(&NormalKeymap));
  showmode(SKK_MODE);
}

void
toZenA(char c)
{
  romkan::flushKana();
  setKeymap(&CurrentKeymap, ZenkakuKeymap);
  showmode(ZENEI_MODE);
}

void
thruToAsc(char c)
{
  toAsc(c);
  thru(c);
}

void
toEsc(char c)
{
  romkan::flushKana();
  setKeymap(&CurrentKeymap, convertKeymap(&EscapedKeymap));
  showmode(SKK_MODE);
}

void
thruToEsc(char c)
{
  toEsc(c);
  thru(c);
}

void
thruBack(char c)
{
  thru(c);
  romkan::toKana(c);
}

void
thru1(char c)
{
  thru(c);
  restoreKeymap(&CurrentKeymap);
  showlastmode();
}
