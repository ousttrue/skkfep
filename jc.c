/*
 *	jc: Kanji Code Converter
 *
 *		by Akinori Ito (R.C.A.I.S. Tohoku Univ.)
 *			10 August, 1988
 *		by Akinori ITO
 *			Kanji-code checker
 *
 *	CAUTION: This source code is public domain.
 *
 *	usage: jc <conv-type> [options] [files]
 *	       jc -c [files]
 *
 *	<conv-type> ::= -<fromtype><totype>
 *	<type>::=
 *		S,I	Shift-JIS
 *		E	EUC
 *		J	JIS (escape $@ - escape (H )
 *		j	JIS (escape $@ - escape (J )
 *		N	JIS (escape $B - escape (J )
 *		n	JIS (escape $B - escape (B )
 *		m	JIS (escape $@ - escape (B )
 *		i	JIS (escape $@ - escape (@ )
 *		U	UX  (escape $ - escape + )
 *		A	JIS (Shift-in code is escape $@ or escape $B,
 *			     Shift-out code is escape (J or escape (B
 *			     or escape (@.
 *			     Shift-in/out code is automatically selected.
 *			     This type is valid only  as <fromtype>.)
 *	options:
 *		-p PAGER
 *
 */

#include	<stdio.h>
#include	<ctype.h>

#include	"config.h"

#ifdef _IBMESA
#define	uchar		char
#else
#define	uchar		unsigned char
#endif

#define	BF_SIZ		2048
#define	bool		char
#define	true		1
#define	false		0
#define	FROMCODES	10	/* Number of supported Kanji code (read) */
#define	TOCODES		9	/* Number of supported Kanji code (write) */

#define	JSIcode  "\033$@"
#define	JSOcode  "\033(H"
#define	J2SIcode "\033$@"
#define	J2SOcode "\033(J"
#define	J3SIcode "\033$@"
#define	J3SOcode "\033(B"
#define	NSIcode  "\033$B"
#define	NSOcode  "\033(J"
#define	N2SIcode  "\033$B"
#define	N2SOcode  "\033(B"
#define	USIcode  "\033$"
#define	USOcode  "\033+"
#define ISIcode  "\033$@"
#define ISOcode  "\033(@"

uchar	Ibuf[BF_SIZ],Obuf[BF_SIZ];
char	*SIcode,*SOcode;
char	*cOption;
char	*pager = NULL;

bool	euc = false;

void	cConvSJ(),cConvJS(),cConvSE(),cConvES(),cConvAS(),cConvSP(),cConvPS();

typedef struct _ConvRoutine {
	char	key;
	void	(*routine)();
	char	*ShiftIn,*ShiftOut;
} ConvRoutine;

ConvRoutine FromSJ[] = {
	{'J',cConvSJ,JSIcode,JSOcode},
	{'N',cConvSJ,NSIcode,NSOcode},
	{'n',cConvSJ,N2SIcode,N2SOcode},
	{'m',cConvSJ,J3SIcode,J3SOcode},
	{'U',cConvSJ,USIcode,USOcode},
	{'j',cConvSJ,J2SIcode,J2SOcode},
	{'i',cConvSJ,ISIcode,ISOcode},
	{'E',cConvSE,"",""},
	{'P',cConvSP,"",""},
};

ConvRoutine ToSJ[] = {
	{'J',cConvJS,JSIcode,JSOcode},
	{'N',cConvJS,NSIcode,NSOcode},
	{'n',cConvJS,N2SIcode,N2SOcode},
	{'m',cConvJS,J3SIcode,J3SOcode},
	{'U',cConvJS,USIcode,USOcode},
	{'j',cConvJS,J2SIcode,J2SOcode},
	{'j',cConvJS,ISIcode,ISOcode},
	{'A',cConvAS,"",""},
	{'E',cConvES,"",""},
	{'P',cConvPS,"",""},
};

FILE	*file_open(),*popen();

