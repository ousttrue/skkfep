#pragma once
#include <stdio.h>
#include <sys/select.h>

/*
 *	CONSTANTS
 */

#define IB_SIZE 1024

#define NIL 0
#define T 1

#define SKK_MODE 0
#define KANA_MODE 1
#define ZENEI_MODE 2
#define KKANA_MODE 3
#define KINPUT_MODE 4
#define KSELECT_MODE 5

/*
 *	MACROS
 */

#if defined TERMIOS || defined TERMIO
#define ERASECHAR (d_ioval.c_cc[VERASE])
#define KILLCHAR (d_ioval.c_cc[VKILL])
#else
#define ERASECHAR (d_ioval.sg_erase)
#define KILLCHAR (d_ioval.sg_kill)
#endif

#define ZENKAKU1(c, code)                                                      \
  ((code) == euc ? (((c) & 0x80) == 0x80)                                      \
                 : (((c) & 0xe0) == 0xa0 || (((c) & 0xe0) == 0xc0)))

/*
 * Macros for select()
 */
#ifdef OLD_SELECT /* old select() */
typedef int FD_set;
#define FD_SET(fd, fdset) (*(fdset) |= 1 << (fd))
#define FD_CLR(fd, fdset) (*(fdset) &= ~(1 << (fd)))
#define FD_ISSET(fd, fdset) (*(fdset) & (1 << (fd)))
#define FD_ZERO(fdset) (*(fdset) = 0)
#else
typedef fd_set FD_set;
#endif /* old select() */

#ifdef INT_SIGNAL
typedef int SignalFunc;
#else
typedef void SignalFunc;
#endif

/*
 *	GLOBAL VARIABLES
 */

extern int tty, Shellfd;

extern char *T_ce, *T_kr, *T_kl, *T_cr, *T_bt, *T_ta, *T_sc, *T_rc, *T_so,
  *T_se, *T_us, *T_ue, *T_cl;

#define MAX_CONSO 5
extern char LastConso[MAX_CONSO]; /* LastConso[0] is always '\0' */
extern short Nconso;
extern char SmallTU;
extern short Kindex;

extern short BlockTty;
extern char PreserveOnFailure;
extern char OkuriFirst;

#ifdef KANJIBS
extern char KanjiBS;
#endif

#ifdef OLD_CTYPE
#undef tolower
#undef toupper
#define _tolower(c) ((c) + 'a' - 'A')
#define _toupper(c) ((c) + 'A' - 'a')
#ifdef __GNUC__
#define tolower(c)                                                             \
  ({                                                                           \
    int _c = (c);                                                              \
    isupper(_c) ? _tolower(_c) : _c;                                           \
  })
#define toupper(c)                                                             \
  ({                                                                           \
    int _c = (c);                                                              \
    islower(_c) ? _toupper(_c) : _c;                                           \
  })
#else
#define tolower(c) (isupper(c) ? _tolower(c) : (c))
#define toupper(c) (islower(c) ? _toupper(c) : (c))
#endif
#endif

void
flushOut(int minchar);

void
nulcmd(char c);

void
thru(char c);

void
toAsc(char c);

void
toZenA(char c);

void
thruToAsc(char c);

void
toEsc(char c);

void
thruToEsc(char c);

void
thruBack(char c);

void
thru1(char c);
