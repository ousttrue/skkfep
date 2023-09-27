#include "romkan.h"
#include "config.h"
#include "connsh.h"
#include "etc.h"
#include "fep.h"
#include "keybind.h"
#include "stty.h"
#include "terms.h"
#include <ctype.h>
#include <string.h>

#define VOW_A 0
#define VOW_I 1
#define VOW_U 2
#define VOW_E 3
#define VOW_O 4

#define CON_0 0
#define CON_K 1
#define CON_S 2
#define CON_T 3
#define CON_N 4
#define CON_H 5
#define CON_M 6
#define CON_R 7
#define CON_G 8
#define CON_Z 9
#define CON_D 10
#define CON_B 11
#define CON_P 12
#define CON_X 13
#define CON_Y 14
#define CON_W 15
#define CON_KY 16
#define CON_SY 17
#define CON_TY 18
#define CON_NY 19
#define CON_HY 20
#define CON_MY 21
#define CON_RY 22
#define CON_GY 23
#define CON_ZY 24
#define CON_DY 25
#define CON_BY 26
#define CON_PY 27
#define CON_XY 28
#define CON_CH 29
#define CON_J 30
#define CON_SH 31
#define CON_TS 32
#define CON_XW 33
#define CON_F 34
#define CON_V 35
#define CON_DH 36
#define CON_JY 37
#define CON_TH 38
#define CON_NN 39

#define KANA_NN [CON_NN][0]
#define KANA_XTU [CON_NN][1]

static const char* HiraTab[][5] = {
  { "あ", "い", "う", "え", "お" },           /* vowel = 0 */
  { "か", "き", "く", "け", "こ" },           /* k = 1 */
  { "さ", "し", "す", "せ", "そ" },           /* s = 2 */
  { "た", "ち", "つ", "て", "と" },           /* t = 3 */
  { "な", "に", "ぬ", "ね", "の" },           /* n = 4 */
  { "は", "ひ", "ふ", "へ", "ほ" },           /* h = 5 */
  { "ま", "み", "む", "め", "も" },           /* m = 6 */
  { "ら", "り", "る", "れ", "ろ" },           /* r = 7 */
  { "が", "ぎ", "ぐ", "げ", "ご" },           /* g = 8 */
  { "さ", "じ", "ず", "せ", "ぞ" },           /* z = 9 */
  { "だ", "ぢ", "づ", "で", "ど" },           /* d = 10 */
  { "ば", "び", "ぶ", "べ", "ぼ" },           /* b = 11 */
  { "ぱ", "ぴ", "ぷ", "ぺ", "ぽ" },           /* p = 12 */
  { "ぁ", "ぃ", "ぅ", "ぇ", "ぉ" },           /* x = 13 */
  { "や", "い", "ゆ", "ぃえ", "よ" },         /* y = 14 */
  { "わ", "うぃ", "う", "うぇ", "を" },       /* w = 15 */
  { "きゃ", "きぃ", "きゅ", "きぇ", "きょ" }, /* ky = 16 */
  { "しゃ", "しぃ", "しゅ", "しぇ", "しょ" }, /* sy = 17 */
  { "ちゃ", "ちぃ", "ちゅ", "ちぇ", "ちょ" }, /* ty = 18 */
  { "にゃ", "にぃ", "にゅ", "にぇ", "にょ" }, /* ny = 19 */
  { "ひゃ", "ひぃ", "ひゅ", "ひぇ", "ひょ" }, /* hy = 20 */
  { "みゃ", "みぃ", "みゅ", "みぇ", "みょ" }, /* my = 21 */
  { "りゃ", "りぃ", "りゅ", "りぇ", "りょ" }, /* ry = 22 */
  { "ぎゃ", "ぎぃ", "ぎゅ", "ぎぇ", "ぎょ" }, /* gy = 23 */
  { "じゃ", "じぃ", "じゅ", "じぇ", "じょ" }, /* zy = 24 */
  { "ぢゃ", "ぢぃ", "ぢゅ", "ぢぇ", "ぢょ" }, /* dy = 25 */
  { "びゃ", "びぃ", "びゅ", "びぇ", "びょ" }, /* by = 26 */
  { "ぴゃ", "ぴぃ", "ぴゅ", "ぴぇ", "ぴょ" }, /* py = 27 */
  { "ゃ", "ぃ", "ゅ", "ぇ", "ょ" },           /* xy = 28 */
  { "ちゃ", "ち", "ちゅ", "ちぇ", "ちょ" },   /* ch = 29 */
  { "じゃ", "じ", "じゅ", "じぇ", "じょ" },   /* j  = 30 */
  { "しゃ", "し", "しゅ", "しぇ", "しょ" },   /* sh = 31 */
  { "つぁ", "つぃ", "つ", "つぇ", "つぉ" },   /* ts = 32 */
  { "ゎ", "ぃ", "ぅ", "ぇ", "ぉ" },           /* xw = 33 */
  { "ふぁ", "ふぃ", "ふ", "ふぇ", "ふぉ" },   /* f  = 34 */
  { "ば", "び", "ぶ", "べ", "ぼ" },           /* v  = 35 */
  { "だぁ", "でぃ", "どぅ", "でぇ", "どぉ" }, /* dh = 36 */
  { "じゃ", "じぃ", "じゅ", "じぇ", "じょ" }, /* jy = 37 */
  { "てゃ", "てぃ", "てゅ", "てぇ", "てょ" }, /* th = 38 */
  { "ん", "っ", "ゐ", "ゑ", "" },             /* others = 39 */
};

