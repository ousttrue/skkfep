#include "connsh.h"
#include "stty.h"
#include "terms.h"
#include <errno.h>
#include <fcntl.h>
#include <pty.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define DEVICELEN 16

namespace child {

FILE* Shellout;
FILE* Shellin;
int Shellfd;
int ShellPID;
char SlaveName[DEVICELEN];

bool
establishShell(const TermSize& size,
               const char* ShellName,
               char** ShellArg,
               void (*onSigChild)(int),
               const char* version)
{
  // pty
  int master, slave;
  struct winsize ws
  {
    .ws_row = (unsigned short)size.Rows, .ws_col = (unsigned short)size.Cols,
  };
  if (openpty(&master, &slave, SlaveName, (struct termios*)0, &ws) == -1) {
    perror("openpty() failed");
    return false;
  }

  // fork
  signal(SIGCHLD, onSigChild);
  auto i = vfork();
  if (i < 0) {
    fprintf(stderr, "fork error\n");
    return false;
  }

  static char procName[32];
  if (i == 0) {     /* child */
    char buf[1024]; //, *p, *q, *e, c1, c2;
    sprintf(buf, "%d", getpid());
    setenv("SKKFEP_PID", buf, 1);
    sprintf(buf, "%s", version);
    setenv("SKKFEP_VERSION", buf, 1);

    sprintf(buf, "%d", size.Cols);
    setenv("COLUMNS", buf, 1);
    sprintf(buf, "%d", size.Rows);
    setenv("LINES", buf, 1);

    //     /*
    //      * Set free from process group and controlling terminal
    //      */
    // #ifdef TERMIOS
    // #define HAS_SETSID /* ? */
    // #endif
    // #ifdef HAS_SETSID
    //     (void)setsid();
    // #else /* HAS_SETSID */
    // #ifdef BSD
    //     if (setpgrp(0, currentPid) == -1)
    //       perror("setpgrp()");
    //     if ((i = open("/dev/tty", O_RDONLY)) < 0)
    //       perror("/dev/tty");
    //     else {
    //       if (ioctl(i, TIOCNOTTY, (char*)0))
    //         perror("ioctl(TIOCNOTTY)");
    //       close(i);
    //     }
    // #else /* BSD */
    // #ifdef SYSV
    //     if (setpgrp() == -1)
    //       perror("setpgrp()");
    // #endif /* SYSV */
    // #endif /* BSD */
    // #endif /* HAS_SETSID */
    //
    //     /*
    //      * obtain new controlling terminal
    //      */
    // #if defined(BSD) && defined(TIOCSCTTY)
    //     close(master);
    //     (void)ioctl(slave, TIOCSCTTY, (char*)0);
    //     /* errors ignored for sun */
    // #else
    //     close(slave);
    //     slave = open(SlaveName, O_RDWR);
    //     if (slave < 0) {
    //       perror("open: pty slave");
    //       _exit(1);
    //     }
    //     close(master);
    // #endif
    terminal::set_tty_sane(slave);
    dup2(slave, 0);
    dup2(slave, 1);
    dup2(slave, 2);
    close(slave);

    seteuid(getuid());

    auto p = &ShellName[strlen(ShellName)];
    while (ShellName <= p && *p != '/')
      p--;
    if (*p == '/')
      p++;
    strcpy(procName, p);
    if (ShellArg == NULL) {
      execl(ShellName, procName, 0);
    } else {
      execvp(ShellName, ShellArg);
    }
    perror(ShellName);
    _exit(127);
  }

  close(slave);
  ShellPID = i;

  Shellout = fdopen(master, "w");
  Shellin = fdopen(master, "r");
  Shellfd = master;

  return true;
}

void
freeDevice()
{
  chmod(SlaveName, 0666);
  chown(SlaveName, 0, 0);
}

void
set_termsize(int tty, const TermSize& size)
{
  struct winsize ws_size
  {
    .ws_row = (unsigned short)size.Rows, .ws_col = (unsigned short)size.Cols,
  };
  int er = ioctl(tty, TIOCSWINSZ, &ws_size);
  if (er == -1) {
    printf("Error occured\n");
  }
}

void
thru(char c)
{
  writeTtyShell1(c);
}

} // namespace
