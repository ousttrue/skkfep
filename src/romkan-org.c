#include "config.h"
#include "fep.h"
#include <ctype.h>

#define VOW_A	0
#define VOW_I	1
#define VOW_U	2
#define VOW_E	3
#define VOW_O	4

#define CON_0	0
#define CON_K	1
#define CON_S	2
#define CON_T	3
#define	CON_N	4
#define CON_H	5
#define CON_M	6
#define CON_R	7
#define CON_G	8
#define CON_Z	9
#define CON_D	10
#define CON_B	11
#define CON_P	12
#define CON_X	13
#define CON_Y	14
#define CON_W	15
#define CON_KY	16
#define CON_SY	17
#define CON_TY	18
#define CON_NY	19
#define CON_HY	20
#define CON_MY	21
#define CON_RY	22
#define CON_GY	23
#define CON_ZY	24
#define CON_DY	25
#define CON_BY	26
#define CON_PY	27
#define CON_XY	28
#define CON_CH	29
#define CON_J	30
#define CON_SH	31
#define CON_TS	32
#define CON_XW	33
#define CON_F	34
#define CON_V	35
#define CON_DH	36
#define CON_JY	37
#define CON_TH	38
#define CON_NN	39

#define KANA_NN		[CON_NN][0]
#define KANA_XTU	[CON_NN][1]

