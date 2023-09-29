#include "stty.h"
#include "statusline.h"

#define LINUX
#define SYSV
#define TERMIOS
#define HAVE_SETENV
#define NO_VFORK
#define HAVE_SETREUID

#include "terms.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  struct sgttyb m_ttyb;      /* TIOCGETP */
  struct tchars m_tchars;    /* TIOCGETC */
#ifdef TIOCLGET
  int m_lmode;               /* TIOCLGET */
#endif /* TIOCLGET */
#ifdef TIOCGETD
  int m_ldisc;               /* TIOCGETD */
#endif /* TIOCGETD */
#ifdef TIOCGLTC
  struct ltchars m_ltchars;  /* TIOCGLTC */
#endif /* TIOCGLTC */

#ifdef sony_news
#ifdef TIOCKGET
  int km_con;                /* TIOCKGET */
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
int g_term_tty;
#ifdef TIOCSWINSZ
struct winsize d_winsize;
#endif /* TIOCSWINSZ */

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

bool
set_tty()
{
  GET_TTY(g_term_tty, &d_ioval);
  auto ioval = d_ioval;

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

  int er = SET_TTY(g_term_tty, &ioval);
  if (er == -1) {
    printf("Error occured\n");
    reset_tty();
    return false;
  }
  return true;
}

void
reset_tty()
{
  reset_tty_without_close();
  close(g_term_tty);
}

void
reset_tty_without_close()
{
  termFep();
  SET_TTY(g_term_tty, &d_ioval);
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
  g_term_tty = open(ttyname(0), O_RDWR);
}

void
writes(std::string_view s)
{
  write(g_term_tty, s.data(), s.size());
}

void
write1(char c)
{
  write(g_term_tty, &c, 1);
}

void
writeShTty(const char* s, int l)
{
  write(g_term_tty, s, l);
}

void
writeTty(const char* s, int l)
{
  write(g_term_tty, s, l);
}
