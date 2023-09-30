#pragma once

extern short BlockTty;
extern char PreserveOnFailure;
extern char OkuriFirst;
extern struct Dictionary* UserDic;

class Skk;

void
kkClearBuf();

void
kkBegV(Skk* skk, char c);

void
kkBegC(Skk* skk, char c);

void
kkBegA(Skk* skk, char c);

void
kalpha(char c, bool o = {});

void
kaBS(char c, bool o = {});

void
kKanaV(char c, bool o = {});

void
okKanaV(Skk* skk, char c, bool first);

void
kKanaC(char c, bool o = {});

void
okKanaC(char c, bool first);

void
kZenAl(char c, bool o = {});

void
kZenEx(char c, bool o = {});

void
kfthru(char c, bool o = {});

void
fxthru(Skk* skk, char c);

void
kfCancel(Skk* skk, char c);

void
kfFix(Skk* skk, char c);

void
kfFixToAsc(Skk* skk, char c);

void
kfFixToZenA(Skk* skk, char c);

void
kfFixThru(Skk* skk, char c);

void
thruKfFixToAsc(Skk* skk, char c);

void
okfFix(Skk* skk, char c);

void
okfFixToAsc(Skk* skk, char c);

void
okfFixToZenA(Skk* skk, char c);

void
okfFixThru(Skk* skk, char c);

void
thruOkfFixToAsc(Skk* skk, char c);

void
kfBS(char c, bool o = {});

void
okuriBS(Skk* skk, char c);

void
kkconv(Skk* skk, char c);

void
toOkuri(Skk* skk);

void
kOkuri(Skk* skk, char c);

void
stSuffix(Skk* skk, char c);

void
stPrefixCv(Skk* skk, char c);

void
showCand();

void
nxCand();

void
pvCand(Skk* skk);

/* back to kanji input mode */
void
backToKanjiInput(Skk* skk);

void
cancelOkuri(Skk* skk);

void
clearOkuri(Skk* skk);

void
fixIt(Skk* skk);

void
thruFixItToAsc(Skk* skk, char c);

void
cancelSel(Skk* skk, char c);

void
endKanjiInput(Skk* skk);

void
h2kkana(Skk* skk, char c);