static
char *HiraTab[][5] = {
	{"$B$"(B","$B$$(B","$B$&(B","$B$((B","$B$*(B"},		/* vowel = 0 */
	{"$B$+(B","$B$-(B","$B$/(B","$B$1(B","$B$3(B"},		/* k = 1 */
	{"$B$5(B","$B$7(B","$B$9(B","$B$;(B","$B$=(B"},		/* s = 2 */
	{"$B$?(B","$B$A(B","$B$D(B","$B$F(B","$B$H(B"},		/* t = 3 */
	{"$B$J(B","$B$K(B","$B$L(B","$B$M(B","$B$N(B"},		/* n = 4 */
	{"$B$O(B","$B$R(B","$B$U(B","$B$X(B","$B$[(B"},		/* h = 5 */
	{"$B$^(B","$B$_(B","$B$`(B","$B$a(B","$B$b(B"},		/* m = 6 */
	{"$B$i(B","$B$j(B","$B$k(B","$B$l(B","$B$m(B"},		/* r = 7 */
	{"$B$,(B","$B$.(B","$B$0(B","$B$2(B","$B$4(B"},		/* g = 8 */
	{"$B$6(B","$B$8(B","$B$:(B","$B$<(B","$B$>(B"},		/* z = 9 */
	{"$B$@(B","$B$B(B","$B$E(B","$B$G(B","$B$I(B"},		/* d = 10 */
	{"$B$P(B","$B$S(B","$B$V(B","$B$Y(B","$B$\(B"},		/* b = 11 */
	{"$B$Q(B","$B$T(B","$B$W(B","$B$Z(B","$B$](B"},		/* p = 12 */
	{"$B$!(B","$B$#(B","$B$%(B","$B$'(B","$B$)(B"},		/* x = 13 */
	{"$B$d(B","$B$$(B","$B$f(B","$B$$$'(B","$B$h(B"},		/* y = 14 */
	{"$B$o(B","$B$&$#(B","$B$&(B","$B$&$'(B","$B$r(B"},		/* w = 15 */
	{"$B$-$c(B","$B$-$#(B","$B$-$e(B","$B$-$'(B","$B$-$g(B"},	/* ky = 16 */
	{"$B$7$c(B","$B$7$#(B","$B$7$e(B","$B$7$'(B","$B$7$g(B"},	/* sy = 17 */
	{"$B$A$c(B","$B$A$#(B","$B$A$e(B","$B$A$'(B","$B$A$g(B"},	/* ty = 18 */
	{"$B$K$c(B","$B$K$#(B","$B$K$e(B","$B$K$'(B","$B$K$g(B"},	/* ny = 19 */
	{"$B$R$c(B","$B$R$#(B","$B$R$e(B","$B$R$'(B","$B$R$g(B"},	/* hy = 20 */
	{"$B$_$c(B","$B$_$#(B","$B$_$e(B","$B$_$'(B","$B$_$g(B"},	/* my = 21 */
	{"$B$j$c(B","$B$j$#(B","$B$j$e(B","$B$j$'(B","$B$j$g(B"},	/* ry = 22 */
	{"$B$.$c(B","$B$.$#(B","$B$.$e(B","$B$.$'(B","$B$.$g(B"},	/* gy = 23 */
	{"$B$8$c(B","$B$8$#(B","$B$8$e(B","$B$8$'(B","$B$8$g(B"},	/* zy = 24 */
	{"$B$B$c(B","$B$B$#(B","$B$B$e(B","$B$B$'(B","$B$B$g(B"},	/* dy = 25 */
	{"$B$S$c(B","$B$S$#(B","$B$S$e(B","$B$S$'(B","$B$S$g(B"},	/* by = 26 */
	{"$B$T$c(B","$B$T$#(B","$B$T$e(B","$B$T$'(B","$B$T$g(B"},	/* py = 27 */
	{"$B$c(B","$B$#(B","$B$e(B","$B$'(B","$B$g(B"},		/* xy = 28 */
	{"$B$A$c(B","$B$A(B","$B$A$e(B","$B$A$'(B","$B$A$g(B"},	/* ch = 29 */
	{"$B$8$c(B","$B$8(B","$B$8$e(B","$B$8$'(B","$B$8$g(B"},	/* j  = 30 */
	{"$B$7$c(B","$B$7(B","$B$7$e(B","$B$7$'(B","$B$7$g(B"},	/* sh = 31 */
	{"$B$D$!(B","$B$D$#(B","$B$D(B","$B$D$'(B","$B$D$)(B"},	/* ts = 32 */
	{"$B$n(B","$B$#(B","$B$%(B","$B$'(B","$B$)(B"},		/* xw = 33 */
	{"$B$U$!(B","$B$U$#(B","$B$U(B","$B$U$'(B","$B$U$)(B"},	/* f  = 34 */
	{"$B$P(B","$B$S(B","$B$V(B","$B$Y(B","$B$\(B"},		/* v  = 35 */
	{"$B$@$!(B","$B$G$#(B","$B$I$%(B","$B$G$'(B","$B$I$)(B"},	/* dh = 36 */
	{"$B$8$c(B","$B$8$#(B","$B$8$e(B","$B$8$((B","$B$8$g(B"},	/* jy = 37 */
	{"$B$F$c(B","$B$F$#(B","$B$F$e(B","$B$F$'(B","$B$F$g(B"},	/* th = 38 */
	{"$B$s(B","$B$C(B","$B$p(B","$B$q(B",""},		/* others = 39 */
};

char *KataTab[][5] = {
	{"$B%"(B","$B%$(B","$B%&(B","$B%((B","$B%*(B"},
	{"$B%+(B","$B%-(B","$B%/(B","$B%1(B","$B%3(B"},
	{"$B%5(B","$B%7(B","$B%9(B","$B%;(B","$B%=(B"},
	{"$B%?(B","$B%A(B","$B%D(B","$B%F(B","$B%H(B"},
	{"$B%J(B","$B%K(B","$B%L(B","$B%M(B","$B%N(B"},
	{"$B%O(B","$B%R(B","$B%U(B","$B%X(B","$B%[(B"},
	{"$B%^(B","$B%_(B","$B%`(B","$B%a(B","$B%b(B"},
	{"$B%i(B","$B%j(B","$B%k(B","$B%l(B","$B%m(B"},
	{"$B%,(B","$B%.(B","$B%0(B","$B%2(B","$B%4(B"},
	{"$B%6(B","$B%8(B","$B%:(B","$B%<(B","$B%>(B"},
	{"$B%@(B","$B%B(B","$B%E(B","$B%G(B","$B%I(B"},
	{"$B%P(B","$B%S(B","$B%V(B","$B%Y(B","$B%\(B"},
	{"$B%Q(B","$B%T(B","$B%W(B","$B%Z(B","$B%](B"},
	{"$B%!(B","$B%#(B","$B%%(B","$B%'(B","$B%)(B"},
	{"$B%d(B","$B%$(B","$B%f(B","$B%$%'(B","$B%h(B"},
	{"$B%o(B","$B%&%#(B","$B%&(B","$B%&%'(B","$B%r(B"},
	{"$B%-%c(B","$B%-%#(B","$B%-%e(B","$B%-%'(B","$B%-%g(B"},
	{"$B%7%c(B","$B%7%#(B","$B%7%e(B","$B%7%'(B","$B%7%g(B"},
	{"$B%A%c(B","$B%A%#(B","$B%A%e(B","$B%A%'(B","$B%A%g(B"},
	{"$B%K%c(B","$B%K%#(B","$B%K%e(B","$B%K%'(B","$B%K%g(B"},
	{"$B%R%c(B","$B%R%#(B","$B%R%e(B","$B%R%'(B","$B%R%g(B"},
	{"$B%_%c(B","$B%_%#(B","$B%_%e(B","$B%_%'(B","$B%_%g(B"},
	{"$B%j%c(B","$B%j%#(B","$B%j%e(B","$B%j%'(B","$B%j%g(B"},
	{"$B%.%c(B","$B%.%#(B","$B%.%e(B","$B%.%'(B","$B%.%g(B"},
	{"$B%8%c(B","$B%8%#(B","$B%8%e(B","$B%8%'(B","$B%8%g(B"},
	{"$B%B%c(B","$B%B%#(B","$B%B%e(B","$B%B%'(B","$B%B%g(B"},
	{"$B%S%c(B","$B%S%#(B","$B%S%e(B","$B%S%'(B","$B%S%g(B"},
	{"$B%T%c(B","$B%T%#(B","$B%T%e(B","$B%T%'(B","$B%T%g(B"},
	{"$B%c(B","$B%#(B","$B%e(B","$B%'(B","$B%g(B"},
	{"$B%A%c(B","$B%A(B","$B%A%e(B","$B%A%'(B","$B%A%g(B"},
	{"$B%8%c(B","$B%8(B","$B%8%e(B","$B%8%'(B","$B%8%g(B"},
	{"$B%7%c(B","$B%7(B","$B%7%e(B","$B%7%'(B","$B%7%g(B"},
	{"$B%D%!(B","$B%D%#(B","$B%D(B","$B%D%'(B","$B%D%)(B"},
	{"$B%n(B","$B%#(B","$B%%(B","$B%'(B","$B%)(B"},
	{"$B%U%!(B","$B%U%#(B","$B%U(B","$B%U%'(B","$B%U%)(B"},
	{"$B%t%!(B","$B%t%#(B","$B%t(B","$B%t%'(B","$B%t%)(B"},
	{"$B%@%!(B","$B%G%#(B","$B%I%%(B","$B%G%'(B","$B%I%)(B"},
	{"$B%8%c(B","$B%8%#(B","$B%8%e(B","$B%8%'(B","$B%8%g(B"},
	{"$B%F%c(B","$B%F%#(B","$B%F%e(B","$B%F%'(B","$B%F%g(B"},
	{"$B%s(B","$B%C(B","$B%p(B","$B%q(B",""},
};

char *ZenkakuAlpha[] = {
/*	SPC	!	"	#	$	%	&	'	*/
	"$B!!(B",	"$B!*(B",	"$B!I(B",	"$B!t(B",	"$B!p(B",	"$B!s(B",	"$B!u(B",	"$B!G(B",
/*	(	)	*	+	,	-	.	/	*/
	"$B!J(B",	"$B!K(B",	"$B!v(B",	"$B!\(B",	"$B!$(B",	"$B!](B",	"$B!%(B",	"$B!?(B",
/*	0	1	2	3	4	5	6	7	*/
	"$B#0(B",	"$B#1(B",	"$B#2(B",	"$B#3(B",	"$B#4(B",	"$B#5(B",	"$B#6(B",	"$B#7(B",
/*	8	9	:	;	<	=	>	?	*/
	"$B#8(B",	"$B#9(B",	"$B!'(B",	"$B!((B",	"$B!c(B",	"$B!a(B",	"$B!d(B",	"$B!)(B",
/*	@	A	B	C	D	E	F	G	*/
	"$B!w(B",	"$B#A(B",	"$B#B(B",	"$B#C(B",	"$B#D(B",	"$B#E(B",	"$B#F(B",	"$B#G(B",
/*	H	I	J	K	L	M	N	O	*/
	"$B#H(B",	"$B#I(B",	"$B#J(B",	"$B#K(B",	"$B#L(B",	"$B#M(B",	"$B#N(B",	"$B#O(B",
/*	P	Q	R	S	T	U	V	W	*/
	"$B#P(B",	"$B#Q(B",	"$B#R(B",	"$B#S(B",	"$B#T(B",	"$B#U(B",	"$B#V(B",	"$B#W(B",
/*	X	Y	Z	[	\	]	^	_	*/
	"$B#X(B",	"$B#Y(B",	"$B#Z(B",	"$B!N(B",	"$B!o(B",	"$B!O(B",	"$B!0(B",	"$B!2(B",
/*	`	a	b	c	d	e	f	g	*/
	"$B!.(B",	"$B#a(B",	"$B#b(B",	"$B#c(B",	"$B#d(B",	"$B#e(B",	"$B#f(B",	"$B#g(B",
/*	h	i	j	k	l	m	n	o	*/
	"$B#h(B",	"$B#i(B",	"$B#j(B",	"$B#k(B",	"$B#l(B",	"$B#m(B",	"$B#n(B",	"$B#o(B",
/*	p	q	r	s	t	u	v	w	*/
	"$B#p(B",	"$B#q(B",	"$B#r(B",	"$B#s(B",	"$B#t(B",	"$B#u(B",	"$B#v(B",	"$B#w(B",
/*	x	y	z	{	|	}	~	DEL	*/
	"$B#x(B",	"$B#y(B",	"$B#z(B",	"$B!P(B",	"$B!C(B",	"$B!Q(B",	"$B!A(B",	"",
};
#ifndef KUTOUTEN /* By Y. Kaneko */
char *ExChars[] = {"$B!<(B","$B!V(B","$B!W(B"};
#else
char *ExChars[] = {"$B!<(B","$B!V(B","$B!W(B","$B!"(B","$B!#(B","$B!&(B"};
#endif

char LastConso[MAX_CONSO];  /* LastConso[0] is always '\0' */
char SmallTU;
short Kindex;
short Nconso;
static char *(*CurrentTab)[5] = HiraTab;

char *modeString[] = {
	"SKK",
	"$B$+$J(B",
	"$BA41Q(B",
	"$B%+%J(B",
	"$BC18lF~NO(B",
	"$BC18lA*Br(B",
};

void writeShells(),flushOut();

void
iKanaV(c)
char c;
{
	inputKanaVowel(c,writeShells);
}

void
iKanaC(c)
char c;
{
	inputKanaConso(c,writeShells,flushOut);
}


void
iZenAl(c)
{
	flushKana();
	inputZenkakuAlpha(c,writeShells);
}

void
iZenEx(c)
char c;
{
	flushKana();
	inputZenkakuEx(c,writeShells);
}

void
flthru(c)
char c;
{
	flushKana();
	thru(c);
}

flushKana()
{
	flushLastConso('\0',writeShells,flushOut);
}

inputKanaVowel(c,output)
char c;
void (*output)();
{
	int vowelnum;

	switch (c) {
	    case 'a': vowelnum = VOW_A; break;
	    case 'i': vowelnum = VOW_I; break;
	    case 'u': vowelnum = VOW_U; break;
	    case 'e': vowelnum = VOW_E; break;
	    case 'o': vowelnum = VOW_O; break;
	}
	if (Kindex != 0) { /* if preceding consonant exists */
		csrLeft(Nconso);
		erase(Nconso);
		csrLeft(Nconso);
	}
	if (SmallTU && vowelnum == VOW_U) {
		output(CurrentTab KANA_XTU);
	}
	else
		output(CurrentTab[Kindex][vowelnum]);
	SmallTU = 0;
	Kindex = 0;
	Nconso = 0;
}

inputKanaConso(c,output,flush)
char c;
void (*output)();
void (*flush)();
{
	char notOverwrite = 0;
	switch (c) {
	    case 'k': Kindex = CON_K; break;
	    case 's':
		if (LastConso[Nconso] == 't') {
			Kindex = CON_TS;
			notOverwrite = 1;
		}
		else
			Kindex = CON_S;
		break;
	    case 't':
		if (LastConso[Nconso] == 'x') {
			SmallTU = 1;
			notOverwrite = 1;
		}
		Kindex = CON_T;
		break;
	    case 'n':
		if (LastConso[Nconso] == 'n') {
			csrLeft(Nconso);
			output(CurrentTab KANA_NN);
			Nconso = 0;
			Kindex = 0;
			return;
		}
		Kindex = CON_N;
		break;
	    case 'h':
		switch (LastConso[Nconso]) {
		    case 'c':
			notOverwrite = 1;
			Kindex = CON_CH;
			break;
		    case 's':
			notOverwrite = 1;
			Kindex = CON_SH;
			break;
		    case 'd':
			notOverwrite = 1;
			Kindex = CON_DH;
			break;
		    case 't':
			notOverwrite = 1;
			Kindex = CON_TH;
			break;
		    default:
			Kindex = CON_H;
		}
		break;
	    case 'm': Kindex = CON_M; break;
	    case 'r': Kindex = CON_R; break;
	    case 'g': Kindex = CON_G; break;
	    case 'z': Kindex = CON_Z; break;
	    case 'd': Kindex = CON_D; break;
	    case 'b': Kindex = CON_B; break;
	    case 'p': Kindex = CON_P; break;
	    case 'x': Kindex = CON_X; break;
	    case 'y':
		if (LastConso[Nconso] && Kindex < CON_Y) {
			notOverwrite = 1;
			Kindex += (CON_Y+1);
		}
		else if (LastConso[Nconso] == 'j') {
			notOverwrite = 1;
			Kindex = CON_JY;
		}
		else
			Kindex = CON_Y;
		break;
	    case 'w':
		if (LastConso[Nconso] == 'x') {
			notOverwrite = 1;
			Kindex = CON_XW;
		}
		else
			Kindex = CON_W;
		break;
	    case 'j': Kindex = CON_J; break;
	    case 'f': Kindex = CON_F; break;
	    case 'v': Kindex = CON_V; break;
	}
	if (!notOverwrite) {
		flushLastConso(c,output,flush);
	}
	Nconso++;
	LastConso[Nconso] = c;
	write1(c);
}

flushLastConso(c,output,flush)
char c;
void (*output)(),(*flush)();
{
	if (Nconso == 0)
		return;
	if (Nconso <= 2)
		csrLeft(Nconso);
	else {
		csrLeft(Nconso);
		erase(Nconso);
		csrLeft(Nconso);
	}
	if (LastConso[Nconso] == 'n') {
		output(CurrentTab KANA_NN);
		if (flush != NULL)
			flush(2);
	}
	else if (LastConso[Nconso] == c) {
		output(CurrentTab KANA_XTU);
		if (flush != NULL)
			flush(2);
	}
	Nconso = 0;
}

void
toKana(c)
char c;
{
	setKeymap(&CurrentKeymap,KanaKeymap);
	if (CurrentTab == HiraTab)
		showmode(KANA_MODE);
	else
		showmode(KKANA_MODE);
}

void
tglK(c)
char c;
{
	flushKana();
	if (CurrentTab == HiraTab) {
		CurrentTab = KataTab;
		showmode(KKANA_MODE);
	}
	else {
		CurrentTab = HiraTab;
		showmode(KANA_MODE);
	}
}

cancelConso()
{
	rubout(Nconso);
	Nconso = 0;
	Kindex = 0;
	SmallTU = 0;
}
	
void
kanaBS(c)
char c;
{
	int i,n;
	char con[MAX_CONSO];
	
	if (Nconso) {
		n = Nconso;
		for (i = 1; i < Nconso; i++)
			con[i] = LastConso[i];
		cancelConso();
		for (i = 1; i < n; i++)
			iKanaC(con[i]);
	}
	else {
#ifdef KANJIBS
		if (KanjiBS)
			writeShell1(c);
		else
#endif
			thru(c);
	}
}

inputZenkakuAlpha(c,output)
char c;
void (*output)();
{
	output(ZenkakuAlpha[c-' ']);
}

inputZenkakuEx(c,output)
char c;
void (*output)();
{
	switch (c) {
	    case '-':
		output(ExChars[0]);
		break;
	    case '[':
		output(ExChars[1]);
		break;
	    case ']':
		output(ExChars[2]);
		break;
#ifdef KUTOUTEN
	    case ',':
		output(ExChars[3]);
		break;
	    case '.':
		output(ExChars[4]);
		break;
	    case '/':
		output(ExChars[5]);
		break;
#endif
	}
}

#define COLBASE	18
char codebuf[5];
int codecol;

codeinMsg()
{
	toMsg();
	writes("JIS or EUC code: ");
}

void
inputCode()
{
	flushKana();
	if (!msgLine())
		return;
	codeinMsg();
	setKeymap(&CurrentKeymap,convertKeymap(&CodeInputKeymap));
	codecol = 0;
}
	
void
cancelCode()
{
	setKeymap(&CurrentKeymap,KanaKeymap);
	showmode(KANA_MODE);
}

void
codein(c)
char c;
{
	if (codecol == 4) {
		codecol = 0;
		codeinMsg();
	}
	write1(c);
	codebuf[codecol] = tolower(c);
	codecol++;
}

#define HEX1(x)	((x)>'9'?((x)-'a'+10):((x)-'0'))
#define HEX2INT(a,b)	((HEX1(a)<<4)+HEX1(b))

void
enterCode()
{
	char kbuf[3];

	fromMsg();
	if (codecol == 4) {
		kbuf[0] = (HEX2INT(codebuf[0],codebuf[1]) | 0x80);
		kbuf[1] = (HEX2INT(codebuf[2],codebuf[3]) | 0x80);
		kbuf[2] = '\0';
		writeShells(kbuf);
	}
	cancelCode();
}

void
hira2kata(buf)
char *buf;
{
    int i,j,k;
    i = 0;
    while (buf[i]) {
	if (buf[0] & 0x80) {
	    for (j = 0; j <= CON_NN; j++) {
		for (k = 0; k < 5; k++) {
		    if (!strncmp(&buf[i],HiraTab[j][k],2)) {
			strncpy(&buf[i],KataTab[j][k],2);
			goto brk1;
		    }
		}
	    }
	brk1: i += 2;
	}
	else
	    i++;
    }
}