const char* KataTab[][5] = {
  { "ア", "イ", "ウ", "エ", "オ" },           /* vowel = 0 */
  { "カ", "キ", "ク", "ケ", "コ" },           /* k = 1 */
  { "サ", "シ", "ス", "セ", "ソ" },           /* s = 2 */
  { "タ", "チ", "ツ", "テ", "ト" },           /* t = 3 */
  { "ナ", "ニ", "ヌ", "ネ", "ノ" },           /* n = 4 */
  { "ハ", "ヒ", "フ", "ヘ", "ホ" },           /* h = 5 */
  { "マ", "ミ", "ム", "メ", "モ" },           /* m = 6 */
  { "ラ", "リ", "ル", "レ", "ロ" },           /* r = 7 */
  { "ガ", "ギ", "グ", "ゲ", "ゴ" },           /* g = 8 */
  { "サ", "ジ", "ズ", "セ", "ゾ" },           /* z = 9 */
  { "ダ", "ヂ", "ヅ", "デ", "ド" },           /* d = 10 */
  { "バ", "ビ", "ブ", "ベ", "ボ" },           /* b = 11 */
  { "パ", "ピ", "プ", "ペ", "ポ" },           /* p = 12 */
  { "ァ", "ィ", "ゥ", "ェ", "ォ" },           /* x = 13 */
  { "ヤ", "イ", "ユ", "ィエ", "ヨ" },         /* y = 14 */
  { "ワ", "ウィ", "ウ", "ウェ", "ヲ" },       /* w = 15 */
  { "キャ", "キィ", "キュ", "キェ", "キョ" }, /* ky = 16 */
  { "シャ", "シィ", "シュ", "シェ", "ショ" }, /* sy = 17 */
  { "チャ", "チィ", "チュ", "チェ", "チョ" }, /* ty = 18 */
  { "ニャ", "ニィ", "ニュ", "ニェ", "ニョ" }, /* ny = 19 */
  { "ヒャ", "ヒィ", "ヒュ", "ヒェ", "ヒョ" }, /* hy = 20 */
  { "ミャ", "ミィ", "ミュ", "ミェ", "ミョ" }, /* my = 21 */
  { "リャ", "リィ", "リュ", "リェ", "リョ" }, /* ry = 22 */
  { "ギャ", "ギィ", "ギュ", "ギェ", "ギョ" }, /* gy = 23 */
  { "ジャ", "ジィ", "ジュ", "ジェ", "ジョ" }, /* zy = 24 */
  { "ヂャ", "ヂィ", "ヂュ", "ヂェ", "ヂョ" }, /* dy = 25 */
  { "ビャ", "ビィ", "ビュ", "ビェ", "ビョ" }, /* by = 26 */
  { "ピャ", "ピィ", "ピュ", "ピェ", "ピョ" }, /* py = 27 */
  { "ャ", "ィ", "ュ", "ェ", "ョ" },           /* xy = 28 */
  { "チャ", "チ", "チュ", "チェ", "チョ" },   /* ch = 29 */
  { "ジャ", "ジ", "ジュ", "ジェ", "ジョ" },   /* j  = 30 */
  { "シャ", "シ", "シュ", "シェ", "ショ" },   /* sh = 31 */
  { "ツァ", "ツィ", "ツ", "ツェ", "ツォ" },   /* ts = 32 */
  { "ヮ", "ィ", "ゥ", "ェ", "ォ" },           /* xw = 33 */
  { "ファ", "フィ", "フ", "フェ", "フォ" },   /* f  = 34 */
  { "ヴァ", "ヴィ", "ヴ", "ヴェ", "ヴォ" },   /* v  = 35 */
  { "ダァ", "ディ", "ドゥ", "デェ", "ドォ" }, /* dh = 36 */
  { "ジャ", "ジィ", "ジュ", "ジェ", "ジョ" }, /* jy = 37 */
  { "テャ", "ティ", "テュ", "テェ", "テョ" }, /* th = 38 */
  { "ン", "ッ", "ヰ", "ヱ", "" },             /* others = 39 */
};

