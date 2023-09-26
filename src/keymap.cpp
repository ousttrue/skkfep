#include "config.h"
#include "ctrlcode.h"
#include "fep.h"
#include "keybind.h"
#include "kkconv.h"
#include "romkan.h"

SparseKeymapBody _NormalKeymap[] = {
  CTRL_T,
  toKana,
  '\0',
  NULL,
};

SparseKeymap NormalKeymap = { thru, _NormalKeymap };

Keymap KanaKeymap = {
  /*	C-@		C-a		C-b		C-c	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	C-d		C-e		C-f		C-g	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	C-h 		C-i 		C-j 		C-k 	*/
  kanaBS,
  flthru,
  flthru,
  flthru,
  /*	C-l 		C-m 		C-n 		C-o	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	C-p		C-q		C-r		C-s	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	C-t		C-u		C-v		C-w	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	C-x		C-y		C-z		C-[	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	C-\		C-]		C-^		C-_	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	SPC		!		"		#	*/
  flthru,
  iZenAl,
  flthru,
  flthru,
  /*	$		%		&		'	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	(		)		*		+	*/
  flthru,
  flthru,
  flthru,
  flthru,
/*	,		-		.		/	*/
#ifndef KUTOUTEN
  iZenAl,
  iZenEx,
  iZenAl,
  kkBegA,
#else
  iZenEx,
  iZenEx,
  iZenEx,
  kkBegA,
#endif
  /*	0		1		2		3	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	4		5		6		7	*/
  flthru,
  flthru,
  flthru,
  flthru,
  /*	8		9		:		;	*/
  flthru,
  flthru,
  iZenAl,
  iZenAl,
  /*	<		=		>		?	*/
  flthru,
  flthru,
  flthru,
  iZenAl,
  /*	@		A		B		C	*/
  flthru,
  kkBegV,
  kkBegC,
  kkBegC,
  /*	D		E		F		G	*/
  kkBegC,
  kkBegV,
  kkBegC,
  kkBegC,
  /*	H		I		J		K	*/
  kkBegC,
  kkBegV,
  kkBegC,
  kkBegC,
  /*	L		M		N		O	*/
  toZenA,
  kkBegC,
  kkBegC,
  kkBegV,
  /*	P		Q		R		S	*/
  kkBegC,
  kkBeg,
  kkBegC,
  kkBegC,
  /*	T		U		V		W	*/
  kkBegC,
  kkBegV,
  kkBegC,
  kkBegC,
  /*	X		Y		Z		[	*/
  kkBegC,
  kkBegC,
  kkBegC,
  iZenEx,
  /*	\		]		^		_	*/
  inputCode,
  iZenEx,
  flthru,
  flthru,
  /*	`		a		b		c	*/
  flthru,
  iKanaV,
  iKanaC,
  iKanaC,
  /*	d		e		f		g	*/
  iKanaC,
  iKanaV,
  iKanaC,
  iKanaC,
  /*	h		i		j		k	*/
  iKanaC,
  iKanaV,
  iKanaC,
  iKanaC,
  /*	l		m		n		o	*/
  toAsc,
  iKanaC,
  iKanaC,
  iKanaV,
  /*	p		q		r		s	*/
  iKanaC,
  tglK,
  iKanaC,
  iKanaC,
  /*	t		u		v		w	*/
  iKanaC,
  iKanaV,
  iKanaC,
  iKanaC,
  /*	x		y		z		{	*/
  iKanaC,
  iKanaC,
  iKanaC,
  flthru,
  /*	|		}		~		DEL	*/
  flthru,
  flthru,
  flthru,
  flthru,
};

Keymap ZenkakuKeymap = {
  /*	C-@		C-a		C-b		C-c	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-d		C-e		C-f		C-g	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-h 		C-i 		C-j 		C-k 	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-l 		C-m 		C-n 		C-o	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-p		C-q		C-r		C-s	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-t		C-u		C-v		C-w	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-x		C-y		C-z		C-[	*/
  thru,
  thru,
  thru,
  thru,
  /*	C-\		C-]		C-^		C-_	*/
  thru,
  thru,
  thru,
  thru,
  /*	SPC		!		"		#	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	$		%		&		'	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	(		)		*		+	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	,		-		.		/	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	0		1		2		3	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	4		5		6		7	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	8		9		:		;	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	<		=		>		?	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	@		A		B		C	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	D		E		F		G	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	H		I		J		K	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	L		M		N		O	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	P		Q		R		S	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	T		U		V		W	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	X		Y		Z		[	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	\		]		^		_	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	`		a		b		c	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	d		e		f		g	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	h		i		j		k	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	l		m		n		o	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	p		q		r		s	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	t		u		v		w	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	x		y		z		{	*/
  iZenAl,
  iZenAl,
  iZenAl,
  iZenAl,
  /*	|		}		~		DEL	*/
  iZenAl,
  iZenAl,
  iZenAl,
  thru,
};

