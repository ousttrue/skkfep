#include "zenkaku.h"

const char* ZenkakuAlpha[] = {
  //	<SPC>	!	"	#	$	%	&	'
  "　",
  "！",
  "＂",
  "＃",
  "＄",
  "％",
  "＆",
  "＇",
  //	(	)	*	+	,	-	.	/
  "（",
  "）",
  "＊",
  "＋",
  "，",
  "－",
  "．",
  "／",
  //	0	1	2	3	4	5	6	7
  "０",
  "１",
  "２",
  "３",
  "４",
  "５",
  "６",
  "７",
  //	8	9	:	;	<	=	>	?
  "８",
  "９",
  "：",
  "；",
  "<",
  "＝",
  "＞",
  "？",
  //	@	A	B	C	D	E	F	G
  "＠",
  "Ａ",
  "Ｂ",
  "Ｃ",
  "Ｄ",
  "Ｅ",
  "Ｆ",
  "Ｇ",
  //	H	I	J	K	L	M	N	O
  "Ｈ",
  "Ｉ",
  "Ｊ",
  "Ｋ",
  "Ｌ",
  "Ｍ",
  "Ｎ",
  "Ｏ",
  //	P	Q	R	S	T	U	V	W
  "Ｐ",
  "Ｑ",
  "Ｒ",
  "Ｓ",
  "Ｔ",
  "Ｕ",
  "Ｖ",
  "Ｗ",
  //	X	Y	Z	[	\	]	^	_
  "Ｘ",
  "Ｙ",
  "Ｚ",
  "［",
  "＼",
  "］",
  "＾",
  "＿",
  //	`	a	b	c	d	e	f	g
  "｀",
  "ａ",
  "ｂ",
  "ｃ",
  "ｄ",
  "ｅ",
  "ｆ",
  "ｇ",
  //	h	i	j	k	l	m	n	o
  "ｈ",
  "ｉ",
  "ｊ",
  "ｋ",
  "ｌ",
  "ｍ",
  "ｎ",
  "ｏ",
  //	p	q	r	s	t	u	v	w
  "ｐ",
  "ｑ",
  "ｒ",
  "ｓ",
  "ｔ",
  "ｕ",
  "ｖ",
  "ｗ",
  //	x	y	z	{	|	}	~	<DEL>
  "ｘ",
  "ｙ",
  "ｚ",
  "｛",
  "｜",
  "｝",
  "～",
  "",
};
const char* ExChars[] = { "ー", "「", "」" };

inline const char*
zen(char8_t c)
{
  switch (c) {
    case '-':
      return ExChars[0];
    case '[':
      return ExChars[1];
    case ']':
      return ExChars[2];
    default:
      return ZenkakuAlpha[c - ' '];
  }
}

namespace skk {

ZenkakuInput::ZenkakuInput()
  : InputMode(InputType::Zenkaku)
{
}

Result
ZenkakuInput::putc(char8_t c)
{
  return { .Output{ .Confirmed = zen(c) } };
}

} // namespace
