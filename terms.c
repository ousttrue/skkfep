#include	"config.h"
#include	"fep.h"

/*#define SHOW_ESCSEQ		/* show escape sequences for debugging */

#define DEF_TS	"\033[?T"
#define DEF_FS	"\033[?F"
#define DEF_SS	"\033[?S"
#define DEF_HS	"\033[?H"

char	bp[1024],funcstr[256],
	PC,*BC,*UP,
	*T_ce,*T_kr,*T_kl,*T_cr,*T_bt,*T_ta,*T_sc,*T_rc,
	*T_so,*T_se,*T_us,*T_ue,*T_cl,*T_cs,*T_cm;

char	*T_TS, *T_FS, *T_SS,*T_HS,*T_ES;
int     columns, lines;
StatusType status;

extern char *getenv();
extern char ReverseStatus;

#ifdef SHOW_ESCSEQ
printe(s,v)
char *s,*v;
{
	char *c;
	printf("%s=",s);
	for (c=v;*c;c++) {
		if (*c < ' ') {
			putchar('^');
			putchar(*c+'@');
		}
		else
			putchar(*c);
	}
	putchar('\n');
}
#define	GETSTR(v,s)	{v = pt; suc = tgetstr(s,&pt); adjstr(&v,&pt);printe(s,v);}
#else
#define	GETSTR(v,s)	{v = pt; suc = tgetstr(s,&pt); adjstr(&v,&pt);}
#endif

getTCstr()
{	int	suc;
	char	*pt = funcstr, *env;

	tgetent(bp,getenv("TERM"));

	GETSTR(T_ce,"ce");			/* clear to the end of line */
	GETSTR(T_kr,"nd");			/* cursor right */
	if (suc == -1)
		GETSTR(T_kr,"kr");
	if (tgetflag("bs")) T_kl = "\b";	/* cursor left */
	else {
		GETSTR(T_kl,"le");
		if (suc == -1)
			GETSTR(T_kl,"kb");
		if (suc == -1)
			GETSTR(T_kl,"kl");
	}
	adjstr(&T_kl,&pt);
	GETSTR(T_cr,"cr");		/* carrige return */
	GETSTR(T_ta,"ta");		/* tab */
	GETSTR(T_bt,"bt");		/* backtab */
	GETSTR(T_sc,"sc");		/* save cursor */
	if (suc == NULL)
		T_sc = NULL;
	GETSTR(T_rc,"rc");		/* restore cursor */
	if (suc == NULL)
		T_rc = NULL;
	if ((T_sc == NULL) || (T_rc == NULL)) {
		fprintf(stderr, "Your terminal cannot save/restore cursor.\n");
		Exit(-1);
	}
	GETSTR(T_so,"so");		/* standout mode */
	GETSTR(T_se,"se");		/* standout mode end */
	GETSTR(T_us,"us");		/* underline mode */
	GETSTR(T_ue,"ue");		/* underline mode end */
	GETSTR(T_cl,"cl");		/* clear screen */
	GETSTR(T_cm,"cm");		/* cursor motion */
	GETSTR(T_cs,"cs");		/* change scroll region */
	if (suc == NULL)
		T_cs = NULL;
	GETSTR(T_TS,"ts");		/* to status line */
	if (suc == NULL)
		T_TS = NULL;
	GETSTR(T_FS,"fs");		/* from status line */
	if (suc == NULL)
		T_FS = NULL;
	GETSTR(T_SS,"ss");		/* show status line */
	if (suc == NULL)
		T_SS = DEF_SS;
	GETSTR(T_HS,"hs");		/* hide status line */
	if (suc == NULL) {
		GETSTR(T_HS, "ds");	/* disable status line */
		if (suc == NULL)
			T_HS = DEF_HS;
	}
	GETSTR(T_ES,"es");		/* erase status line */
	if (suc == NULL)
		T_ES = NULL;
	
	if ( (T_TS != NULL) && (T_FS != NULL) ) {
		status = HaveStatusLine;
	} else if ( (T_cs != NULL) && (T_cm != NULL) &&
		    (T_sc != NULL) && (T_rc != NULL) ) {
		status = UseBottomLine;
	} else {
		status = NoStatusLine;
	}

	lines = tgetnum("li");
	if ( (env=getenv("LINES")) != NULL ) {
		lines = atoi(env);
	}
	columns = tgetnum("co");
	if ( (env=getenv("COLUMNS")) != NULL ) {
		columns = atoi(env);
	}
}

