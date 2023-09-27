#include "romkan.h"
#include "config.h"
#include "etc.h"
#include "fep.h"
#include "keybind.h"
#include "readwrite.h"
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
  { "\244\242",
    "\244\244",
    "\244\246",
    "\244\250",
    "\244\252" }, /* vowel = 0 */
  { "\244\253", "\244\255", "\244\257", "\244\261", "\244\263" }, /* k = 1 */
  { "\244\265", "\244\267", "\244\271", "\244\273", "\244\275" }, /* s = 2 */
  { "\244\277", "\244\301", "\244\304", "\244\306", "\244\310" }, /* t = 3 */
  { "\244\312", "\244\313", "\244\314", "\244\315", "\244\316" }, /* n = 4 */
  { "\244\317", "\244\322", "\244\325", "\244\330", "\244\333" }, /* h = 5 */
  { "\244\336", "\244\337", "\244\340", "\244\341", "\244\342" }, /* m = 6 */
  { "\244\351", "\244\352", "\244\353", "\244\354", "\244\355" }, /* r = 7 */
  { "\244\254", "\244\256", "\244\260", "\244\262", "\244\264" }, /* g = 8 */
  { "\244\266", "\244\270", "\244\272", "\244\274", "\244\276" }, /* z = 9 */
  { "\244\300", "\244\302", "\244\305", "\244\307", "\244\311" }, /* d = 10 */
  { "\244\320", "\244\323", "\244\326", "\244\331", "\244\334" }, /* b = 11 */
  { "\244\321", "\244\324", "\244\327", "\244\332", "\244\335" }, /* p = 12 */
  { "\244\241", "\244\243", "\244\245", "\244\247", "\244\251" }, /* x = 13 */
  { "\244\344",
    "\244\244",
    "\244\346",
    "\244\244\244\247",
    "\244\350" }, /* y = 14 */
  { "\244\357",
    "\244\246\244\243",
    "\244\246",
    "\244\246\244\247",
    "\244\362" }, /* w = 15 */
  { "\244\255\244\343",
    "\244\255\244\243",
    "\244\255\244\345",
    "\244\255\244\247",
    "\244\255\244\347" }, /* ky = 16 */
  { "\244\267\244\343",
    "\244\267\244\243",
    "\244\267\244\345",
    "\244\267\244\247",
    "\244\267\244\347" }, /* sy = 17 */
  { "\244\301\244\343",
    "\244\301\244\243",
    "\244\301\244\345",
    "\244\301\244\247",
    "\244\301\244\347" }, /* ty = 18 */
  { "\244\313\244\343",
    "\244\313\244\243",
    "\244\313\244\345",
    "\244\313\244\247",
    "\244\313\244\347" }, /* ny = 19 */
  { "\244\322\244\343",
    "\244\322\244\243",
    "\244\322\244\345",
    "\244\322\244\247",
    "\244\322\244\347" }, /* hy = 20 */
  { "\244\337\244\343",
    "\244\337\244\243",
    "\244\337\244\345",
    "\244\337\244\247",
    "\244\337\244\347" }, /* my = 21 */
  { "\244\352\244\343",
    "\244\352\244\243",
    "\244\352\244\345",
    "\244\352\244\247",
    "\244\352\244\347" }, /* ry = 22 */
  { "\244\256\244\343",
    "\244\256\244\243",
    "\244\256\244\345",
    "\244\256\244\247",
    "\244\256\244\347" }, /* gy = 23 */
  { "\244\270\244\343",
    "\244\270\244\243",
    "\244\270\244\345",
    "\244\270\244\247",
    "\244\270\244\347" }, /* zy = 24 */
  { "\244\302\244\343",
    "\244\302\244\243",
    "\244\302\244\345",
    "\244\302\244\247",
    "\244\302\244\347" }, /* dy = 25 */
  { "\244\323\244\343",
    "\244\323\244\243",
    "\244\323\244\345",
    "\244\323\244\247",
    "\244\323\244\347" }, /* by = 26 */
  { "\244\324\244\343",
    "\244\324\244\243",
    "\244\324\244\345",
    "\244\324\244\247",
    "\244\324\244\347" },                                         /* py = 27 */
  { "\244\343", "\244\243", "\244\345", "\244\247", "\244\347" }, /* xy = 28 */
  { "\244\301\244\343",
    "\244\301",
    "\244\301\244\345",
    "\244\301\244\247",
    "\244\301\244\347" }, /* ch = 29 */
  { "\244\270\244\343",
    "\244\270",
    "\244\270\244\345",
    "\244\270\244\247",
    "\244\270\244\347" }, /* j  = 30 */
  { "\244\267\244\343",
    "\244\267",
    "\244\267\244\345",
    "\244\267\244\247",
    "\244\267\244\347" }, /* sh = 31 */
  { "\244\304\244\241",
    "\244\304\244\243",
    "\244\304",
    "\244\304\244\247",
    "\244\304\244\251" },                                         /* ts = 32 */
  { "\244\356", "\244\243", "\244\245", "\244\247", "\244\251" }, /* xw = 33 */
  { "\244\325\244\241",
    "\244\325\244\243",
    "\244\325",
    "\244\325\244\247",
    "\244\325\244\251" },                                         /* f  = 34 */
  { "\244\320", "\244\323", "\244\326", "\244\331", "\244\334" }, /* v  = 35 */
  { "\244\300\244\241",
    "\244\307\244\243",
    "\244\311\244\245",
    "\244\307\244\247",
    "\244\311\244\251" }, /* dh = 36 */
  { "\244\270\244\343",
    "\244\270\244\243",
    "\244\270\244\345",
    "\244\270\244\250",
    "\244\270\244\347" }, /* jy = 37 */
  { "\244\306\244\343",
    "\244\306\244\243",
    "\244\306\244\345",
    "\244\306\244\247",
    "\244\306\244\347" },                                 /* th = 38 */
  { "\244\363", "\244\303", "\244\360", "\244\361", "" }, /* others = 39 */
};