main(argc,argv)
int	argc;
char	*argv[];
{
	char checkKanjiCode();
	int i;
	FILE *f;
	static
	char	tmpf[60];
	char	cvchar[4];
	bool	etmp = false;
	int	nargc;
	char	**nargv;
	char	*nargptr;

	if (argc < 2 || argv[1][0] != '-') {
		fprintf(stderr,"No convertion-option specified\n");
		usage();
		exit(-1);
	}
	if (!strcmp(argv[1],"-c")) {
		if (argc == 2) {
			printf("%c\n",checkKanjiCode(stdin));
		}
		else {
			for (i = 2; i < argc; i++) {
				f = file_open(argv[i],"r");
				printf("%c\n",checkKanjiCode(f));
				fclose(f);
			}
		}
		exit(0);
	}
	else if (!strncmp(argv[1],"-a",2)) {
		if (argc == 2) {
			etmp = true;
			sprintf(tmpf,"/tmp/jctmp%d",getpid());
			if ((f = fopen(tmpf,"w")) == NULL) {
				fprintf(stderr,"jc: Can't open tmpfile %s\n",tmpf);
				exit(1);
			}
			while (!feof(stdin))
				putc(getchar(),f);
			fclose(f);
			f = fopen(tmpf,"r");
			cvchar[0] = '-';
			cvchar[1] = checkKanjiCode(f);
			cvchar[2] = argv[1][2];
			cvchar[3] = '\0';
			fclose(f);
			cOption = cvchar;
			nargptr = tmpf;
			nargc = 1;
			nargv = &nargptr;
		}
		else {
			f = file_open(argv[2],"r");
			cvchar[0] = '-';
			cvchar[1] = checkKanjiCode(f);
			cvchar[2] = argv[1][2];
			cvchar[3] = '\0';
			fclose(f);
			cOption = cvchar;
			nargc = argc-2;
			nargv = argv+2;
		}
	}
	else {
		cOption = argv[1];
		nargc = argc-2;
		nargv = argv+2;
	}
	doConv(nargc,nargv,cOption[1],cOption[2]);
	if (etmp)
		unlink(tmpf);
	exit(0);
}

doConv(argc,argv,fromCode,toCode)
int	argc;
char	**argv,fromCode,toCode;
{	FILE	*f = stdin,*outf = stdout;
	int	i = 0;

	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'p':
				if (outf != stdout) {
					fprintf(stderr,"PAGER already open\n");
					exit(-1);
				}
				pager = argv[++i];
				outf = popen(pager,"w");
				if (outf == NULL) {
					fprintf(stderr,"Cannot open PAGER %s\n",pager);
					exit(-1);
				}
				break;
			default:
				usage();
				exit(-1);
			}
			continue;
		}
		else
			f = file_open(argv[i],"r");
		while(fgets((char *)Ibuf,BF_SIZ,f) != NULL) {
			conv(Ibuf,Obuf,fromCode,toCode);
			fputs((char *)Obuf,outf);
		}
		fclose(f);
		f = NULL;
	}
	if (f == stdin) {
		while(fgets((char *)Ibuf,BF_SIZ,f) != NULL) {
			conv(Ibuf,Obuf,fromCode,toCode);
			fputs((char *)Obuf,outf);
		}
	}
	if (pager) {
		pclose(outf);
	}
}

conv(is,os,fc,tc)
char	*is,*os,fc,tc;
{	static uchar	mbuf[BF_SIZ];
	int		i;

	if (fc != 'I' && fc != 'S') {
		for (i = 0; i < FROMCODES; i++) {
			if (ToSJ[i].key == fc) {
				SIcode = ToSJ[i].ShiftIn;
				SOcode = ToSJ[i].ShiftOut;
				(*ToSJ[i].routine)(is,mbuf);
				goto next;
			}
		}
		n_impr(cOption);
	}
	else 
		strcpy(mbuf,is);
next:
	if (tc != 'I' && tc != 'S') {
		for (i = 0; i < TOCODES; i++) {
			if (FromSJ[i].key == tc) {
				SIcode = FromSJ[i].ShiftIn;
				SOcode = FromSJ[i].ShiftOut;
				(*FromSJ[i].routine)(mbuf,os);
				return;
			}
		}
		n_impr(cOption);
	}
	else 
		strcpy(os,mbuf);
}

n_impr(s)
char	*s;
{
	fprintf(stderr,"jc: option %s is not implemented yet... sorry\n",s);
	usage();
	exit(-1);
}

putSFT(pptr,code)	/* Put Shift in/out code */
uchar	**pptr,*code;
{
	while (*code != '\0') 
		*((*pptr)++) = *(code++);
}