const char* ZenkakuAlpha[] = {
  /*	SPC	!	"	#	$	%	&	'	*/
  "　",
  "！",
  "",
  "＃",
  "＄",
  "％",
  "＆",
  "",
  /*	(	)	*	+	,	-	.	/	*/
  "（",
  "）",
  "＊",
  "＋",
  "、",
  "ー",
  "。",
  "／",
  /*	0	1	2	3	4	5	6	7	*/
  "０",
  "０",
  "０",
  "０",
  "０",
  "０",
  "０",
  "０",
  /*	8	9	:	;	<	=	>	?	*/
  "８",
  "８",
  "８",
  "８",
  "８",
  "８",
  "８",
  "８",
  /*	@	A	B	C	D	E	F	G	*/
  "A",
  "A",
  "A",
  "A",
  "A",
  "A",
  "A",
  "A",
  /*	H	I	J	K	L	M	N	O	*/
  "\243\310",
  "\243\311",
  "\243\312",
  "\243\313",
  "\243\314",
  "\243\315",
  "\243\316",
  "\243\317",
  /*	P	Q	R	S	T	U	V	W	*/
  "\243\320",
  "\243\321",
  "\243\322",
  "\243\323",
  "\243\324",
  "\243\325",
  "\243\326",
  "\243\327",
  /*	X	Y	Z	[	\	]	^	_	*/
  "\243\330",
  "\243\331",
  "\243\332",
  "\241\316",
  "\241\357",
  "\241\317",
  "\241\260",
  "\241\262",
  /*	`	a	b	c	d	e	f	g	*/
  "\241\256",
  "ａ",
  "ｂ",
  "ｃ",
  "ｄ",
  "",
  "ｆ",
  "ｇ",
  /*	h	i	j	k	l	m	n	o	*/
  "ｘ",
  "ｘ",
  "ｘ",
  "ｘ",
  "ｘ",
  "ｘ",
  "ｘ",
  "ｘ",
  /*	p	q	r	s	t	u	v	w	*/
  "ｙ",
  "ｙ",
  "ｙ",
  "ｙ",
  "ｙ",
  "ｙ",
  "ｙ",
  "ｙ",
  /*	x	y	z	{	|	}	~	DEL	*/
  "ｘ",
  "ｙ",
  "ｚ",
  "｛",
  "｜",
  "｝",
  "〜",
  "",
};
const char* ExChars[] = { "ー", "「", "」" };

char LastConso[MAX_CONSO]; /* LastConso[0] is always '\0' */
char SmallTU;
short Kindex;
short Nconso;
static const char* (*CurrentTab)[5] = HiraTab;

namespace romkan {
void
iKanaV(char c)
{
  inputKanaVowel(c, writeShells);
}

void
iKanaC(char c)
{
  inputKanaConso(c, writeShells, flushOut);
}

void
iZenAl(char c)
{
  flushKana();
  inputZenkakuAlpha(c, writeShells);
}

void
iZenEx(char c)
{
  flushKana();
  inputZenkakuEx(c, writeShells);
}

void
flthru(char c)
{
  flushKana();
  thru(c);
}

void
flushKana()
{
  flushLastConso('\0', writeShells, flushOut);
}

void
inputKanaVowel(char c, void (*output)(const char*))
{
  int vowelnum;

  switch (c) {
    case 'a':
      vowelnum = VOW_A;
      break;
    case 'i':
      vowelnum = VOW_I;
      break;
    case 'u':
      vowelnum = VOW_U;
      break;
    case 'e':
      vowelnum = VOW_E;
      break;
    case 'o':
      vowelnum = VOW_O;
      break;
  }
  if (Kindex != 0) { /* if preceding consonant exists */
    csrLeft(Nconso);
    erase(Nconso);
    csrLeft(Nconso);
  }
  if (SmallTU && vowelnum == VOW_U) {
    output(CurrentTab KANA_XTU);
  } else
    output(CurrentTab[Kindex][vowelnum]);
  SmallTU = 0;
  Kindex = 0;
  Nconso = 0;
}

void
inputKanaConso(char c, void (*output)(const char*), void (*flush)(int))

{
  char notOverwrite = 0;
  switch (c) {
    case 'k':
      Kindex = CON_K;
      break;
    case 's':
      if (LastConso[Nconso] == 't') {
        Kindex = CON_TS;
        notOverwrite = 1;
      } else
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
    case 'm':
      Kindex = CON_M;
      break;
    case 'r':
      Kindex = CON_R;
      break;
    case 'g':
      Kindex = CON_G;
      break;
    case 'z':
      Kindex = CON_Z;
      break;
    case 'd':
      Kindex = CON_D;
      break;
    case 'b':
      Kindex = CON_B;
      break;
    case 'p':
      Kindex = CON_P;
      break;
    case 'x':
      Kindex = CON_X;
      break;
    case 'y':
      if (LastConso[Nconso] && Kindex < CON_Y) {
        notOverwrite = 1;
        Kindex += (CON_Y + 1);
      } else if (LastConso[Nconso] == 'j') {
        notOverwrite = 1;
        Kindex = CON_JY;
      } else
        Kindex = CON_Y;
      break;
    case 'w':
      if (LastConso[Nconso] == 'x') {
        notOverwrite = 1;
        Kindex = CON_XW;
      } else
        Kindex = CON_W;
      break;
    case 'j':
      Kindex = CON_J;
      break;
    case 'f':
      Kindex = CON_F;
      break;
    case 'v':
      Kindex = CON_V;
      break;
  }
  if (!notOverwrite) {
    flushLastConso(c, output, flush);
  }
  Nconso++;
  LastConso[Nconso] = c;
  write1(c);
}

void
flushLastConso(char c, void (*output)(const char*), void (*flush)(int))
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
  } else if (LastConso[Nconso] == c) {
    output(CurrentTab KANA_XTU);
    if (flush != NULL)
      flush(2);
  }
  Nconso = 0;
}

