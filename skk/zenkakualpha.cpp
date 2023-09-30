#include "zenkakualpha.h"
#include "connsh.h"
#include "romkan.h"

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

namespace zenkakualpha {

void
iZenAl(char c)
{
  romkan::flushKana();
  child::writeShells(inputZenkakuAlpha(c));
}

std::string
inputZenkakuAlpha(char c)

{
  return ZenkakuAlpha[c - ' '];
}

void
iZenEx(char c)
{
  romkan::flushKana();
  child::writeShells(inputZenkakuEx(c));
}

std::string
inputZenkakuEx(char c)
{
  switch (c) {
    case '-':
      return ExChars[0];
    case '[':
      return ExChars[1];
    case ']':
      return ExChars[2];
  }

  return "";
}

} // namespace
