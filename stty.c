#include "config.h"
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include "fep.h"
#include <sys/types.h>
#include <sys/wait.h>

#if defined TERMIOS || defined TERMIO

#if !defined(VDISABLE)
# ifdef _POSIX_VDISABLE
#	define VDISABLE _POSIX_VDISABLE
# else
#	define VDISABLE 0377
# endif
#endif

#ifdef TERMIOS
#define GET_TTY(tty,ioval)	tcgetattr(tty,ioval)
#define SET_TTY(tty,ioval)	tcsetattr(tty,TCSANOW,ioval)
#else /* not TERMIOS */
#define GET_TTY(tty,ioval)      ioctl(tty, TCGETA, ioval)
#define SET_TTY(tty,ioval)      ioctl(tty, TCSETA, ioval)
#endif /* TERMIOS */

#else /* not TERMIOS nor TERMIO */

#define GET_TTY(tty,ioval)	tiocget(tty, ioval)
#define SET_TTY(tty,ioval)	tiocset(tty, ioval)

#endif

TERMPARAM	d_ioval;
int		tty;
#ifdef TIOCSWINSZ
struct winsize  d_winsize;
#endif /* TIOCSWINSZ */

extern
int	ShellPID;
extern
FILE   *Shellout;

#if !defined(TERMIOS) && !defined(TERMIO)
static int
tiocget(fd, ioval)
    int fd;
    TERMPARAM *ioval;
{
    int er;
    er = ioctl(fd, TIOCGETP, &(ioval->m_ttyb));
    if ( er == -1 ) return er;
    er = ioctl(fd, TIOCGETC, &(ioval->m_tchars));
    if ( er == -1 ) return er;
#ifdef TIOCLGET
    er = ioctl(fd, TIOCLGET, &(ioval->m_lmode));
    if ( er == -1 ) return er;
#endif /* TIOCLGET */
#ifdef TIOCGETD
    er = ioctl(fd, TIOCGETD, &(ioval->m_ldisc));
    if ( er == -1 ) return er;
#endif /* TIOCGETD */
#ifdef TIOCGLTC
    er = ioctl(fd, TIOCGLTC, &(ioval->m_ltchars));
    if ( er == -1 ) return er;
#endif /* TIOCGLTC */

#ifdef sony_news
# ifdef TIOCKGET
    er = ioctl(fd, TIOCKGET, &(ioval->km_con));
    if ( er == -1 ) return er;
# endif /* TIOCKGET */
# ifdef TIOCKGETC
    er = ioctl(fd, TIOCKGETC, &(ioval->km_jtchars));
    if ( er == -1 ) return er;
# endif /* TIOCKGETC */
#endif /* sony_news */
    return er;
}

static int
tiocset(fd, ioval)
    int fd;
    TERMPARAM *ioval;
{
    int er;
    er = ioctl(fd, TIOCSETP, &(ioval->m_ttyb));
    if ( er == -1 ) return er;
    er = ioctl(fd, TIOCSETC, &(ioval->m_tchars));
#ifdef TIOCLSET
    er = ioctl(fd, TIOCLSET, &(ioval->m_lmode));
    if ( er == -1 ) return er;
#endif /* TIOCLSET */
#ifdef TIOCGETD
    er = ioctl(fd, TIOCSETD, &(ioval->m_ldisc));
    if ( er == -1 ) return er;
#endif /* TIOCGETD */
#ifdef TIOCGLTC
    er = ioctl(fd, TIOCSLTC, &(ioval->m_ltchars));
    if ( er == -1 ) return er;
#endif /* TIOCGLTC */

#ifdef sony_news
# ifdef TIOCKGET
    er = ioctl(fd, TIOCKSET, &(ioval->km_con));
    if ( er == -1 ) return er;
# endif /* TIOCKGET */
# ifdef TIOCKGETC
    er = ioctl(fd, TIOCKSETC, &(ioval->km_jtchars));
    if ( er == -1 ) return er;
# endif /* TIOCKGETC */
#endif /* sony_news */
    return er;
}
#endif /* not TERMIOS */

set_winsize(tty)
	int tty;
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

