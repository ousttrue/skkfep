#include "stty.h"
#include "app.h"
#include "config.h"
#include "connsh.h"
#include "etc.h"
#include "fep.h"
#include "keybind.h"
#include "skklib.h"
#include "terms.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined TERMIOS || defined TERMIO

#ifdef TERMIOS
#include <termios.h>
#else
#include <termio.h>
#endif /* TERMIOS */

#if defined(sun) || defined(SVR4)
#include <sys/filio.h>
#else
#include <sys/ioctl.h>
#endif

#ifdef SVR4
#include <sys/filio.h>
#endif

#ifdef HPUX
#include <bsdtty.h>
#endif /* HPUX */

#else /* not TERMIOS nor TERMIO */
#include <sgtty.h>
#include <sys/ioctl.h>
#endif /* TERMIOS || TERMIO */

#if defined TERMIOS || defined TERMIO
#ifdef TERMIOS
typedef struct termios TERMPARAM;
#else
typedef struct termio TERMPARAM;
#endif
#else
typedef struct
{
  struct sgttyb m_ttyb;   /* TIOCGETP */
  struct tchars m_tchars; /* TIOCGETC */
#ifdef TIOCLGET
  int m_lmode; /* TIOCLGET */
#endif /* TIOCLGET */
#ifdef TIOCGETD
  int m_ldisc; /* TIOCGETD */
#endif /* TIOCGETD */
#ifdef TIOCGLTC
  struct ltchars m_ltchars; /* TIOCGLTC */
#endif /* TIOCGLTC */

#ifdef sony_news
#ifdef TIOCKGET
  int km_con; /* TIOCKGET */
#endif /* TIOCKGET */
#ifdef TIOCKGETC
  struct jtchars km_jtchars; /* TIOCKGETC */
#endif /* TIOCKGETC */
#endif /* sony_news */
} TERMPARAM;
#endif /* not TERMIOS */

#if defined TERMIOS || defined TERMIO

#if !defined(VDISABLE)
#ifdef _POSIX_VDISABLE
#define VDISABLE _POSIX_VDISABLE
#else
#define VDISABLE 0377
#endif
#endif
extern TERMPARAM d_ioval;

#ifdef TERMIOS
#define GET_TTY(tty, ioval) tcgetattr(tty, ioval)
#define SET_TTY(tty, ioval) tcsetattr(tty, TCSANOW, ioval)
#else /* not TERMIOS */
#define GET_TTY(tty, ioval) ioctl(tty, TCGETA, ioval)
#define SET_TTY(tty, ioval) ioctl(tty, TCSETA, ioval)
#endif /* TERMIOS */

#else /* not TERMIOS nor TERMIO */

#define GET_TTY(tty, ioval) tiocget(tty, ioval)
#define SET_TTY(tty, ioval) tiocset(tty, ioval)

#endif

TERMPARAM d_ioval;
int tty;
#ifdef TIOCSWINSZ
struct winsize d_winsize;
#endif /* TIOCSWINSZ */

extern int ShellPID;
extern FILE* Shellout;

#if !defined(TERMIOS) && !defined(TERMIO)
static int
tiocget(fd, ioval)
int fd;
TERMPARAM* ioval;
{
  int er;
  er = ioctl(fd, TIOCGETP, &(ioval->m_ttyb));
  if (er == -1)
    return er;
  er = ioctl(fd, TIOCGETC, &(ioval->m_tchars));
  if (er == -1)
    return er;
#ifdef TIOCLGET
  er = ioctl(fd, TIOCLGET, &(ioval->m_lmode));
  if (er == -1)
    return er;
#endif /* TIOCLGET */
#ifdef TIOCGETD
  er = ioctl(fd, TIOCGETD, &(ioval->m_ldisc));
  if (er == -1)
    return er;
#endif /* TIOCGETD */
#ifdef TIOCGLTC
  er = ioctl(fd, TIOCGLTC, &(ioval->m_ltchars));
  if (er == -1)
    return er;
#endif /* TIOCGLTC */

#ifdef sony_news
#ifdef TIOCKGET
  er = ioctl(fd, TIOCKGET, &(ioval->km_con));
  if (er == -1)
    return er;
#endif /* TIOCKGET */
#ifdef TIOCKGETC
  er = ioctl(fd, TIOCKGETC, &(ioval->km_jtchars));
  if (er == -1)
    return er;
#endif /* TIOCKGETC */
#endif /* sony_news */
  return er;
}

