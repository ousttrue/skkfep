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
	{"あ","い","う","え","お"},		/* vowel = 0 */
	{"か","き","く","け","こ"},		/* k = 1 */
	{"さ","し","す","せ","そ"},		/* s = 2 */
	{"た","ち","つ","て","と"},		/* t = 3 */
	{"な","に","ぬ","ね","の"},		/* n = 4 */
	{"は","ひ","ふ","へ","ほ"},		/* h = 5 */
	{"ま","み","む","め","も"},		/* m = 6 */
	{"ら","り","る","れ","ろ"},		/* r = 7 */
	{"が","ぎ","ぐ","げ","ご"},		/* g = 8 */
	{"ざ","じ","ず","ぜ","ぞ"},		/* z = 9 */
	{"だ","ぢ","づ","で","ど"},		/* d = 10 */
	{"ば","び","ぶ","べ","ぼ"},		/* b = 11 */
	{"ぱ","ぴ","ぷ","ぺ","ぽ"},		/* p = 12 */
	{"ぁ","ぃ","ぅ","ぇ","ぉ"},		/* x = 13 */
	{"や","い","ゆ","いぇ","よ"},		/* y = 14 */
	{"わ","うぃ","う","うぇ","を"},		/* w = 15 */
	{"きゃ","きぃ","きゅ","きぇ","きょ"},	/* ky = 16 */
	{"しゃ","しぃ","しゅ","しぇ","しょ"},	/* sy = 17 */
	{"ちゃ","ちぃ","ちゅ","ちぇ","ちょ"},	/* ty = 18 */
	{"にゃ","にぃ","にゅ","にぇ","にょ"},	/* ny = 19 */
	{"ひゃ","ひぃ","ひゅ","ひぇ","ひょ"},	/* hy = 20 */
	{"みゃ","みぃ","みゅ","みぇ","みょ"},	/* my = 21 */
	{"りゃ","りぃ","りゅ","りぇ","りょ"},	/* ry = 22 */
	{"ぎゃ","ぎぃ","ぎゅ","ぎぇ","ぎょ"},	/* gy = 23 */
	{"じゃ","じぃ","じゅ","じぇ","じょ"},	/* zy = 24 */
	{"ぢゃ","ぢぃ","ぢゅ","ぢぇ","ぢょ"},	/* dy = 25 */
	{"びゃ","びぃ","びゅ","びぇ","びょ"},	/* by = 26 */
	{"ぴゃ","ぴぃ","ぴゅ","ぴぇ","ぴょ"},	/* py = 27 */
	{"ゃ","ぃ","ゅ","ぇ","ょ"},		/* xy = 28 */
	{"ちゃ","ち","ちゅ","ちぇ","ちょ"},	/* ch = 29 */
	{"じゃ","じ","じゅ","じぇ","じょ"},	/* j  = 30 */
	{"しゃ","し","しゅ","しぇ","しょ"},	/* sh = 31 */
	{"つぁ","つぃ","つ","つぇ","つぉ"},	/* ts = 32 */
	{"ゎ","ぃ","ぅ","ぇ","ぉ"},		/* xw = 33 */
	{"ふぁ","ふぃ","ふ","ふぇ","ふぉ"},	/* f  = 34 */
	{"ば","び","ぶ","べ","ぼ"},		/* v  = 35 */
	{"だぁ","でぃ","どぅ","でぇ","どぉ"},	/* dh = 36 */
	{"じゃ","じぃ","じゅ","じえ","じょ"},	/* jy = 37 */
	{"てゃ","てぃ","てゅ","てぇ","てょ"},	/* th = 38 */
	{"ん","っ","ゐ","ゑ",""},		/* others = 39 */
};

