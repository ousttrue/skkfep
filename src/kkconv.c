#include "config.h"
#include "fep.h"
#include "functions.h"
#include <ctype.h>

#define WORD_BUF_LEN 128
#define OKURI_LEN 8

#define VOWEL(c) ((c)=='a'||(c)=='i'||(c)=='u'||(c)=='e'||(c)=='o')

static char WordBuf[WORD_BUF_LEN];
static int WordBufLen;
static CandList CurrentCand;
static CandList *FirstCandEntry;
static short OkuriInput;
static char OkuriBuf[OKURI_LEN];
static int OkuriBufLen;

static void putOkuri();
void showCand();

static void
bufferedInput(s)
char *s;
{
	writes(s);
	for (; *s != '\0'; s++)
		WordBuf[WordBufLen++] = *s;
}

void
kkBeg()
{
	setKeymap(&CurrentKeymap,KanjiInputKeymap);
	showmode(KINPUT_MODE);
	kanjiInputEffect(1);
	WordBufLen = 0;
}
void
kkBegV(c)
char c;
{
	kkBeg();
	inputKanaVowel(tolower(c),bufferedInput);
}
	
void
kkBegC(c)
char c;
{
	kkBeg();
	inputKanaConso(tolower(c),bufferedInput,NULL);
}

void
kkBegA(c)
char c;
{
	flushKana();
	setKeymap(&CurrentKeymap,KAlphaInputKeymap);
	showmode(KINPUT_MODE);
	kanjiInputEffect(1);
	WordBufLen = 0;
}

void
kalpha(c)
char c;
{
	write1(c);
	WordBuf[WordBufLen++] = c;
}

void
kaBS(c)
char c;
{
	if (WordBufLen > 0) {
		kanjiInputEffect(0);
		rubout(1);
		WordBufLen--;
		kanjiInputEffect(1);
	}
}

void
kKanaV(c)
char c;
{
	inputKanaVowel(c,bufferedInput);
}

void
okKanaV(c,first)
char c,first;
{
	if (first) {
		WordBuf[WordBufLen++] = c;
	}
	rubout(1);
	inputKanaVowel(tolower(c),putOkuri);
	kkconv();
}

void
kKanaC(c)
char c;
{
	inputKanaConso(c,bufferedInput,NULL);
}

void
okKanaC(c,first)
char c,first;
{
	if (Nconso == 0) {
		WordBuf[WordBufLen++] = c;
	}
	else if (first) {
		flushLastConso('\0',bufferedInput,NULL);
		WordBuf[WordBufLen++] = c;
	}
	inputKanaConso(tolower(c),putOkuri,NULL);
}

void
kZenAl(c)
char c;
{
	inputZenkakuAlpha(c,bufferedInput);
}

void
kZenEx(c)
char c;
{
	inputZenkakuEx(c,bufferedInput);
}

void
kfthru(c)
char c;
{
	static char buf[2];
	buf[0] = c;
	buf[1] = '\0';
	bufferedInput(buf);
}

void
fxthru(c)
char c;
{
	/* fix and through */
	fixIt();
	(*CurrentKeymap[c])(c);
}

void
kfCancel(c)
char c;
{
	kanjiInputEffect(0);
	cancelConso();
	rubout(WordBufLen);
	WordBuf[WordBufLen] = '\0';
	endKanjiInput();
}

void
kfFix(c)
char c;
{
	csrLeft(WordBufLen);
	WordBuf[WordBufLen] = '\0';
	kanjiInputEffect(0);
	writeShells(WordBuf);
	flushOut(WordBufLen);
	endKanjiInput();
}

void
kfFixToAsc(c)
char c;
{
	kfFix(c);
	toAsc(c);
}

void
kfFixToZenA(c)
char c;
{
	kfFix(c);
	toZenA(c);
}

void
kfFixThru(c)
{
	kfFix(c);
	thru(c);
}

void
thruKfFixToAsc(c)
char c;
{
	kfFix(c);
	toAsc(c);
	thru(c);
}

void
thruKfFixToEsc(c)
char c;
{
	kfFix(c);
	toEsc(c);
	thru(c);
}

void
okfFix(c)
{
	cancelOkuri();
	if (CurrentKeymap == OkuriInputKeymap)
		cancelOkuri();
	kfFix(0);
}

void
okfFixToAsc(c)
char c;
{
	okfFix(c);
	toAsc(c);
}

void
okfFixToZenA(c)
char c;
{
	okfFix(c);
	toZenA(c);
}

void
okfFixThru(c)
{
	okfFix(c);
	thru(c);
}