setEnvirons()
{
	extern char *version;
	char buf[1024], *p, *q, *e, c1, c2;
	int li, ignore;

	sprintf(buf, "%d", getpid()); 
	setenv("SKKFEP_PID",buf,1);
	sprintf(buf, "%s", version); 
	setenv("SKKFEP_VERSION",buf,1);

	if (msgLine() != (int)UseBottomLine) {
		return;
	}
	li = lines-1;

	sprintf(buf, "%d", columns);
	setenv("COLUMNS",buf,1);
	sprintf(buf, "%d", li);
	setenv("LINES",buf,1);

	ignore = 0;
#ifdef TERMINFO
	if (getenv("TERMCAP") !=NULL) {
#endif
	for(p=bp, q=buf; *p != '\0'; p++) {
		if ( *p == ':' ) {
			if ( !ignore ) *q++ = *p;
			c1 = *(p+1); c2 = *(p+2);
			switch ((c1<<8)+c2) {
			    case ('c'<<8)+'o': /* columns */
			    case ('l'<<8)+'i': /* lines */
			    case ('s'<<8)+'c': /* save cursor */
			    case ('r'<<8)+'c': /* restore cursor */
			    case ('c'<<8)+'s': /* change scroll region */
			    case ('t'<<8)+'s': /* to status line */
			    case ('f'<<8)+'s': /* from status line */
				ignore=1;
				break;
			    default:
				ignore=0;
				break;
			}
		} else {
			if ( !ignore ) *q++ = *p;
			if ( (*p == '\\') && (*(p+1) != '\0') ) {
				p++;
				if ( !ignore ) *q++ = *p;
			}
		}
	}
	if ( *(q-1) != ':' ) {
		*q++ = ':';
	}
	sprintf(q,"li#%d:", li);
	q += strlen(q);
	sprintf(q,"co#%d:", columns);
	q += strlen(q);
	setenv("TERMCAP",buf,1);
#ifdef TERMINFO
	}
#endif
}

#ifndef HAVE_SETENV
setenv(key, value, flag)
char *key, *value;
int flag;
{
	char *p;
#ifndef HAVE_PUTENV
	char **e,**ne,**newenv;
	int l,el,i;
	extern char **environ;
#endif

	if ((flag == 0) && (getenv(key)!=NULL)) {
		return 0;
	}
	p = (char *)malloc(strlen(key)+strlen(value)+2);
	if (p == NULL ) {
		return -1;
	}
	sprintf(p, "%s=%s", key, value);
#ifdef HAVE_PUTENV
	return (putenv(p) == 0)?(0):(-1);
#else
	l = strlen(key);
	for (e = environ, i = 0; *e != NULL; e++,i++) {
		if (strncmp(*e,key,l) == 0 && (*e)[l] == '=') {
			el = strlen(*e)-l-1;
			if (el >= strlen(value)) {
				strcpy(*e+l+1,value);
				return 0;
			}
			else {
				for (; *e != NULL; e++,i++) {
					*e = *(e+1);
				}
				i--;
				break;
			}
		}
	}
	newenv = (char**)malloc((i+2)*sizeof(char*));
	if (newenv == NULL)
		return -1;
	for (e = environ, ne = newenv; *e != NULL; *(ne++) = *(e++));
	*(ne++) = p;
	*ne = NULL;
	environ = newenv;
	return 0;
#endif
}
#endif /* not HAVE_SETENV */

adjstr(buf,ptr)
char	**buf,**ptr;
{
#if defined TERMINFO && !defined _AIX 
	char *p = (*ptr)-2;
	if( *(p--) == '>' ) {
		while ('0' <= *p && *p <= '9') p--;
		if( *(p--) == '<' && *p == '$' ) *p='\0';
	}
#else /* TERMCAP */
	while ('0' <= **buf && **buf <= '9')
		(*buf)++;
#endif /* TERMCAP */
	*((*ptr)++) = '\0';
}

msgLine()
{
	return (int)status;
}

toMsg()
{
	if (ReverseStatus)
		standout(1);
	if ( status == HaveStatusLine ) {
		writes(tgoto(T_TS,0,0));
	} else {
		writes(T_sc);
		writes(tgoto(T_cm,0,lines-1));
	}
}

fromMsg()
{
	if ( status == HaveStatusLine ) {
		writes(T_FS);
	} else {
		writes(T_rc);
	}
	if (ReverseStatus)
		standout(0);
}

initFep()
{
	if (status == HaveStatusLine) {
		if (T_SS != NULL) {
			writes(T_SS);
		}
	} else if (status == UseBottomLine) {
		writes("\r\n");
		writes(tgoto(T_cs,lines-2,0));
		writes(tgoto(T_cm,0,lines-2));
	}
}

termFep()
{
	if (status == HaveStatusLine) {
		if (T_HS != NULL) {
			writes(T_HS);
		}
	} else if (status == UseBottomLine) {
		writes(tgoto(T_cs,lines-1,0));
		writes(tgoto(T_cm, 0, lines-1));
	}
}

underline(ctl)
int ctl;
{
	if (ctl)
		writes(T_us);
	else
		writes(T_ue);
}

standout(ctl)
int ctl;
{
	if (ctl)
		writes(T_so);
	else
		writes(T_se);
}

saveCsr()
{
	writes(T_sc);
}

restoreCsr()
{
	writes(T_rc);
}

clearToEnd()
{
	writes(T_ce);
}

csrLeft(n)
int n;
{
	while (n) {
		writes(T_kl);
		n--;
	}
}

csrRight(n)
int n;
{
	while (n) {
		writes(T_kr);
		n--;
	}
}

cls()
{
	writes(T_cl);
}

erase(n)
int n;
{
	while (n) {
		write1(' ');
		n--;
	}
}

rubout(n)
int n;
{
	csrLeft(n);
	erase(n);
	csrLeft(n);
}