set_tty()
{	int	er;
	TERMPARAM ioval;

	GET_TTY(tty, &d_ioval);
	ioval = d_ioval;

#if defined TERMIOS || defined TERMIO
	ioval.c_iflag &= ~(INLCR|IGNCR|ICRNL|IXON|IXOFF);
# ifdef ISTRIP
	ioval.c_iflag &= ~ISTRIP;	/* 8bit through */
# endif
	ioval.c_lflag &= ~(ISIG|ICANON|ECHO);
	ioval.c_oflag &= ~(OPOST);
	ioval.c_cc[VMIN] = 1;
	ioval.c_cc[VTIME] = 0;

	/* invalidate special control characters */
	ioval.c_cc[VINTR]   = VDISABLE;
	ioval.c_cc[VSTART]  = VDISABLE;
	ioval.c_cc[VSTOP]   = VDISABLE;
	ioval.c_cc[VQUIT]   = VDISABLE;
# ifdef VDISCARD
	ioval.c_cc[VDISCARD]= VDISABLE;
# endif
# ifdef VLNEXT
	ioval.c_cc[VLNEXT]  = VDISABLE;
# endif
# ifdef VSUSP
	ioval.c_cc[VSUSP]   = VDISABLE;
# endif
# ifdef VDSUSP
	ioval.c_cc[VDSUSP]  = VDISABLE;
# endif
#else
	ioval.m_ttyb.sg_flags = RAW;
#endif

	er = SET_TTY(tty, &ioval);

	initFep();

	if (er == -1) {
		printf("Error occured\n");
		reset_tty();
		Exit(-1);
	}
}

reset_tty()
{
	reset_tty_without_close();
	close(tty);
}

reset_tty_without_close()
{
	termFep();
	SET_TTY(tty, &d_ioval);
}

set_tty_sane(tty)
int tty;
{	int	er;
	TERMPARAM	ioval;

	ioval = d_ioval;
#if defined TERMIOS || defined TERMIO
#ifndef ECHOKE
# define ECHOKE 0
#endif
	ioval.c_lflag |= ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHOKE;
	ioval.c_iflag |= BRKINT | IGNPAR | ICRNL;
# ifdef ISTRIP
	ioval.c_iflag &= ~ISTRIP;	/* 8bit through */
# endif
	ioval.c_oflag |= OPOST | ONLCR;
#else /* if not TERMIOS */
#ifdef BSD42
	ioval.m_ttyb.sg_flags |= ECHO | CRMOD | ANYP | CRTERA | CRTKIL;
#else
	ioval.m_ttyb.sg_flags |= ECHO | CRMOD | ANYP | CRTERA | CRTKIL | PASS8;
#endif /* BSD42 */
# ifdef TIOCLGET
#  ifdef LPASS8
	ioval.m_lmode |= LPASS8;
#  endif
#  ifdef LPASS8OUT
	ioval.m_lmode |= LPASS8OUT;
#  endif
#  ifdef LEUCBKSP
	ioval.m_lmode |= LEUCBKSP;
#  endif
# endif
#endif /* not TERMIOS */
	er = SET_TTY(tty, &ioval);
	if (er == -1) {
		printf("Error occured\n");
	}
}

tty_ready()
{	char	*ttyname();

	tty = open(ttyname(0),O_RDWR);
}

SignalFunc
reset_exit()
{
	reset_tty();
	signal(SIGCHLD,SIG_DFL);
	Exit(-1);
}

#ifndef LINUX
SignalFunc
bus_exit()
{
	reset_tty();
	fprintf(stderr,"BUS ERROR\n");
	Abort();
}
#endif

SignalFunc
segv_exit()
{
	reset_tty();
	fprintf(stderr,"SEGMENTATION VIOLATION\n");
	Abort();
}

#ifdef APOLLO
SignalFunc
chld_exit()
{
	reset_tty();
	Exit(0);
}
#endif

#ifdef HPUX
#define IF_STOPPED(x)	WIFSTOPPED((x).w_stopval)
#else /* not HPUX */
#define IF_STOPPED(x)	WIFSTOPPED(x)
#endif /* not HPUX */