void
thruOkfFixToAsc(c)
char c;
{
	okfFix(c);
	toAsc(c);
	thru(c);
}

void
thruOkfFixToEsc(c)
char c;
{
	okfFix(c);
	toEsc(c);
	thru(c);
}

void
kfBS(c)
char c;
{
	int i,n;
	char con[MAX_CONSO];

	kanjiInputEffect(0);
	if (Nconso) {
		rubout(Nconso);
		for (i = 1; i < Nconso; i++)
			con[i] = LastConso[i];
		n = Nconso;
		Nconso = 0;
		Kindex = 0;
		SmallTU = 0;
		for (i = 1; i < n; i++)
			kKanaC(con[i]);
	}
	else if (WordBufLen > 0) {
		if (WordBuf[WordBufLen-1] & 0x80) { /* is KANJI */
			rubout(2);
			WordBufLen -= 2;
		}
		else {
			rubout(1);
			WordBufLen--;
		}
	}
	kanjiInputEffect(1);
}

void
okuriBS(c)
char c;
{
	int i,n;
	char con[MAX_CONSO];

	kanjiInputEffect(0);
	n = Nconso;
	for (i = 0; i < n; i++)
		con[i] = LastConso[i];
	if (n <= 1) {
		cancelOkuri();
	}
	else {
		cancelConso();
		for (i = 0; i < n; i++)
			okKanaC(con[i],0);
	}
	kanjiInputEffect(1);
}


void
kkconv(c)
char c;
{
	int l;
	DicList originalDicItem = NULL;

	kanjiInputEffect(0);
	flushLastConso('\0',bufferedInput,NULL);
	cancelConso();
	if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
		endKanjiInput();
	}
	showmode(KSELECT_MODE); 
		
	WordBuf[WordBufLen] = '\0';
	CurrentCand = getCand(UserDic,WordBuf);
	if (CurrentCand) {
		originalDicItem = CurrentCand->dicitem;
		if (OkuriInput)
			CurrentCand = searchOkuri(CurrentCand,
						  OkuriBuf,
						  &FirstCandEntry);
		else
			FirstCandEntry = &(CurrentCand->dicitem->cand);
	}
	else {
		FirstCandEntry = NULL;
	}
#ifdef SYSTEM_DIC_NAME
	if (CurrentCand == NULL) {
	    CurrentCand = getCand(SystemDic,WordBuf);
		if (CurrentCand && OkuriInput)
			CurrentCand = searchOkuri(CurrentCand,
						  OkuriBuf,
						  &FirstCandEntry);
	}
#endif

	l = WordBufLen;
	if (OkuriInput)
		l += strlen(OkuriBuf)-1;
	rubout(l);
	setKeymap(&CurrentKeymap,convertKeymap(&SelectionKeymap));

	if (CurrentCand == NULL) {
		/* Really, enter register mode */
		if (PreserveOnFailure) {
			bell();
			backToKanjiInput();
		}
		else {
			endKanjiInput();
		}
	}
	else {
		if (CurrentCand->dicitem == NULL && originalDicItem) {
			CurrentCand->dicitem = originalDicItem;
			if (!OkuriInput)
				FirstCandEntry = &(CurrentCand->dicitem->cand);
		}
		showCand();
		BlockTty = 1;
	}
}

static void
putOkuri(s)
char *s;
{
	char *p;
	int l = OkuriBufLen;
	writes(s);
	for (p = s; *p; p++) {
		OkuriBuf[OkuriBufLen++] = *p;
		if (OkuriBufLen >= OKURI_LEN) {
			bell();
			OkuriBuf[l] = '\0';
			OkuriBufLen = l;
			rubout(strlen(s));
			return;
		}
	}
	OkuriBuf[OkuriBufLen] = '\0';
}

toOkuri()
{
	OkuriInput = 1;
	*OkuriBuf = '\0';
	OkuriBufLen = 0;
	setKeymap(&CurrentKeymap, OkuriInputKeymap);
}
	
void
kOkuri(c)
char c;
{
	char okuri = tolower(c);

	if (WordBufLen == 0) {
		/* Recover chattering effect */
		if (VOWEL(okuri)) {
			kKanaV(okuri);
			return;
		}
		cancelConso();
		endKanjiInput();
		kanjiInputEffect(0);
		return;
	}
	toOkuri();
	write1('*');
	(*CurrentKeymap[okuri])(okuri,1);
}

void
stSuffix(c)
char c;
{
	fixIt();
	kkBeg();
	kfthru(c);
}

