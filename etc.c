#include "config.h"
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include "fep.h"

static char *lastmodestr;
static char *curmodestr;

#ifdef NO_USLEEP
/*
 * alarm in microsecond
 */
static
alarm_ms(n)
int	n;
{
	struct itimerval itval;

	itval.it_interval.tv_sec = 0;
	itval.it_interval.tv_usec = 0;
	itval.it_value.tv_sec = n / 1000000;
	itval.it_value.tv_usec = n % 1000000;
	setitimer(ITIMER_REAL, &itval, NULL);
}

/*
 * sleep in microsecond
 */
sleep_ms(n)
int	n;
{
	void	break_sleep();

	signal(SIGALRM, break_sleep);
	alarm_ms(n);
	pause();
	signal(SIGALRM, SIG_DFL);
}

void
break_sleep()
{
}
#else

sleep_ms(n)
int n;
{
	usleep(n*1000);
}
#endif

showmode(s)
int s;
{
    lastmodestr = curmodestr;
    showmessage(modeString[s]);
    curmodestr = modeString[s];
}

showcurmode()
{
    showmessage(curmodestr);
}

showlastmode()
{
    showmessage(lastmodestr);
    curmodestr = lastmodestr;
}

showmessage(s)
char *s;
{
	if (msgLine()) {
		toMsg();
		writes(s);
		clearToEnd();
		fromMsg();
	}
}

kanjiInputEffect(on)
int on;
{
	underline(on);
}

kanjiSelectionEffect(on)
int on;
{
	standout(on);
}