SignalFunc
chld_changed()
{
	int cpid;
/* #ifdef _AIX */
#if defined(_AIX) || defined(NECEWS) || defined(SOLARIS2)
	int statusp;
	reset_tty_without_close();
	cpid = wait(&statusp);
	if (cpid != -1 && IF_STOPPED(statusp)) { /* suspend */
		kill(0,SIGTSTP);
	}
	else 
		Exit(0);

#else /* not AIX */
	union wait statusp;

	reset_tty_without_close();
#ifndef NO_SUSPEND
	cpid = wait((int *)&statusp);
	if (cpid != -1 && IF_STOPPED(statusp)) { /* suspend */
		kill(0,SIGTSTP);
	}
	else 
#endif /* NO_SUSPEND */
		Exit(0);
#endif /* AIX */
}

SignalFunc
iot_exit()
{
	reset_tty();
	fprintf(stderr,"Abort.\n");
	Abort();
}

SignalFunc
hup_exit()
{
	reset_tty();
	fprintf(stderr,"Hungup\n");
	Exit(-1);
}

SignalFunc
int_exit()
{
	reset_tty();
	fprintf(stderr,"Interrupt\n");
	Exit(-1);
}

SignalFunc
pipe_exit()
{
	reset_tty();
	fprintf(stderr,"Pipe down\n");
	Exit(-1);
}

SignalFunc
term_exit()
{
	reset_tty();
	fprintf(stderr,"Terminate\n");
	Exit(-1);
}

#ifndef NO_SUSPEND
SignalFunc
suspend()
{
	reset_tty_without_close();
	signal(SIGTSTP,SIG_DFL);
	kill(0,SIGTSTP);
}

SignalFunc
susp_cont()
{
	signal(SIGTSTP,suspend);
	reset_tty_without_close();
	set_tty();
	kill(ShellPID,SIGCONT);
}
#endif

#ifdef SIGWINCH
SignalFunc
winchange()
{
	int tty;
        signal(SIGWINCH,SIG_IGN);
	if (Shellout != NULL)
          tty = fileno(Shellout);
	else
	  tty = 1; /* stdout */
	get_winsize();
	set_winsize(tty);
	initFep();
	showcurmode();
        signal(SIGWINCH,winchange);
}
#endif

SignalFunc
sig_usr1()
{
        signal(SIGUSR1,SIG_IGN);
	initFep();
	toggleEscape(ViEsc);
        signal(SIGUSR1,sig_usr1);
}

SignalFunc
sig_usr2()
{
        signal(SIGUSR2,SIG_IGN);
	initFep();
	toggleEscape(EmacsEsc);
        signal(SIGUSR2,sig_usr2);
}

SignalFunc
sig_int()
{
        signal(SIGHUP,SIG_IGN);
	initFep();
	toggleEscape(NoEsc);
        signal(SIGHUP,sig_int);
}

set_int()
{	
	signal(SIGHUP ,hup_exit);
	signal(SIGINT ,sig_int);
/*	signal(SIGQUIT,iot_exit); */
	signal(SIGILL ,reset_exit);
	signal(SIGIOT ,iot_exit);
	signal(SIGFPE ,reset_exit);
#ifndef LINUX
	signal(SIGBUS ,bus_exit);
#endif
	signal(SIGSEGV,segv_exit);
	signal(SIGPIPE,pipe_exit);
	signal(SIGTERM,term_exit);
#ifndef NO_SUSPEND
	signal(SIGTSTP,suspend);
	signal(SIGCONT,susp_cont);
#endif
#ifdef SIGWINCH
	signal(SIGWINCH,winchange);
#endif
	signal(SIGUSR1,sig_usr1);
	signal(SIGUSR2,sig_usr2);
}

saveJisyo()
{
#ifdef USE_SERVER
	if ((NetLearnMode != LearnTemporal) && (UserDic != NULL)) {
		printf("Saving JISYO...\n");
		closeSKK(UserDic,UserDicName);
	}
#else
	if (UserDic != NULL) {
		printf("Saving JISYO...\n");
		closeSKK(UserDic,UserDicName);
	}
#endif
}

	
Exit(v)
int v;
{
	freeDevice();
	saveJisyo();
	exit(v);
}

Abort()
{
	freeDevice();
	saveJisyo();
	signal(SIGQUIT,SIG_DFL);
	signal(SIGIOT, SIG_DFL);
	abort();
}