void
stPrefixCv(c)
char c;
{
	if (WordBufLen == 0) {
		kfthru(c);
	}
	else {
		kfthru(c);
		kkconv(' ');
	}
}

void
showCand()
{
	kanjiSelectionEffect(1);
	writes(CurrentCand->candword);
	if (OkuriInput) {
		writes(OkuriBuf);
	}
}

static
showNextCand(nextcand)
CandList nextcand;
{
	int l;

	l= strlen(CurrentCand->candword);
	if (OkuriInput)
		l += strlen(OkuriBuf);
	kanjiSelectionEffect(0);
	csrLeft(l);
	erase(l);
	csrLeft(l);
	kanjiSelectionEffect(1);
	CurrentCand = nextcand;
	writes(CurrentCand->candword);
	if (OkuriInput)
		writes(OkuriBuf);
}

void
nxCand()
{
	int l;
	CandList nextcand,nnextcand;

	nextcand = CurrentCand->nextcand;
	nnextcand = CurrentCand;
	while (nextcand && nextcand->okuri) {
		nnextcand = nextcand;
		nextcand = nnextcand->nextcand;
	}
	if (nextcand) {
		showNextCand(nextcand);
	}
}

void
pvCand()
{
	int l;

	if (CurrentCand->prevcand && CurrentCand->prevcand->okuri) {
		CurrentCand = CurrentCand->prevcand;
		pvCand();
	}
	l= strlen(CurrentCand->candword);
	if (OkuriInput)
		l += strlen(OkuriBuf);
	kanjiSelectionEffect(0);
	csrLeft(l);
	erase(l);
	csrLeft(l);
	if (CurrentCand->prevcand) {
		kanjiSelectionEffect(1);
		CurrentCand = CurrentCand->prevcand;
		writes(CurrentCand->candword);
		if (OkuriInput)
			writes(OkuriBuf);
	}
	else {
		backToKanjiInput();
	}
}

/* back to kanji input mode */
backToKanjiInput()
{
	restoreKeymap(&CurrentKeymap);
	showmode(KINPUT_MODE);
	kanjiInputEffect(1);
	if (OkuriInput) {
		clearOkuri();
		writes(WordBuf);
		write1('*');
		OkuriFirst = 1;
	}
	else
		writes(WordBuf);
}

void
cancelOkuri()
{
	kanjiInputEffect(0);
	if (Nconso == 0) {
		rubout(1);
		OkuriInput = 0;
		setKeymap(&CurrentKeymap, KanjiInputKeymap);
	}
	else {
		cancelConso();
		clearOkuri();
		OkuriFirst = 1;
	}
	kanjiInputEffect(1);
}

clearOkuri()
{
	WordBufLen--;
	WordBuf[WordBufLen] = '\0';
	toOkuri();
}

void
fixIt(c)
char c;
{
	int l;
	DicList dlist;

	kanjiSelectionEffect(0);
	if (CurrentCand != NULL) {
		l= strlen(CurrentCand->candword);
		if (OkuriInput)
			l += strlen(OkuriBuf);
		csrLeft(l);
		writeShells(CurrentCand->candword);
		if (OkuriInput) {
			writeShells(OkuriBuf);
		}
		flushOut(l);
	}
	endKanjiInput();
}

void
thruFixItToAsc(c)
{
	fixIt(c);
	toAsc(c);
	thru(c);
}

void
thruFixItToEsc(c)
{
	fixIt(c);
	toEsc(c);
	thru(c);
}

void
cancelSel(c)
char c;
{
	int l;
	DicList dlist;

	kanjiSelectionEffect(0);
	if (CurrentCand != NULL) {
		l= strlen(CurrentCand->candword);
		if (OkuriInput)
			l += strlen(OkuriBuf);
		rubout(l);
	}
	backToKanjiInput();
}
	

endKanjiInput()
{
	*OkuriBuf = '\0';
	OkuriBufLen = 0;
	OkuriInput = 0;
	toKana();
	BlockTty = 0;
}

void
h2kkana(c)
char c;
{
	int l;

	kanjiInputEffect(0);
	flushLastConso('\0',bufferedInput,NULL);
	cancelConso();
	if (WordBufLen == 0 || (OkuriInput && WordBufLen == 1)) {
		endKanjiInput();
	}
	WordBuf[WordBufLen] = '\0';

	l = WordBufLen;
	if (OkuriInput)
		l += strlen(OkuriBuf)-1;
	rubout(l);
	hira2kata(WordBuf);
	writeShells(WordBuf);
	endKanjiInput();
}