static int
tiocset(fd, ioval)
int fd;
TERMPARAM* ioval;
{
  int er;
  er = ioctl(fd, TIOCSETP, &(ioval->m_ttyb));
  if (er == -1)
    return er;
  er = ioctl(fd, TIOCSETC, &(ioval->m_tchars));
#ifdef TIOCLSET
  er = ioctl(fd, TIOCLSET, &(ioval->m_lmode));
  if (er == -1)
    return er;
#endif /* TIOCLSET */
#ifdef TIOCGETD
  er = ioctl(fd, TIOCSETD, &(ioval->m_ldisc));
  if (er == -1)
    return er;
#endif /* TIOCGETD */
#ifdef TIOCGLTC
  er = ioctl(fd, TIOCSLTC, &(ioval->m_ltchars));
  if (er == -1)
    return er;
#endif /* TIOCGLTC */

#ifdef sony_news
#ifdef TIOCKGET
  er = ioctl(fd, TIOCKSET, &(ioval->km_con));
  if (er == -1)
    return er;
#endif /* TIOCKGET */
#ifdef TIOCKGETC
  er = ioctl(fd, TIOCKSETC, &(ioval->km_jtchars));
  if (er == -1)
    return er;
#endif /* TIOCKGETC */
#endif /* sony_news */
  return er;
}
#endif /* not TERMIOS */

void
set_winsize(int tty)
{
#ifdef TIOCSWINSZ
  int er;
  struct winsize s_winsize;

  s_winsize = d_winsize;
  if (msgLine() == (int)UseBottomLine) {
    s_winsize.ws_row--;
  }
  er = ioctl(tty, TIOCSWINSZ, &s_winsize);
  if (er == -1) {
    printf("Error occured\n");
  }
#endif /* TIOCSWINSZ */
}

void
get_winsize()
{
#ifdef TIOCSWINSZ
  extern int lines, columns;

  ioctl(tty, TIOCGWINSZ, &d_winsize);
  if (d_winsize.ws_col == 0) {
    d_winsize.ws_col = columns;
  } else {
    columns = d_winsize.ws_col;
  }
  if (d_winsize.ws_row == 0) {
    d_winsize.ws_row = lines;
  } else {
    lines = d_winsize.ws_row;
  }
  ioctl(tty, TIOCSWINSZ, &d_winsize);
#endif /* TIOCSWINSZ */
}

void
set_tty()
{
  int er;
  TERMPARAM ioval;

  GET_TTY(tty, &d_ioval);
  ioval = d_ioval;

#if defined TERMIOS || defined TERMIO
  ioval.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
#ifdef ISTRIP
  ioval.c_iflag &= ~ISTRIP; /* 8bit through */
#endif
  ioval.c_lflag &= ~(ISIG | ICANON | ECHO);
  ioval.c_oflag &= ~(OPOST);
  ioval.c_cc[VMIN] = 1;
  ioval.c_cc[VTIME] = 0;

  /* invalidate special control characters */
  ioval.c_cc[VINTR] = VDISABLE;
  ioval.c_cc[VSTART] = VDISABLE;
  ioval.c_cc[VSTOP] = VDISABLE;
  ioval.c_cc[VQUIT] = VDISABLE;
#ifdef VDISCARD
  ioval.c_cc[VDISCARD] = VDISABLE;
#endif
#ifdef VLNEXT
  ioval.c_cc[VLNEXT] = VDISABLE;
#endif
#ifdef VSUSP
  ioval.c_cc[VSUSP] = VDISABLE;
#endif
#ifdef VDSUSP
  ioval.c_cc[VDSUSP] = VDISABLE;
#endif
#else
  ioval.m_ttyb.sg_flags = RAW;
#endif

  er = SET_TTY(tty, &ioval);

  initFep();

  if (er == -1) {
    printf("Error occured\n");
    reset_tty();
    App::Instance().Exit(-1);
  }
}

void
reset_tty()
{
  reset_tty_without_close();
  close(tty);
}

void
reset_tty_without_close()
{
  termFep();
  SET_TTY(tty, &d_ioval);
}

