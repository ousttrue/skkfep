#pragma once

extern short BlockTty;
extern char PreserveOnFailure;
extern char OkuriFirst;
extern struct Dictionary* UserDic;

void
kkBeg(char);
void
kkBegV(char c);

void
kkBegC(char c);

void
kkBegA(char c);

void
kalpha(char c);

void
kaBS(char c);

void
kKanaV(char c);

void
okKanaV(char c /*, char first*/);

void
kKanaC(char c);

void
okKanaC(char c /*, char first*/);

void
kZenAl(char c);

void
kZenEx(char c);

void
kfthru(char c);

void
fxthru(char c);

void
kfCancel(char c);

void
kfFix(char c);

void
kfFixToAsc(char c);

void
kfFixToZenA(char c);

void
kfFixThru(char c);

void
thruKfFixToAsc(char c);

void
okfFix(char c);

void
okfFixToAsc(char c);

void
okfFixToZenA(char c);

void
okfFixThru(char c);

void
thruOkfFixToAsc(char c);

void
kfBS(char c);

void
okuriBS(char c);

void
kkconv(char c);

void
toOkuri();

void
kOkuri(char c);

void
stSuffix(char c);

void
stPrefixCv(char c);

void
showCand();

void
nxCand(char);

void
pvCand(char);

/* back to kanji input mode */
void
backToKanjiInput();

void
cancelOkuri(char);

void
clearOkuri();

void
fixIt(char c);

void
thruFixItToAsc(char c);

void
cancelSel(char c);

void
endKanjiInput();

void
h2kkana(char c);