uchar getSLb(ptr,ub)	/* Get Shift-JIS Lower byte */
uchar	*ptr,*ub;
{	uchar	c = *ptr;

	*ub <<= 1;
	if (c < 0x9f) {
		if (c > 0x7e)
			c--;
		*ub -= 1;
		c -= 0x3f;
	}
	else {
		c -= 0x9e;
	}
	return c;
}

void cConvSP(is,os)
uchar *is,*os;
{
	uchar buf[BF_SIZ],pbuf[4];
	uchar *b = buf;

	euc = true;
	cConvSJ(is,buf);
	while (*b) {
		if (*b & 0x80) {
			sprintf((char *)pbuf,"%o",*b);
			*(os++) = '\\';
			*(os++) = pbuf[0];
			*(os++) = pbuf[1];
			*(os++) = pbuf[2];
			b++;
		}
		else
			*(os++) = *(b++);
	}
	*os = '\0';
}

void cConvPS(is,os)
uchar *is,*os;
{
	uchar buf[BF_SIZ];
	uchar *b = buf;
	int i;

	while (*is) {
		if (*is == '\\' && 
		    isdigit(is[1]) &&
		    isdigit(is[2]) &&
		    isdigit(is[3])) {
			i = (is[1]-'0')*64+(is[2]-'0')*8+(is[3]-'0');
			*(b++) = i;
			is += 4;
		}
		else
			*(b++) = *(is++);
	}
	*b = '\0';
	cConvES(buf,os);
}

void cConvSE(is,os)
uchar	*is,*os;
{
	euc = true;
	cConvSJ(is,os);
}

void cConvSJ(is,os)		/* Convert Shift-JIS to JIS/real JIS/EUC */
uchar	*is,*os;
{	uchar	c,km = 0,ub,lb,*p,*q = os;

	for (p = is; *p != '\0'; p++) {
		c = *p;
		if (0x80 <= c && c <= 0x9f) {
			if (!euc && km == 0) {
				putSFT(&q,SIcode);
				km = 1;
			}
			p++;
			ub = c - 0x80;
			lb = getSLb(p,&ub);
			ub += 0x20;
			lb += 0x20;
			if (euc) {
				ub |= 0x80;
				lb |= 0x80;
			}
			*(q++) = ub;
			*(q++) = lb;
		}
		else if (0xe0 <= c && c <= 0xef) {
			if (!euc && km == 0) {
				putSFT(&q,SIcode);
				km = 1;
			}
			p++;
			ub = (c & 0x7f) - 0x40;
			lb = getSLb(p,&ub);
			ub += 0x20;
			lb += 0x20;
			if (euc) {
				ub |= 0x80;
				lb |= 0x80;
			}
			*(q++) = ub;
			*(q++) = lb;
		}
		else {
			if (!euc && km == 1) {
				putSFT(&q,SOcode);
				km = 0;
			}
			*(q++) = c;
		}
	}
	*q = '\0';
}

void cConvJS(is,os)		/* Convert JIS to Shift-JIS */
char	*is,*os;
{	char	*ip = is,*op = os;
	char	kmode = 0;
	int	il = strlen(SIcode),
		ol = strlen(SOcode);

	while (*ip != '\0') {
		if (kmode) {
			if (strncmp(ip,SOcode,ol) == 0) {
				ip += ol;
				kmode = 0;
			}
			else {
				put_js(*ip,*(ip+1),op);
				ip += 2;
				op += 2;
			}
		}
		else {
			if (strncmp(ip,SIcode,il) == 0) {
				ip += il;
				kmode = 1;
			}
			else
				*(op++) = *(ip++);
		}
	}
	*op = '\0';
}

void cConvES(is,os)		/* Convert EUC to Shift-JIS */
char	*is,*os;
{	char	*ip = is,*op = os;
	uchar	ub,lb;
	bool	firstByte = true;

	while (*ip != '\0') {
		ub = (uchar)*(ip++);
		if (ub >= 0x80) {
			ub &= 0x7f;
			lb = (uchar)*(ip++) & 0x7f;
			put_js(ub,lb,op);
			op += 2;
		}
		else {
			*(op++) = (char)ub;
		}
	}
	*op = '\0';
}