void
set_tty_sane(int tty)
{
  int er;
  TERMPARAM ioval;

  ioval = d_ioval;
#if defined TERMIOS || defined TERMIO
#ifndef ECHOKE
#define ECHOKE 0
#endif
  ioval.c_lflag |= ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHOKE;
  ioval.c_iflag |= BRKINT | IGNPAR | ICRNL;
#ifdef ISTRIP
  ioval.c_iflag &= ~ISTRIP; /* 8bit through */
#endif
  ioval.c_oflag |= OPOST | ONLCR;
#else /* if not TERMIOS */
#ifdef BSD42
  ioval.m_ttyb.sg_flags |= ECHO | CRMOD | ANYP | CRTERA | CRTKIL;
#else
  ioval.m_ttyb.sg_flags |= ECHO | CRMOD | ANYP | CRTERA | CRTKIL | PASS8;
#endif /* BSD42 */
#ifdef TIOCLGET
#ifdef LPASS8
  ioval.m_lmode |= LPASS8;
#endif
#ifdef LPASS8OUT
  ioval.m_lmode |= LPASS8OUT;
#endif
#ifdef LEUCBKSP
  ioval.m_lmode |= LEUCBKSP;
#endif
#endif
#endif /* not TERMIOS */
  er = SET_TTY(tty, &ioval);
  if (er == -1) {
    printf("Error occured\n");
  }
}

void
tty_ready()
{
  tty = open(ttyname(0), O_RDWR);
}

void
reset_exit(int)
{
  reset_tty();
  signal(SIGCHLD, SIG_DFL);
  App::Instance().Exit(-1);
}

void
segv_exit(int)
{
  reset_tty();
  fprintf(stderr, "SEGMENTATION VIOLATION\n");
  App::Instance().Abort();
}

#ifdef HPUX
#define IF_STOPPED(x) WIFSTOPPED((x).w_stopval)
#else /* not HPUX */
#define IF_STOPPED(x) WIFSTOPPED(x)
#endif /* not HPUX */

void
chld_changed(int)
{
  int cpid;
/* #ifdef _AIX */
#if defined(_AIX) || defined(NECEWS) || defined(SOLARIS2)
  int statusp;
  reset_tty_without_close();
  cpid = wait(&statusp);
  if (cpid != -1 && IF_STOPPED(statusp)) { /* suspend */
    kill(0, SIGTSTP);
  } else
    Exit(0);

#else /* not AIX */
  int statusp;

  reset_tty_without_close();
#ifndef NO_SUSPEND
  cpid = wait((int*)&statusp);
  if (cpid != -1 && IF_STOPPED(statusp)) { /* suspend */
    kill(0, SIGTSTP);
  } else
#endif /* NO_SUSPEND */
    App::Instance().Exit(0);
#endif /* AIX */
}

void
iot_exit(int)
{
  reset_tty();
  fprintf(stderr, "Abort.\n");
  App::Instance().Abort();
}

void
hup_exit(int)
{
  reset_tty();
  fprintf(stderr, "Hungup\n");
  App::Instance().Exit(-1);
}

void
int_exit(int)
{
  reset_tty();
  fprintf(stderr, "Interrupt\n");
  App::Instance().Exit(-1);
}

void
pipe_exit(int)
{
  reset_tty();
  fprintf(stderr, "Pipe down\n");
  App::Instance().Exit(-1);
}

void
term_exit(int)
{
  reset_tty();
  fprintf(stderr, "Terminate\n");
  App::Instance().Exit(-1);
}

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
  set_tty();
  kill(ShellPID, SIGCONT);
}
#endif

#ifdef SIGWINCH
void
winchange(int)
{
  int tty;
  signal(SIGWINCH, SIG_IGN);
  if (Shellout != NULL)
    tty = fileno(Shellout);
  else
    tty = 1; /* stdout */
  get_winsize();
  set_winsize(tty);
  initFep();
  showcurmode();
  signal(SIGWINCH, winchange);
}
#endif

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
set_int()
{
  signal(SIGHUP, hup_exit);
  signal(SIGINT, sig_int);
  /*	signal(SIGQUIT,iot_exit); */
  signal(SIGILL, reset_exit);
  signal(SIGIOT, iot_exit);
  signal(SIGFPE, reset_exit);
#ifndef LINUX
  signal(SIGBUS, bus_exit);
#endif
  signal(SIGSEGV, segv_exit);
  signal(SIGPIPE, pipe_exit);
  signal(SIGTERM, term_exit);
#ifndef NO_SUSPEND
  signal(SIGTSTP, suspend);
  signal(SIGCONT, susp_cont);
#endif
#ifdef SIGWINCH
  signal(SIGWINCH, winchange);
#endif
  signal(SIGUSR1, sig_usr1);
  signal(SIGUSR2, sig_usr2);
}