const char* KataTab[][5] = {
  { "\245\242", "\245\244", "\245\246", "\245\250", "\245\252" },
  { "\245\253", "\245\255", "\245\257", "\245\261", "\245\263" },
  { "\245\265", "\245\267", "\245\271", "\245\273", "\245\275" },
  { "\245\277", "\245\301", "\245\304", "\245\306", "\245\310" },
  { "\245\312", "\245\313", "\245\314", "\245\315", "\245\316" },
  { "\245\317", "\245\322", "\245\325", "\245\330", "\245\333" },
  { "\245\336", "\245\337", "\245\340", "\245\341", "\245\342" },
  { "\245\351", "\245\352", "\245\353", "\245\354", "\245\355" },
  { "\245\254", "\245\256", "\245\260", "\245\262", "\245\264" },
  { "\245\266", "\245\270", "\245\272", "\245\274", "\245\276" },
  { "\245\300", "\245\302", "\245\305", "\245\307", "\245\311" },
  { "\245\320", "\245\323", "\245\326", "\245\331", "\245\334" },
  { "\245\321", "\245\324", "\245\327", "\245\332", "\245\335" },
  { "\245\241", "\245\243", "\245\245", "\245\247", "\245\251" },
  { "\245\344", "\245\244", "\245\346", "\245\244\245\247", "\245\350" },
  { "\245\357",
    "\245\246\245\243",
    "\245\246",
    "\245\246\245\247",
    "\245\362" },
  { "\245\255\245\343",
    "\245\255\245\243",
    "\245\255\245\345",
    "\245\255\245\247",
    "\245\255\245\347" },
  { "\245\267\245\343",
    "\245\267\245\243",
    "\245\267\245\345",
    "\245\267\245\247",
    "\245\267\245\347" },
  { "\245\301\245\343",
    "\245\301\245\243",
    "\245\301\245\345",
    "\245\301\245\247",
    "\245\301\245\347" },
  { "\245\313\245\343",
    "\245\313\245\243",
    "\245\313\245\345",
    "\245\313\245\247",
    "\245\313\245\347" },
  { "\245\322\245\343",
    "\245\322\245\243",
    "\245\322\245\345",
    "\245\322\245\247",
    "\245\322\245\347" },
  { "\245\337\245\343",
    "\245\337\245\243",
    "\245\337\245\345",
    "\245\337\245\247",
    "\245\337\245\347" },
  { "\245\352\245\343",
    "\245\352\245\243",
    "\245\352\245\345",
    "\245\352\245\247",
    "\245\352\245\347" },
  { "\245\256\245\343",
    "\245\256\245\243",
    "\245\256\245\345",
    "\245\256\245\247",
    "\245\256\245\347" },
  { "\245\270\245\343",
    "\245\270\245\243",
    "\245\270\245\345",
    "\245\270\245\247",
    "\245\270\245\347" },
  { "\245\302\245\343",
    "\245\302\245\243",
    "\245\302\245\345",
    "\245\302\245\247",
    "\245\302\245\347" },
  { "\245\323\245\343",
    "\245\323\245\243",
    "\245\323\245\345",
    "\245\323\245\247",
    "\245\323\245\347" },
  { "\245\324\245\343",
    "\245\324\245\243",
    "\245\324\245\345",
    "\245\324\245\247",
    "\245\324\245\347" },
  { "\245\343", "\245\243", "\245\345", "\245\247", "\245\347" },
  { "\245\301\245\343",
    "\245\301",
    "\245\301\245\345",
    "\245\301\245\247",
    "\245\301\245\347" },
  { "\245\270\245\343",
    "\245\270",
    "\245\270\245\345",
    "\245\270\245\247",
    "\245\270\245\347" },
  { "\245\267\245\343",
    "\245\267",
    "\245\267\245\345",
    "\245\267\245\247",
    "\245\267\245\347" },
  { "\245\304\245\241",
    "\245\304\245\243",
    "\245\304",
    "\245\304\245\247",
    "\245\304\245\251" },
  { "\245\356", "\245\243", "\245\245", "\245\247", "\245\251" },
  { "\245\325\245\241",
    "\245\325\245\243",
    "\245\325",
    "\245\325\245\247",
    "\245\325\245\251" },
  { "\245\364\245\241",
    "\245\364\245\243",
    "\245\364",
    "\245\364\245\247",
    "\245\364\245\251" },
  { "\245\300\245\241",
    "\245\307\245\243",
    "\245\311\245\245",
    "\245\307\245\247",
    "\245\311\245\251" },
  { "\245\270\245\343",
    "\245\270\245\243",
    "\245\270\245\345",
    "\245\270\245\247",
    "\245\270\245\347" },
  { "\245\306\245\343",
    "\245\306\245\243",
    "\245\306\245\345",
    "\245\306\245\247",
    "\245\306\245\347" },
  { "\245\363", "\245\303", "\245\360", "\245\361", "" },
};