put_js(ub,lb,op)
uchar	ub,lb;
uchar	*op;
{	
	ub -= 0x20;
	lb -= 0x20;
	if ((ub&1) == 0)
		lb += 94;
	ub = ((ub-1)>>1)+0x81;
	lb += 0x3f;
	if (ub > 0x9f)
		ub += 0x40;
	if (lb > 0x7e)
		lb++;

	*op = ub;
	*(op+1) = lb;
}

FILE *file_open(fn,m)
char	*fn,*m;
{	FILE	*f;
	f = fopen(fn,m);
	if (f == NULL) {
		fprintf(stderr,"Cannot open %s\n",fn);
		exit(-1);
	}
	return f;
}

void cConvAS(is,os)		/* Convert JIS to Shift-JIS */
char	*is,*os;
{	char	*ip = is,*op = os;
	char	kmode = 0;

	while (*ip != '\0') {
		if (kmode) {
			if (*ip == '\033' && *(ip+1) == '(' &&
				(*(ip+2) == 'J' || 
				 *(ip+2) == 'B' || 
				 *(ip+2) == 'H' ||
				 *(ip+2) == '@')) {
				ip += 3;
				kmode = 0;
			}
			else {
				put_js(*ip,*(ip+1),op);
				ip += 2;
				op += 2;
			}
		}
		else {
			if (*ip == '\033' && *(ip+1) == '$' &&
				(*(ip+2) == '@' || *(ip+2) == 'B')) {
				ip += 3;
				kmode = 1;
			}
			else
				*(op++) = *(ip++);
		}
	}
	*op = '\0';
}

char
checkKanjiCode(f)
FILE *f;
{
	unsigned char c;
	char res = '\0';
	short bit8 = 0;

	while (c = fgetc(f),!feof(f)) {
		if (c & 0x80) {
			bit8 = 1;
			if (0x80 <= c && c <= 0x9f) { /* SJIS */
				if (res && res != 'S') {
					return '?';
				}
				else
					res = 'S';
			    	c = fgetc(f);
			}
			else if (0xe0 <= c && c <= 0xea) {
			    	c = fgetc(f);
			    	if (!(c & 0x80)) {	/* SJIS */
			    		if (res && res != 'S') {
			    			return '?';
					}
					else
						res = 'S';
				}
			}
			else {
				c = fgetc(f);
			}
		}
		else if (c == '\033') {
			c = fgetc(f);
			if (c == '(') { /* ) */
				switch (c = fgetc(f)) {
				case 'H':
					if (res == 'j' || res == 'J')
						res = 'J';
					else
						res = 'A';
					break;
				case 'J':
					if (res != 'j' && res != 'N')
						res = 'A';
					break;
				case 'B':
					if (res == 'N' || res == 'n')
						res = 'n';
					else if (res == 'j')
						res = 'm';
					else
						res = 'A';
					break;
				case '@':
					if (res == 'j' || res == 'i')
						res = 'i';
					else
						res = 'A';
					break;
				}
			}
			else if (c == '$') {
				switch (c = fgetc(f)) {
				case '@':
					if (!res)
						res = 'j';
					else if (res == 'N' || res == 'n')
						res = 'A';
					break;
				case 'B':
					if (!res)
						res = 'N';
					else if (res != 'N' && res != 'n')
						res = 'A';
						break;
				}
			}
		}
	}
	if (!res && bit8)
		res = 'E';
	return res;
}
				

usage()
{
	fputs("\
 usage: jc <conv-type> [options] [files] or\n\
        jc -c [files]\n\
\n\
<conv-type> ::= -<fromtype><totype>\n\
<type>::=\n\
	S,I	Shift-JIS\n\
	E	EUC\n\
	J	JIS (escape $@ - escape (H )\n\
	j	JIS (escape $@ - escape (J )\n\
	N	JIS (escape $B - escape (J )\n\
	n	JIS (escape $B - escape (B )\n\
	m	JIS (escape $@ - escape (B )\n\
	i	JIS (escape $@ - escape (@ )\n\
	U	UX  (escape $ - escape + )\n\
	A	JIS (Shift-in code is escape $@ or escape $B,\n\
		     Shift-out code is escape (J or escape (B.\n\
		     Shift-in/out code is automatically selected.\n\
		     This type is valid only  as <fromtype>.)\n\
options:\n\
	-p PAGER\n\
",stderr);
}
