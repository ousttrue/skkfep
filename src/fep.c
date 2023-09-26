#include "config.h"
#include "fep.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include "kanjicode.h"
#include "functions.h"
#include <errno.h>
#ifdef USE_LOCALE
#include <locale.h>
#endif

#ifndef __STDC__
# define const
#endif

#define SH_BUF_SIZ 256
#define FLUSH_TIMEOUT	300000	/* timeout ( micro-sec ) */

static char shellBuf[SH_BUF_SIZ];
void toAsc();
kanjicode decideCode();

KeymapPtr CurrentKeymap;
char UserDicName[64];
Dictionary UserDic;

short BlockTty;

extern char *version;
extern char ShellName[];
extern char **ShellArg;
extern char *KanaKey;

#ifdef DEBUG
FILE *wrlog;
#endif
#ifdef KANJIBS
char KanjiBS = KANJIBS_DEFAULT;
#endif

char PreserveOnFailure = PRESERVE_ON_FAILURE;
char ReverseStatus;
char OkuriFirst;

void
guess_system_kanji_code()
{
    char *p  = NULL;
    static struct Locale { const char* str; kanjicode code; } local[] = {
        { "ja_JP.JIS"    , jisBB },
        { "ja_JP.jis7"   , jisBB },
        { "ja_JP.EUC"    , euc   },
        { "japanese.euc" , euc   },
#ifdef HPUX /* for my hpux machine */
        { "japanese"     , sj    },
#else
        { "japanese"     , euc   },
#endif
        { "ja_JP.ujis"   , euc   },
        { "ja_JP.SJIS"   , sj    },
        { "ja_JP.mscode" , sj    }, {0,0}};

#ifdef USE_LOCALE
    p = setlocale(LC_CTYPE, "");
    if (p == NULL)
        p = getenv("LC_CTYPE");
#endif
    if (p == NULL)
        p = getenv("LANG");
    if (p != NULL)
    {
        struct Locale *tab;
        for (tab = local; tab->str; tab++)
        {
            if (strcmp(tab->str, p)==0)
            {
                OutCode = tab->code;
                WriteCode = tab->code;
                break;
            }
        }
    }
}

const char*
kanjicode2string(code)
{
    switch (code)
    {
    case euc: return "EUC";
    case sj:  return "MS-KANJI(ShiftJIS)";
    case jisAH: return "JIS(@H)";
    case jisAJ: return "JIS(@J)";
    case jisAB: return "JIS(@B)";
    case jisBH: return "JIS(BH)";
    case jisBJ: return "JIS(BJ)";
    case jisBB: return "JIS(BB)";
    }
}


