#include "connsh.h"
// #include "app.h"
// #include "config.h"
// #include "fep.h"
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
#include <unistd.h>
#include <wait.h>

#ifdef NO_VFORK
#define vfork fork
#endif

#define DEVICELEN 16

FILE *Shellout, *Shellin;

#if !defined(HAVE_BSD_OPENPTY) && !defined(IRIX)
#ifdef HPUX
char *MasterDevice = "/dev/ptym/pty%s", *SlaveDevice = "/dev/pty/tty%s",
     *deviceNo[] = {
       "p0", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "pa", "pb",
       "pc", "pd", "pe", "pf", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",
       "q8", "q9", "qa", "qb", "qc", "qd", "qe", "qf", "r0", "r1", "r2", "r3",
       "r4", "r5", "r6", "r7", "r8", "r9", "ra", "rb", "rc", "rd", "re", "rf",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "sa", "sb",
       "sc", "sd", "se", "sf", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "t8", "t9", "ta", "tb", "tc", "td", "te", "tf", "u0", "u1", "u2", "u3",
       "u4", "u5", "u6", "u7", "u8", "u9", "ua", "ub", "uc", "ud", "ue", "uf",
       "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "va", "vb",
       "vc", "vd", "ve", "vf", "w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7",
       "w8", "w9", "wa", "wb", "wc", "wd", "we", "wf", 0,
     };
#else          /* NOT HPUX */
#ifdef _IBMESA /* AIX/ESA */
char *MasterDevice = "/dev/ptyp%s", *SlaveDevice = "/dev/ttyp%s",
     *deviceNo[] = {
       "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b",
       "0c", "0d", "0e", "0f", "10", "11", "12", "13", "14", "15", "16", "17",
       "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
       "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
       "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b",
       "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
       "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53",
       "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
       "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
       "6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77",
       "78", "79", "7a", "7b", "7c", "7d", "7e", "7f", "80", "81", "82", "83",
       "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
       "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b",
       "9c", "9d", "9e", "9f", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
       "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
       "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
       "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb",
       "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
       "d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3",
       "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
       "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
       "fc", "fd", "fe", "ff",
     };
#else
const char* MasterDevice = "/dev/pty%s";
const char* SlaveDevice = "/dev/tty%s";
const char* deviceNo[] = {
  "p0", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "pa", "pb", "pc",
  "pd", "pe", "pf", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9",
  "qa", "qb", "qc", "qd", "qe", "qf", "r0", "r1", "r2", "r3", "r4", "r5", "r6",
  "r7", "r8", "r9", "ra", "rb", "rc", "rd", "re", "rf", 0,
};
#endif /* _IBMESA */
#endif /* HPUX */
#endif /* !defined(HAVE_BSD_OPENPTY) && !defined(IRIX) */

int Shellfd;
int ShellPID;

char SlaveName[DEVICELEN];

extern int errno;

#ifdef NO_SETEUID
#ifdef HAVE_SETREUID
#define seteuid(e) setreuid(-1, (e))
#else /* NOT HAVE_SETREUID */
#ifdef HAVE_SETRESUID
#define seteuid(e) setresuid(-1, (e), -1)
#else  /* NOT HAVE_SETRESUID */
/* I can't set euid. (;_;) */
#endif /* HAVE_SETRESUID */
#endif /* HAVE_SETREUID */
#endif /* NO_SETEUID */

bool
establishShell(const char* ShellName, char** ShellArg, void (*onSigChild)(int))
{
  static int i, master, slave, currentPid;
  static char procName[32];
  char* p;
  struct passwd* pwent;

  getDevice(&master, &slave);

  currentPid = getpid();

  set_winsize(master);
  setEnvirons();

  signal(SIGCHLD, onSigChild);

#ifdef APOLLO
  signal(SIGCLD, chld_exit);
#endif

  if ((i = vfork()) < 0) {
    fprintf(stderr, "fork error\n");
    return false;
  }

  if (i == 0) { /* child */
    currentPid = getpid();

    /*
     * Set free from process group and controlling terminal
     */
#ifdef TERMIOS
#define HAS_SETSID /* ? */
#endif
#ifdef HAS_SETSID
    (void)setsid();
#else /* HAS_SETSID */
#ifdef BSD
    if (setpgrp(0, currentPid) == -1)
      perror("setpgrp()");
    if ((i = open("/dev/tty", O_RDONLY)) < 0)
      perror("/dev/tty");
    else {
      if (ioctl(i, TIOCNOTTY, (char*)0))
        perror("ioctl(TIOCNOTTY)");
      close(i);
    }
#else /* BSD */
#ifdef SYSV
    if (setpgrp() == -1)
      perror("setpgrp()");
#endif /* SYSV */
#endif /* BSD */
#endif /* HAS_SETSID */

    /*
     * obtain new controlling terminal
     */
#if defined(BSD) && defined(TIOCSCTTY)
    close(master);
    (void)ioctl(slave, TIOCSCTTY, (char*)0);
    /* errors ignored for sun */
#else
    close(slave);
    slave = open(SlaveName, O_RDWR);
    if (slave < 0) {
      perror("open: pty slave");
      _exit(1);
    }
    close(master);
#endif
    set_tty_sane(slave);
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

/*
 * Use openpty(3) of 4.3BSD Reno and later,
 * or the same interface function.
 */
void
getDevice(int* master, int* slave)
{
  if (openpty(
        master, slave, SlaveName, (struct termios*)0, (struct winsize*)0) ==
      -1) {
    perror("openpty() failed");
    exit(1);
  }
}

void
freeDevice()
{
  chmod(SlaveName, 0666);
  chown(SlaveName, 0, 0);
}

void
writeShell1(char c)
{
  write(fileno(Shellout), &c, 1);
}

void
writeShells(const char* s)
{
  int l = strlen(s);
  write(fileno(Shellout), s, l);
}

void
writeTtyShell1(char c)
{
  write(fileno(Shellout), &c, 1);
}

void
writeTtyShells(const char* s)
{
  int l = strlen(s);
  write(fileno(Shellout), s, l);
}
