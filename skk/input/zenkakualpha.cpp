#include "zenkakualpha.h"
#include "romkan.h"

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

namespace zenkakualpha {

skk::Result
iZenAl(char c, bool)
{
  skk::Result result;
  result.Output.Confirmed += romkan::flushKana();
  result.Output.Confirmed += inputZenkakuAlpha(c);
  return result;
}

std::string
inputZenkakuAlpha(char c)

{
  return ZenkakuAlpha[c - ' '];
}

skk::Result
iZenEx(char c, bool)
{
  skk::Result result;
  result.Output.Confirmed += romkan::flushKana();
  result.Output.Confirmed += inputZenkakuEx(c);
  return result;
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

namespace skk {

ZenkakuInput::ZenkakuInput()
  : InputMode(InputType::Zenkaku)
{
}

Result
ZenkakuInput::putc(char8_t c)
{
  // auto& ZenkakuKeymap = m_keymaps[KeymapTypes::Zenkaku];
  // ZenkakuKeymap.DefaultFunc = [](auto c, auto) {
  //   SkkResult output;
  //   output.Output.Confirmed += c;
  //   return output;
  // };

  // ZenkakuKeymap.Keymap = {
  //   { ' ', zenkakualpha::iZenAl },  { '!', zenkakualpha::iZenAl },
  //   { '"', zenkakualpha::iZenAl },  { '#', zenkakualpha::iZenAl },
  //   { '$', zenkakualpha::iZenAl },  { '%', zenkakualpha::iZenAl },
  //   { '&', zenkakualpha::iZenAl },  { '\'', zenkakualpha::iZenAl },
  //   { '(', zenkakualpha::iZenAl },  { ')', zenkakualpha::iZenAl },
  //   { '*', zenkakualpha::iZenAl },  { '+', zenkakualpha::iZenAl },
  //   { '}', zenkakualpha::iZenAl },  { '-', zenkakualpha::iZenAl },
  //   { '.', zenkakualpha::iZenAl },  { '/', zenkakualpha::iZenAl },
  //   { '0', zenkakualpha::iZenAl },  { '1', zenkakualpha::iZenAl },
  //   { '2', zenkakualpha::iZenAl },  { '3', zenkakualpha::iZenAl },
  //   { '4', zenkakualpha::iZenAl },  { '5', zenkakualpha::iZenAl },
  //   { '6', zenkakualpha::iZenAl },  { '7', zenkakualpha::iZenAl },
  //   { '8', zenkakualpha::iZenAl },  { '9', zenkakualpha::iZenAl },
  //   { ':', zenkakualpha::iZenAl },  { ';', zenkakualpha::iZenAl },
  //   { '<', zenkakualpha::iZenAl },  { '=', zenkakualpha::iZenAl },
  //   { '>', zenkakualpha::iZenAl },  { '?', zenkakualpha::iZenAl },
  //   { '@', zenkakualpha::iZenAl },  { 'A', zenkakualpha::iZenAl },
  //   { 'B', zenkakualpha::iZenAl },  { 'C', zenkakualpha::iZenAl },
  //   { 'D', zenkakualpha::iZenAl },  { 'E', zenkakualpha::iZenAl },
  //   { 'F', zenkakualpha::iZenAl },  { 'G', zenkakualpha::iZenAl },
  //   { 'H', zenkakualpha::iZenAl },  { 'I', zenkakualpha::iZenAl },
  //   { 'J', zenkakualpha::iZenAl },  { 'K', zenkakualpha::iZenAl },
  //   { 'L', zenkakualpha::iZenAl },  { 'M', zenkakualpha::iZenAl },
  //   { 'N', zenkakualpha::iZenAl },  { 'O', zenkakualpha::iZenAl },
  //   { 'P', zenkakualpha::iZenAl },  { 'Q', zenkakualpha::iZenAl },
  //   { 'R', zenkakualpha::iZenAl },  { 'S', zenkakualpha::iZenAl },
  //   { 'T', zenkakualpha::iZenAl },  { 'U', zenkakualpha::iZenAl },
  //   { 'V', zenkakualpha::iZenAl },  { 'W', zenkakualpha::iZenAl },
  //   { 'X', zenkakualpha::iZenAl },  { 'Y', zenkakualpha::iZenAl },
  //   { 'Z', zenkakualpha::iZenAl },  { '[', zenkakualpha::iZenAl },
  //   { '\\', zenkakualpha::iZenAl }, { ']', zenkakualpha::iZenAl },
  //   { '^', zenkakualpha::iZenAl },  { '_', zenkakualpha::iZenAl },
  //   { '`', zenkakualpha::iZenAl },  { 'a', zenkakualpha::iZenAl },
  //   { 'b', zenkakualpha::iZenAl },  { 'c', zenkakualpha::iZenAl },
  //   { 'd', zenkakualpha::iZenAl },  { 'e', zenkakualpha::iZenAl },
  //   { 'f', zenkakualpha::iZenAl },  { 'g', zenkakualpha::iZenAl },
  //   { 'h', zenkakualpha::iZenAl },  { 'i', zenkakualpha::iZenAl },
  //   { 'j', zenkakualpha::iZenAl },  { 'k', zenkakualpha::iZenAl },
  //   { 'l', zenkakualpha::iZenAl },  { 'm', zenkakualpha::iZenAl },
  //   { 'n', zenkakualpha::iZenAl },  { 'o', zenkakualpha::iZenAl },
  //   { 'p', zenkakualpha::iZenAl },  { 'q', zenkakualpha::iZenAl },
  //   { 'r', zenkakualpha::iZenAl },  { 's', zenkakualpha::iZenAl },
  //   { 't', zenkakualpha::iZenAl },  { 'u', zenkakualpha::iZenAl },
  //   { 'v', zenkakualpha::iZenAl },  { 'w', zenkakualpha::iZenAl },
  //   { 'x', zenkakualpha::iZenAl },  { 'y', zenkakualpha::iZenAl },
  //   { 'z', zenkakualpha::iZenAl },  { '{', zenkakualpha::iZenAl },
  //   { '|', zenkakualpha::iZenAl },  { '}', zenkakualpha::iZenAl },
  //   { '~', zenkakualpha::iZenAl },
  // };
  return zenkakualpha::iZenAl(c, false);
}

} // namespace