char *KataTab[][5] = {
	{"ア","イ","ウ","エ","オ"},
	{"カ","キ","ク","ケ","コ"},
	{"サ","シ","ス","セ","ソ"},
	{"タ","チ","ツ","テ","ト"},
	{"ナ","ニ","ヌ","ネ","ノ"},
	{"ハ","ヒ","フ","ヘ","ホ"},
	{"マ","ミ","ム","メ","モ"},
	{"ラ","リ","ル","レ","ロ"},
	{"ガ","ギ","グ","ゲ","ゴ"},
	{"ザ","ジ","ズ","ゼ","ゾ"},
	{"ダ","ヂ","ヅ","デ","ド"},
	{"バ","ビ","ブ","ベ","ボ"},
	{"パ","ピ","プ","ペ","ポ"},
	{"ァ","ィ","ゥ","ェ","ォ"},
	{"ヤ","イ","ユ","イェ","ヨ"},
	{"ワ","ウィ","ウ","ウェ","ヲ"},
	{"キャ","キィ","キュ","キェ","キョ"},
	{"シャ","シィ","シュ","シェ","ショ"},
	{"チャ","チィ","チュ","チェ","チョ"},
	{"ニャ","ニィ","ニュ","ニェ","ニョ"},
	{"ヒャ","ヒィ","ヒュ","ヒェ","ヒョ"},
	{"ミャ","ミィ","ミュ","ミェ","ミョ"},
	{"リャ","リィ","リュ","リェ","リョ"},
	{"ギャ","ギィ","ギュ","ギェ","ギョ"},
	{"ジャ","ジィ","ジュ","ジェ","ジョ"},
	{"ヂャ","ヂィ","ヂュ","ヂェ","ヂョ"},
	{"ビャ","ビィ","ビュ","ビェ","ビョ"},
	{"ピャ","ピィ","ピュ","ピェ","ピョ"},
	{"ャ","ィ","ュ","ェ","ョ"},
	{"チャ","チ","チュ","チェ","チョ"},
	{"ジャ","ジ","ジュ","ジェ","ジョ"},
	{"シャ","シ","シュ","シェ","ショ"},
	{"ツァ","ツィ","ツ","ツェ","ツォ"},
	{"ヮ","ィ","ゥ","ェ","ォ"},
	{"ファ","フィ","フ","フェ","フォ"},
	{"ヴァ","ヴィ","ヴ","ヴェ","ヴォ"},
	{"ダァ","ディ","ドゥ","デェ","ドォ"},
	{"ジャ","ジィ","ジュ","ジェ","ジョ"},
	{"テャ","ティ","テュ","テェ","テョ"},
	{"ン","ッ","ヰ","ヱ",""},
};

char *ZenkakuAlpha[] = {
/*	SPC	!	"	#	$	%	&	'	*/
	"　",	"！",	"”",	"＃",	"＄",	"％",	"＆",	"’",
/*	(	)	*	+	,	-	.	/	*/
	"（",	"）",	"＊",	"＋",	"，",	"−",	"．",	"／",
/*	0	1	2	3	4	5	6	7	*/
	"０",	"１",	"２",	"３",	"４",	"５",	"６",	"７",
/*	8	9	:	;	<	=	>	?	*/
	"８",	"９",	"：",	"；",	"＜",	"＝",	"＞",	"？",
/*	@	A	B	C	D	E	F	G	*/
	"＠",	"Ａ",	"Ｂ",	"Ｃ",	"Ｄ",	"Ｅ",	"Ｆ",	"Ｇ",
/*	H	I	J	K	L	M	N	O	*/
	"Ｈ",	"Ｉ",	"Ｊ",	"Ｋ",	"Ｌ",	"Ｍ",	"Ｎ",	"Ｏ",
/*	P	Q	R	S	T	U	V	W	*/
	"Ｐ",	"Ｑ",	"Ｒ",	"Ｓ",	"Ｔ",	"Ｕ",	"Ｖ",	"Ｗ",
/*	X	Y	Z	[	\	]	^	_	*/
	"Ｘ",	"Ｙ",	"Ｚ",	"［",	"￥",	"］",	"＾",	"＿",
/*	`	a	b	c	d	e	f	g	*/
	"｀",	"ａ",	"ｂ",	"ｃ",	"ｄ",	"ｅ",	"ｆ",	"ｇ",
/*	h	i	j	k	l	m	n	o	*/
	"ｈ",	"ｉ",	"ｊ",	"ｋ",	"ｌ",	"ｍ",	"ｎ",	"ｏ",
/*	p	q	r	s	t	u	v	w	*/
	"ｐ",	"ｑ",	"ｒ",	"ｓ",	"ｔ",	"ｕ",	"ｖ",	"ｗ",
/*	x	y	z	{	|	}	~	DEL	*/
	"ｘ",	"ｙ",	"ｚ",	"｛",	"｜",	"｝",	"〜",	"",
};
#ifndef KUTOUTEN /* By Y. Kaneko */
char *ExChars[] = {"ー","「","」"};
#else
char *ExChars[] = {"ー","「","」","、","。","・"};
#endif

char LastConso[MAX_CONSO];  /* LastConso[0] is always '\0' */
char SmallTU;
short Kindex;
short Nconso;
static char *(*CurrentTab)[5] = HiraTab;

char *modeString[] = {
	"SKK",
	"かな",
	"全英",
	"カナ",
	"単語入力",
	"単語選択",
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

