/*
 *	INCLUDE FILES
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<fcntl.h>
#include	<signal.h>
#include	"skklib.h"


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
#include <sys/ioctl.h>
#include <sgtty.h>
#endif /* TERMIOS || TERMIO */

/*
 *	CONSTANTS
 */

#define	IB_SIZE		1024

#define	NIL		0
#define	T		1

#define INTERNALCODE	euc

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
#define	ERASECHAR	(d_ioval.c_cc[VERASE])
#define	KILLCHAR	(d_ioval.c_cc[VKILL])
#else
#define	ERASECHAR	(d_ioval.sg_erase)
#define	KILLCHAR	(d_ioval.sg_kill)
#endif

#define	ZENKAKU1(c,code)	((code)==euc?(((c)&0x80) == 0x80):\
				 (((c)&0xe0) == 0xa0 || (((c)&0xe0) == 0xc0)))

/*
 * Macros for select()
 */
#ifdef OLD_SELECT /* old select() */
typedef int FD_set;
#define FD_SET(fd,fdset) (*(fdset) |= 1<<(fd))
#define FD_CLR(fd,fdset) (*(fdset) &= ~(1<<(fd)))
#define FD_ISSET(fd,fdset) (*(fdset) & (1<<(fd)))
#define FD_ZERO(fdset) (*(fdset) = 0)
#else
typedef fd_set FD_set;
#endif /* old select() */

#ifdef INT_SIGNAL
typedef int SignalFunc;
#else
typedef void SignalFunc;
#endif

typedef void (*Keymap[128])();
typedef void (**KeymapPtr)();
typedef struct SparseKeymapBody {
	char key;
	void (*function)();
} SparseKeymapBody;

typedef struct SparseKeymap {
	void (*defaultfunc)();
	struct SparseKeymapBody *keymap;
} SparseKeymap;

typedef enum {
	NoStatusLine = 0,
	HaveStatusLine,
	UseBottomLine
} StatusType;

typedef enum { NoEsc=0,
	       SimpleEsc,
	       ViEsc,
	       EmacsEsc,
	       ToggleEsc
	   } EscapeBehavior;

/*
 *	GLOBAL VARIABLES
 */

#if defined TERMIOS || defined TERMIO
#ifdef TERMIOS
typedef struct termios TERMPARAM;
#else
typedef struct termio TERMPARAM;
#endif
#else
typedef struct {
    struct sgttyb  m_ttyb;     /* TIOCGETP */
    struct tchars  m_tchars;   /* TIOCGETC */
#ifdef TIOCLGET
    int            m_lmode;    /* TIOCLGET */
#endif /* TIOCLGET */
#ifdef TIOCGETD
    int            m_ldisc;    /* TIOCGETD */
#endif /* TIOCGETD */
#ifdef TIOCGLTC
    struct ltchars m_ltchars;  /* TIOCGLTC */
#endif /* TIOCGLTC */

#ifdef sony_news
# ifdef TIOCKGET
    int            km_con;     /* TIOCKGET */
# endif /* TIOCKGET */
# ifdef TIOCKGETC
    struct jtchars km_jtchars;  /* TIOCKGETC */
# endif /* TIOCKGETC */
#endif /* sony_news */
} TERMPARAM;
#endif /* not TERMIOS */

extern
int	tty,Shellfd;
extern
TERMPARAM	d_ioval;

extern
char	*T_ce,*T_kr,*T_kl,*T_cr,*T_bt,*T_ta,*T_sc,*T_rc,
	*T_so,*T_se,*T_us,*T_ue,*T_cl;

extern
FILE	*Shellin,*Shellout;

extern
SparseKeymap NormalKeymap,SelectionKeymap,CodeInputKeymap;

extern
SparseKeymap EscapedKeymap;
extern
SparseKeymapBody _ViEscKeymap[],_EmacsEscKeymap[];

extern
Keymap KanaKeymap,ZenkakuKeymap,KanjiInputKeymap,OkuriInputKeymap,
       KAlphaInputKeymap;

extern
char *modeString[];

extern
KeymapPtr CurrentKeymap;

extern
char UserDicName[];

extern
EscapeBehavior CurrentEscapeBehavior, LastEscapeBehavior;

extern
Dictionary UserDic;

#define MAX_CONSO 5
extern char LastConso[MAX_CONSO];  /* LastConso[0] is always '\0' */
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
# undef tolower
# undef toupper
# define _tolower(c)	((c)+'a'-'A')
# define _toupper(c)	((c)+'A'-'a')
# ifdef __GNUC__
#  define tolower(c)	({int _c=(c); isupper(_c) ? _tolower(_c) : _c;})
#  define toupper(c)	({int _c=(c); islower(_c) ? _toupper(_c) : _c;})
# else
#  define tolower(c)	(isupper(c) ? _tolower(c) : (c))
#  define toupper(c)	(islower(c) ? _toupper(c) : (c))
# endif
#endif