void
toKana(char c)
{
  setKeymap(&CurrentKeymap, KanaKeymap);
  if (CurrentTab == HiraTab)
    showmode(KANA_MODE);
  else
    showmode(KKANA_MODE);
}

void
tglK(char c)
{
  flushKana();
  if (CurrentTab == HiraTab) {
    CurrentTab = KataTab;
    showmode(KKANA_MODE);
  } else {
    CurrentTab = HiraTab;
    showmode(KANA_MODE);
  }
}

void
cancelConso()
{
  rubout(Nconso);
  Nconso = 0;
  Kindex = 0;
  SmallTU = 0;
}

void
kanaBS(char c)
{
  int i, n;
  char con[MAX_CONSO];

  if (Nconso) {
    n = Nconso;
    for (i = 1; i < Nconso; i++)
      con[i] = LastConso[i];
    cancelConso();
    for (i = 1; i < n; i++)
      iKanaC(con[i]);
  } else {
    thru(c);
  }
}

void
inputZenkakuAlpha(char c, void (*output)(const char*))

{
  output(ZenkakuAlpha[c - ' ']);
}

void
inputZenkakuEx(char c, void (*output)(const char*))
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
  }
}

#define COLBASE 18
char codebuf[5];
int codecol;

void
codeinMsg()
{
  toMsg();
  writes("JIS or EUC code: ");
}

void
inputCode(char)
{
  flushKana();
  if (!msgLine())
    return;
  codeinMsg();
  setKeymap(&CurrentKeymap, convertKeymap(&CodeInputKeymap));
  codecol = 0;
}

void
cancelCode(char)
{
  setKeymap(&CurrentKeymap, KanaKeymap);
  showmode(KANA_MODE);
}

void
codein(char c)
{
  if (codecol == 4) {
    codecol = 0;
    codeinMsg();
  }
  write1(c);
  codebuf[codecol] = tolower(c);
  codecol++;
}

#define HEX1(x) ((x) > '9' ? ((x) - 'a' + 10) : ((x) - '0'))
#define HEX2INT(a, b) ((HEX1(a) << 4) + HEX1(b))

void
enterCode(char)
{
  char kbuf[3];

  fromMsg();
  if (codecol == 4) {
    kbuf[0] = (HEX2INT(codebuf[0], codebuf[1]) | 0x80);
    kbuf[1] = (HEX2INT(codebuf[2], codebuf[3]) | 0x80);
    kbuf[2] = '\0';
    writeShells(kbuf);
  }
  cancelCode({});
}

void
hira2kata(char* buf)
{
  int i, j, k;
  i = 0;
  while (buf[i]) {
    if (buf[0] & 0x80) {
      for (j = 0; j <= CON_NN; j++) {
        for (k = 0; k < 5; k++) {
          if (!strncmp(&buf[i], HiraTab[j][k], 2)) {
            strncpy(&buf[i], KataTab[j][k], 2);
            goto brk1;
          }
        }
      }
    brk1:
      i += 2;
    } else
      i++;
  }
}

} // namespace
