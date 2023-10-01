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
kkBegV(char c, bool o = {});

SkkOutput
kkBegC(char c, bool o = {});

SkkOutput
kkBegA(char c, bool o = {});

SkkOutput
kalpha(char c, bool o = {});

SkkOutput
kaBS(char c, bool o = {});

SkkOutput
kKanaV(char c, bool o = {});

SkkOutput
okKanaV(char c, bool first);

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

SkkOutput
kfBS(char c, bool o = {});

SkkOutput
okuriBS(char c, bool o = {});

SkkOutput
kkconv(char c, bool o = {});

SkkOutput
kOkuri(char c, bool o = {});

SkkOutput
stSuffix(char c, bool o = {});

SkkOutput
stPrefixCv(char c, bool o = {});

SkkOutput
nxCand(char c = {}, bool o = {});

SkkOutput
pvCand(char c = {}, bool o = {});

SkkOutput
cancelOkuri(char c = {}, bool o = {});

SkkOutput
fixIt(char c = {}, bool o = {});

SkkOutput
thruFixItToAsc(char c, bool o = {});

SkkOutput
cancelSel(char c, bool o = {});

SkkOutput
h2kkana(char c, bool o = {});