Keymap KanjiInputKeymap = {
  /*	C-@		C-a		C-b		C-c	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-d		C-e		C-f		C-g	*/
  nulcmd,
  nulcmd,
  nulcmd,
  kfCancel,
  /*	C-h 		C-i 		C-j 		C-k 	*/
  kfBS,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-l 		C-m 		C-n 		C-o	*/
  nulcmd,
  kfFixThru,
  nulcmd,
  nulcmd,
  /*	C-p		C-q		C-r		C-s	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-t		C-u		C-v		C-w	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-x		C-y		C-z		C-[	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-\		C-]		C-^		C-_	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	SPC		!		"		#	*/
  kkconv,
  kZenAl,
  kfthru,
  kfthru,
  /*	$		%		&		'	*/
  kfthru,
  kfthru,
  kfthru,
  kfthru,
  /*	(		)		*		+	*/
  kfthru,
  kfthru,
  kfthru,
  kfthru,
  /*	,		-		.		/	*/
  kZenAl,
  kZenEx,
  kZenAl,
  kfthru,
  /*	0		1		2		3	*/
  kfthru,
  kfthru,
  kfthru,
  kfthru,
  /*	4		5		6		7	*/
  kfthru,
  kfthru,
  kfthru,
  kfthru,
  /*	8		9		:		;	*/
  kfthru,
  kfthru,
  kZenAl,
  kZenAl,
  /*	<		=		>		?	*/
  stPrefixCv,
  kfthru,
  stPrefixCv,
  stPrefixCv,
  /*	@		A		B		C	*/
  kfthru,
  kOkuri,
  kOkuri,
  kOkuri,
  /*	D		E		F		G	*/
  kOkuri,
  kOkuri,
  kOkuri,
  kOkuri,
  /*	H		I		J		K	*/
  kOkuri,
  kOkuri,
  kOkuri,
  kOkuri,
  /*	L		M		N		O	*/
  kfFixToZenA,
  kOkuri,
  kOkuri,
  kOkuri,
  /*	P		Q		R		S	*/
  kOkuri,
  nulcmd,
  kOkuri,
  kOkuri,
  /*	T		U		V		W	*/
  kOkuri,
  kOkuri,
  kOkuri,
  kOkuri,
  /*	X		Y		Z		[	*/
  kOkuri,
  kOkuri,
  kOkuri,
  kZenEx,
  /*	\		]		^		_	*/
  kfthru,
  kZenEx,
  kfthru,
  kfthru,
  /*	`		a		b		c	*/
  kfthru,
  kKanaV,
  kKanaC,
  kKanaC,
  /*	d		e		f		g	*/
  kKanaC,
  kKanaV,
  kKanaC,
  kKanaC,
  /*	h		i		j		k	*/
  kKanaC,
  kKanaV,
  kKanaC,
  kKanaC,
  /*	l		m		n		o	*/
  kfFixToAsc,
  kKanaC,
  kKanaC,
  kKanaV,
  /*	p		q		r		s	*/
  kKanaC,
  h2kkana,
  kKanaC,
  kKanaC,
  /*	t		u		v		w	*/
  kKanaC,
  kKanaV,
  kKanaC,
  kKanaC,
  /*	x		y		z		{	*/
  kKanaC,
  kKanaC,
  kKanaC,
  kfthru,
  /*	|		}		~		DEL	*/
  kfthru,
  kfthru,
  kfthru,
  thru,
};

