#pragma once
#include "keymap.h"

extern short BlockTty;
extern char PreserveOnFailure;
extern char OkuriFirst;
extern struct Dictionary* UserDic;

class Skk;

void
kkClearBuf();

SkkOutput
kkBegV(Skk* skk, char c);

SkkOutput
kkBegC(Skk* skk, char c);

void
kkBegA(Skk* skk, char c);

SkkOutput
kalpha(char c, bool o = {});

void
kaBS(char c, bool o = {});

SkkOutput
kKanaV(char c, bool o = {});

SkkOutput
okKanaV(Skk* skk, char c, bool first);

SkkOutput
kKanaC(char c, bool o = {});

SkkOutput
okKanaC(char c, bool first);

SkkOutput
kZenAl(char c, bool o = {});

SkkOutput
kZenEx(char c, bool o = {});

SkkOutput
kfthru(char c, bool o = {});

SkkOutput
fxthru(Skk* skk, char c);

SkkOutput
kfCancel(char c, bool o = {});

SkkOutput
kfFix(char c, bool o = {});

SkkOutput
kfFixToAsc(char c, bool o = {});

SkkOutput
kfFixToZenA(char c, bool o = {});

SkkOutput
kfFixThru(Skk* skk, char c);

SkkOutput
thruKfFixToAsc(char c, bool o = {});

SkkOutput
okfFix(char c, bool o = {});

SkkOutput
okfFixToAsc(char c, bool o = {});

SkkOutput
okfFixToZenA(char c, bool o = {});

SkkOutput
okfFixThru(char c, bool o = {});

SkkOutput
thruOkfFixToAsc(char c, bool o = {});

void
kfBS(char c, bool o = {});

SkkOutput
okuriBS(Skk* skk, char c);

SkkOutput
kkconv(Skk* skk, char c);

SkkOutput
kOkuri(char c, bool o = {});

SkkOutput
stSuffix(Skk* skk, char c);

SkkOutput
stPrefixCv(Skk* skk, char c);

void
nxCand();

void
pvCand(Skk* skk);

/* back to kanji input mode */
SkkOutput
backToKanjiInput(Skk* skk);

SkkOutput
cancelOkuri(char c = {}, bool o = {});

SkkOutput
fixIt(char c = {}, bool o = {});

SkkOutput
thruFixItToAsc(char c, bool o = {});

SkkOutput
cancelSel(Skk* skk, char c);

SkkOutput
h2kkana(char c, bool o = {});