main(argc,argv,envp)
int	argc;
char	*argv[],*envp[];
{
	char c;
	int i;
	FD_set selfds;
	int fdnum;
	void thru();
#ifdef DEBUG
	char *debfile = "rwlog";
#endif
        guess_system_kanji_code();
	/* Arguments */
	for (i = 1; i < argc; i++) {
		if (!strncmp(argv[i],"-o",2))
			OutCode = decideCode(argv[i]+2);
		else if (!strncmp(argv[i],"-f",2))
			WriteCode = decideCode(argv[i]+2);
		else if (!strncmp(argv[i],"-k",2))
			KanaKey = argv[i]+2;
		else if (!strcmp(argv[i],"-udic"))
			strcpy(UserDicName,argv[++i]);
#ifdef KANJIBS
		else if (!strcmp(argv[i],"-bs"))
			KanjiBS = ~KANJIBS_DEFAULT;
#endif
		else if (!strcmp(argv[i],"-e")) {
			strcpy(ShellName,argv[++i]);
			ShellArg = argv+i;
			break;
		}
		else if (!strcmp(argv[i],"-P")) {
			PreserveOnFailure ^= 1;
		}
		else if (!strcmp(argv[i],"-esc")) {
			setEscape(SimpleEsc);
			LastEscapeBehavior = NoEsc;
		}
		else if (!strcmp(argv[i],"-viesc")) {
			setEscape(ViEsc);
			LastEscapeBehavior = NoEsc;
		}
		else if (!strcmp(argv[i],"-emacsesc")) {
			setEscape(EmacsEsc);
			LastEscapeBehavior = NoEsc;
		}
		else if (!strcmp(argv[i],"-rs")) {
			ReverseStatus = 1;
		}
#ifdef DEBUG
		else if (!strcmp(argv[i],"-deb"))
			debfile = argv[++i];
#endif
		else {
			fprintf(stderr,"skkfep: bad option %s\n",argv[i]);
			fprintf(stderr,"\
usage: skkfep [options]\n\
options:\n\
	-o{euc|sj|jis{A@B}{HJB}}	DISPLAY KANJI CODE\n\
	-f{euc|sj|jis{A@B}{HJB}}	FILEIO KANJI CODE\n\
	-udic <dictionary>		USER DICTIONARY\n\
	-bs				TOGGLE BS BEHAVIOR\n\
	-k<char>			SKK TOGGLE KEY\n\
	-e cmd				EXECUTED COMMAND\n\
	-lm {none|temporal|permanent}   LEARN MODE\n\
	-P				TOGGLE BEHAVIOR OF CONVERSION FAILURE\n\
	-esc				ENABLE THROUGH ESCAPE\n\
	-viesc				ENABLE THROUGH ESCAPE and iaoIASR\n\
	-emacsesc			ENABLE THROUGH ESCAPE and etc.\n\
	-rs				REVERSE STATUS LINE\n\
");
			exit(1);
		}
	}
#ifdef DEBUG
	wrlog = fopen(debfile,"w");
	if (wrlog == NULL) {
		fprintf(stderr,"Can't open %s\n",debfile);
		exit(1);
	}
#endif
			
	printf("SKKFEP version %s\n",version);
        printf("Display: %s, FileIO: %s.\n",
            kanjicode2string(OutCode), kanjicode2string(WriteCode));

	/* Initialize */
	setKanaKey();
	getTCstr();
	tty_ready();
	get_winsize();
	set_tty();
	set_int();

	if (*UserDicName == '\0') {
		strcpy(UserDicName,getenv("HOME"));
		strcat(UserDicName,"/");
		strcat(UserDicName,USER_DIC_NAME);
	}
	UserDic = openSKK(UserDicName);
	toAsc();

	establishShell();

	/* Preparation for select() */

	FD_ZERO(&selfds);
	fdnum = Shellfd+1;

	/* Loop */
	for (;;) {
		FD_SET(0,&selfds);
		FD_SET(Shellfd,&selfds);
		i = select(fdnum,&selfds,NULL,NULL,NULL);
                if (i == -1 && errno == EINTR)
                    continue;
		if (FD_ISSET(Shellfd,&selfds) && !BlockTty) {	
			/* Shell input is ready */
			if ((i = read(Shellfd,shellBuf,SH_BUF_SIZ)) > 0) 
				writeShTty(shellBuf,i);
			if (i == SH_BUF_SIZ) continue;
		}
		if (!FD_ISSET(0,&selfds)) { /* Key input not ready */
			continue;
		}
		if (ioctl(0,FIONREAD,&i) == 0) {
			while (i) {
				char o = OkuriFirst;
				OkuriFirst = 0;
				c = getchar();
				if (c & 0x80)
					thru(c);
				else
					(*CurrentKeymap[c])(c,o);
				i--;
			}
		}
	}
}

kanjicode
decideCode(s)
char *s;
{
	if (!strcmp(s,"sj"))
		return sj;
	else if (!strcmp(s,"euc"))
		return euc;
	else if (!strncmp(s,"jis",3)) {
		switch (s[3]) {
		case 'A': case '@':
			switch(s[4]) {
			case 'H': return jisAH;
			case 'J': return jisAJ;
			case 'B': return jisAB;
			}
			break;
		case 'B':
			switch(s[4]) {
			case 'H': return jisBH;
			case 'J': return jisBJ;
			case 'B': return jisBB;
			}
			break;
		}
	}
	return jisAJ;	/* default */
}

flushOut(minchar)
int minchar;
{
	int i,nchar,ntimeout;
	FD_set readfds;
	struct timeval timeout;

	FD_ZERO(&readfds);
	FD_SET(Shellfd,&readfds);
	timeout.tv_sec = FLUSH_TIMEOUT/1000000;
	timeout.tv_usec = FLUSH_TIMEOUT%1000000;
	ntimeout = 0;
	nchar = 0;
	while (ntimeout < minchar) {
		i = select(Shellfd+1,&readfds,NULL,NULL,&timeout);
                if (i == -1 && errno == EINTR)
                    continue;
		if (i > 0) {
			i = read(Shellfd,shellBuf,SH_BUF_SIZ);	
			writeShTty(shellBuf,i);
			nchar += i;
			if (nchar >= minchar)
				break;
		}
		else
			ntimeout++;
	}
}

void
nulcmd(c)
char c;
{}


void
thru(c)
char c;
{
	writeTtyShell1(c);
}


void
toAsc(c)
char c;
{
	flushKana();
	setKeymap(&CurrentKeymap,convertKeymap(&NormalKeymap));
	showmode(SKK_MODE);
}

void
toZenA(c)
char c;
{
	flushKana();
	setKeymap(&CurrentKeymap,ZenkakuKeymap);
	showmode(ZENEI_MODE);
}

void
thruToAsc(c)
{
	toAsc(c);
	thru(c);
}

void
toEsc(c)
{
	flushKana();
	setKeymap(&CurrentKeymap,convertKeymap(&EscapedKeymap));
	showmode(SKK_MODE);
}

void
thruToEsc(c)
char c;
{
	toEsc(c);
	thru(c);
}

void
thruBack(c)
char c;
{
	thru(c);
	toKana(c);
}

void
thru1(c)
char c;
{
	thru(c);
	restoreKeymap(&CurrentKeymap);
	showlastmode();
}
