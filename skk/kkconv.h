#pragma once
#include "keymap.h"

extern short BlockTty;
extern char PreserveOnFailure;
extern char OkuriFirst;
extern struct Dictionary* UserDic;

class Skk;

void
kkClearBuf();

SkkResult
kkBegV(char c, bool o = {});

SkkResult
kkBegC(char c, bool o = {});

SkkResult
kkBegA(char c, bool o = {});

SkkResult
kalpha(char c, bool o = {});

SkkResult
kaBS(char c, bool o = {});

SkkResult
kKanaV(char c, bool o = {});

SkkResult
okKanaV(char c, bool first);

SkkResult
kKanaC(char c, bool o = {});

SkkResult
okKanaC(char c, bool first);

SkkResult
kZenAl(char c, bool o = {});

SkkResult
kZenEx(char c, bool o = {});

SkkResult
kfthru(char c, bool o = {});

SkkResult
fxthru(Skk* skk, char c);

SkkResult
kfCancel(char c, bool o = {});

SkkResult
kfFix(char c, bool o = {});

SkkResult
kfFixToAsc(char c, bool o = {});

SkkResult
kfFixToZenA(char c, bool o = {});

SkkResult
kfFixThru(Skk* skk, char c);

SkkResult
thruKfFixToAsc(char c, bool o = {});

SkkResult
okfFix(char c, bool o = {});

SkkResult
okfFixToAsc(char c, bool o = {});

SkkResult
okfFixToZenA(char c, bool o = {});

SkkResult
okfFixThru(char c, bool o = {});

SkkResult
thruOkfFixToAsc(char c, bool o = {});

SkkResult
kfBS(char c, bool o = {});

SkkResult
okuriBS(char c, bool o = {});

SkkResult
kkconv(char c, bool o = {});

SkkResult
kOkuri(char c, bool o = {});

SkkResult
stSuffix(char c, bool o = {});

SkkResult
stPrefixCv(char c, bool o = {});

SkkResult
nxCand(char c = {}, bool o = {});

SkkResult
pvCand(char c = {}, bool o = {});

SkkResult
cancelOkuri(char c = {}, bool o = {});

SkkResult
fixIt(char c = {}, bool o = {});

SkkResult
thruFixItToAsc(char c, bool o = {});

SkkResult
cancelSel(char c, bool o = {});

SkkResult
h2kkana(char c, bool o = {});