Keymap OkuriInputKeymap = {
  /*	C-@		C-a		C-b		C-c	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-d		C-e		C-f		C-g	*/
  nulcmd,
  nulcmd,
  nulcmd,
  cancelOkuri,
  /*	C-h 		C-i 		C-j 		C-k 	*/
  okuriBS,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-l 		C-m 		C-n 		C-o	*/
  nulcmd,
  okfFixThru,
  nulcmd,
  nulcmd,
  /*	C-p		C-q		C-r		C-s	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-t		C-u		C-v		C-w	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-x		C-y		C-z		C-[	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-\		C-]		C-^		C-_	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	SPC		!		"		#	*/
  okfFix,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	$		%		&		'	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	(		)		*		+	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	,		-		.		/	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	0		1		2		3	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	4		5		6		7	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	8		9		:		;	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	<		=		>		?	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	@		A		B		C	*/
  nulcmd,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	D		E		F		G	*/
  okKanaC,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	H		I		J		K	*/
  okKanaC,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	L		M		N		O	*/
  okfFixToZenA,
  okKanaC,
  okKanaC,
  okKanaV,
  /*	P		Q		R		S	*/
  okKanaC,
  nulcmd,
  okKanaC,
  okKanaC,
  /*	T		U		V		W	*/
  okKanaC,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	X		Y		Z		[	*/
  okKanaC,
  okKanaC,
  okKanaC,
  nulcmd,
  /*	\		]		^		_	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	`		a		b		c	*/
  nulcmd,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	d		e		f		g	*/
  okKanaC,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	h		i		j		k	*/
  okKanaC,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	l		m		n		o	*/
  okfFixToAsc,
  okKanaC,
  okKanaC,
  okKanaV,
  /*	p		q		r		s	*/
  okKanaC,
  nulcmd,
  okKanaC,
  okKanaC,
  /*	t		u		v		w	*/
  okKanaC,
  okKanaV,
  okKanaC,
  okKanaC,
  /*	x		y		z		{	*/
  okKanaC,
  okKanaC,
  okKanaC,
  nulcmd,
  /*	|		}		~		DEL	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
};

Keymap KAlphaInputKeymap = {
  /*	C-@		C-a		C-b		C-c	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-d		C-e		C-f		C-g	*/
  nulcmd,
  nulcmd,
  nulcmd,
  kfCancel,
  /*	C-h 		C-i 		C-j 		C-k 	*/
  kaBS,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-l 		C-m 		C-n 		C-o	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-p		C-q		C-r		C-s	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-t		C-u		C-v		C-w	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-x		C-y		C-z		C-[	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	C-\		C-]		C-^		C-_	*/
  nulcmd,
  nulcmd,
  nulcmd,
  nulcmd,
  /*	SPC		!		"		#	*/
  kkconv,
  kalpha,
  kalpha,
  kalpha,
  /*	$		%		&		'	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	(		)		*		+	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	,		-		.		/	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	0		1		2		3	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	4		5		6		7	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	8		9		:		;	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	<		=		>		?	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	@		A		B		C	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	D		E		F		G	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	H		I		J		K	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	L		M		N		O	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	P		Q		R		S	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	T		U		V		W	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	X		Y		Z		[	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	\		]		^		_	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	`		a		b		c	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	d		e		f		g	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	h		i		j		k	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	l		m		n		o	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	p		q		r		s	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	t		u		v		w	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	x		y		z		{	*/
  kalpha,
  kalpha,
  kalpha,
  kalpha,
  /*	|		}		~		DEL	*/
  kalpha,
  kalpha,
  kalpha,
  nulcmd,
};

SparseKeymapBody _SelectionKeymap[] = {
  CTRL_G, cancelSel, CTRL_T,     fixIt,          ' ',        nxCand,
  'x',    pvCand,    '>',        stSuffix,       '?',        stSuffix,
  '<',    stSuffix,  EXTRA_CODE, thruFixItToAsc, EXTRA_CODE, thruFixItToEsc,
  '\0',   NULL,
};

SparseKeymap SelectionKeymap = { fxthru, _SelectionKeymap };

SparseKeymapBody _CodeInputKeymap[] = {
  CTRL_C, cancelCode, CTRL_G, cancelCode, CTRL_J, enterCode, CTRL_M, enterCode,
  CTRL_T, toKana,     '0',    codein,     '1',    codein,    '2',    codein,
  '3',    codein,     '4',    codein,     '5',    codein,    '6',    codein,
  '7',    codein,     '8',    codein,     '9',    codein,    'A',    codein,
  'B',    codein,     'C',    codein,     'D',    codein,    'E',    codein,
  'F',    codein,     'a',    codein,     'b',    codein,    'c',    codein,
  'd',    codein,     'e',    codein,     'f',    codein,    '\0',   NULL,
};

SparseKeymap CodeInputKeymap = { nulcmd, _CodeInputKeymap };

SparseKeymapBody _ViEscKeymap[] = {
  'i', thruBack, 'I', thruBack, 'a',  thruBack, 'A', thruBack, 'o', thruBack,
  'O', thruBack, 'S', thruBack, 's',  thruBack, 'R', thruBack, 'r', thruBack,
  'C', thruBack, 'c', thruBack, '\0', NULL,
};

SparseKeymapBody _EmacsEscKeymap[] = {
  EXTRA_CODE,
  nulcmd,
  '\0',
  NULL,
};

SparseKeymap EscapedKeymap;