char* ZenkakuAlpha[] = {
  /*	SPC	!	"	#	$	%	&	'	*/
  "\241\241",
  "\241\252",
  "\241\311",
  "\241\364",
  "\241\360",
  "\241\363",
  "\241\365",
  "\241\307",
  /*	(	)	*	+	,	-	.	/	*/
  "\241\312",
  "\241\313",
  "\241\366",
  "\241\334",
  "\241\244",
  "\241\335",
  "\241\245",
  "\241\277",
  /*	0	1	2	3	4	5	6	7	*/
  "\243\260",
  "\243\261",
  "\243\262",
  "\243\263",
  "\243\264",
  "\243\265",
  "\243\266",
  "\243\267",
  /*	8	9	:	;	<	=	>	?	*/
  "\243\270",
  "\243\271",
  "\241\247",
  "\241\250",
  "\241\343",
  "\241\341",
  "\241\344",
  "\241\251",
  /*	@	A	B	C	D	E	F	G	*/
  "\241\367",
  "\243\301",
  "\243\302",
  "\243\303",
  "\243\304",
  "\243\305",
  "\243\306",
  "\243\307",
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
  "\243\341",
  "\243\342",
  "\243\343",
  "\243\344",
  "\243\345",
  "\243\346",
  "\243\347",
  /*	h	i	j	k	l	m	n	o	*/
  "\243\350",
  "\243\351",
  "\243\352",
  "\243\353",
  "\243\354",
  "\243\355",
  "\243\356",
  "\243\357",
  /*	p	q	r	s	t	u	v	w	*/
  "\243\360",
  "\243\361",
  "\243\362",
  "\243\363",
  "\243\364",
  "\243\365",
  "\243\366",
  "\243\367",
  /*	x	y	z	{	|	}	~	DEL	*/
  "\243\370",
  "\243\371",
  "\243\372",
  "\241\320",
  "\241\303",
  "\241\321",
  "\241\301",
  "",
};
#ifndef KUTOUTEN /* By Y. Kaneko */
char* ExChars[] = { "\241\274", "\241\326", "\241\327" };
#else
char* ExChars[] = { "\241\274", "\241\326", "\241\327",
                    "\241\242", "\241\243", "\241\246" };
#endif

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
#ifdef KANJIBS
    if (KanjiBS)
      writeShell1(c);
    else
#endif
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
