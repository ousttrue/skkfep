#include "ctrlcode.h"
// #include "fep.h"
// #include "keybind.h"
#include "connsh.h"
#include "kkconv.h"
#include "romkan.h"
#include "skk.h"
#include <stdio.h>

void
Skk::initializeKeymap()
{
  NormalKeymap = { thru,
                   {
                     { CTRL_T, [](auto) { g_skk.toKana(); } },
                   } };

  KanaKeymap = {
    /*	C-@		C-a		C-b		C-c	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-d		C-e		C-f		C-g	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-h 		C-i 		C-j 		C-k 	*/
    romkan::kanaBS,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-l 		C-m 		C-n 		C-o	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-p		C-q		C-r		C-s	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-t		C-u		C-v		C-w	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-x		C-y		C-z		C-[	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	C-\		C-]		C-^		C-_	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	SPC		!		"		#	*/
    romkan::flthru,
    romkan::iZenAl,
    romkan::flthru,
    romkan::flthru,
    /*	$		%		&		'	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	(		)		*		+	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	,		-		.		/	*/
    romkan::iZenAl,
    romkan::iZenEx,
    romkan::iZenAl,
    kkBegA,
    /*	0		1		2		3	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	4		5		6		7	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    /*	8		9		:		;	*/
    romkan::flthru,
    romkan::flthru,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	<		=		>		?	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::iZenAl,
    /*	@		A		B		C	*/
    romkan::flthru,
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
    [](auto) { g_skk.toZenA(); },
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
    romkan::iZenEx,
    /*	\		]		^		_	*/
    romkan::inputCode,
    romkan::iZenEx,
    romkan::flthru,
    romkan::flthru,
    /*	`		a		b		c	*/
    romkan::flthru,
    romkan::iKanaV,
    romkan::iKanaC,
    romkan::iKanaC,
    /*	d		e		f		g	*/
    romkan::iKanaC,
    romkan::iKanaV,
    romkan::iKanaC,
    romkan::iKanaC,
    /*	h		i		j		k	*/
    romkan::iKanaC,
    romkan::iKanaV,
    romkan::iKanaC,
    romkan::iKanaC,
    /*	l		m		n		o	*/
    [](auto) { g_skk.toAsc(); },
    romkan::iKanaC,
    romkan::iKanaC,
    romkan::iKanaV,
    /*	p		q		r		s	*/
    romkan::iKanaC,
    romkan::tglK,
    romkan::iKanaC,
    romkan::iKanaC,
    /*	t		u		v		w	*/
    romkan::iKanaC,
    romkan::iKanaV,
    romkan::iKanaC,
    romkan::iKanaC,
    /*	x		y		z		{	*/
    romkan::iKanaC,
    romkan::iKanaC,
    romkan::iKanaC,
    romkan::flthru,
    /*	|		}		~		DEL	*/
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
    romkan::flthru,
  };

  ZenkakuKeymap = {
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
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	$		%		&		'	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	(		)		*		+	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	,		-		.		/	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	0		1		2		3	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	4		5		6		7	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	8		9		:		;	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	<		=		>		?	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	@		A		B		C	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	D		E		F		G	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	H		I		J		K	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	L		M		N		O	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	P		Q		R		S	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	T		U		V		W	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	X		Y		Z		[	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	\		]		^		_	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	`		a		b		c	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	d		e		f		g	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	h		i		j		k	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	l		m		n		o	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	p		q		r		s	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	t		u		v		w	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	x		y		z		{	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    /*	|		}		~		DEL	*/
    romkan::iZenAl,
    romkan::iZenAl,
    romkan::iZenAl,
    thru,
  };

  KanjiInputKeymap = {
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

  OkuriInputKeymap = {
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

  KAlphaInputKeymap = {
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

  SelectionKeymap = { fxthru,
                      {
                        { CTRL_G, cancelSel },
                        { CTRL_T, fixIt },
                        { ' ', nxCand },
                        { 'x', pvCand },
                        { '>', stSuffix },
                        { '?', stSuffix },
                        { '<', stSuffix },
                        { EXTRA_CODE, thruFixItToAsc },
                        { EXTRA_CODE, thruFixItToEsc },
                      } };

  CodeInputKeymap = { nulcmd,
                      {
                        { CTRL_C, [](auto) { g_skk.cancelCode(); } },
                        { CTRL_G, [](auto) { g_skk.cancelCode(); } },
                        { CTRL_J, romkan::enterCode },
                        { CTRL_M, romkan::enterCode },
                        { CTRL_T, [](auto) { g_skk.toKana(); } },
                        { '0', romkan::codein },
                        { '1', romkan::codein },
                        { '2', romkan::codein },
                        { '3', romkan::codein },
                        { '4', romkan::codein },
                        { '5', romkan::codein },
                        { '6', romkan::codein },
                        { '7', romkan::codein },
                        { '8', romkan::codein },
                        { '9', romkan::codein },
                        { 'A', romkan::codein },
                        { 'B', romkan::codein },
                        { 'C', romkan::codein },
                        { 'D', romkan::codein },
                        { 'E', romkan::codein },
                        { 'F', romkan::codein },
                        { 'a', romkan::codein },
                        { 'b', romkan::codein },
                        { 'c', romkan::codein },
                        { 'd', romkan::codein },
                        { 'e', romkan::codein },
                        { 'f', romkan::codein },
                      } };

  _ViEscKeymap = {
    { 'i', thruBack }, { 'I', thruBack }, { 'a', thruBack }, { 'A', thruBack },
    { 'o', thruBack }, { 'O', thruBack }, { 'S', thruBack }, { 's', thruBack },
    { 'R', thruBack }, { 'r', thruBack }, { 'C', thruBack }, { 'c', thruBack },
  };

  _EmacsEscKeymap = {
    { EXTRA_CODE, nulcmd },
  };
}
