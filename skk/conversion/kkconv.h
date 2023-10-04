#pragma once
#include "keymap.h"

extern short BlockTty;
extern char PreserveOnFailure;
extern char OkuriFirst;
extern struct Dictionary* UserDic;

void
kkClearBuf();

skk::Result
kkBegV(char c, bool o = {});

skk::Result
kkBegC(char c, bool o = {});

skk::Result
kkBegA(char c, bool o = {});

skk::Result
kalpha(char c, bool o = {});

skk::Result
kaBS(char c, bool o = {});

skk::Result
kKanaV(char c, bool o = {});

skk::Result
okKanaV(char c, bool first);

skk::Result
kKanaC(char c, bool o = {});

skk::Result
okKanaC(char c, bool first);

skk::Result
kfthru(char c, bool o = {});

skk::Result
fxthru(char c = {}, bool o = {});

skk::Result
kfCancel(char c, bool o = {});

skk::Result
kfFix(char c, bool o = {});

skk::Result
kfFixToAsc(char c, bool o = {});

skk::Result
kfFixToZenA(char c, bool o = {});

skk::Result
kfFixThru(char c, bool o = {});

skk::Result
thruKfFixToAsc(char c, bool o = {});

skk::Result
okfFix(char c, bool o = {});

skk::Result
okfFixToAsc(char c, bool o = {});

skk::Result
okfFixToZenA(char c, bool o = {});

skk::Result
okfFixThru(char c, bool o = {});

skk::Result
thruOkfFixToAsc(char c, bool o = {});

skk::Result
kfBS(char c, bool o = {});

skk::Result
okuriBS(char c, bool o = {});

skk::Result
kkconv(char c, bool o = {});

skk::Result
kOkuri(char c, bool o = {});

skk::Result
stSuffix(char c, bool o = {});

skk::Result
stPrefixCv(char c, bool o = {});

skk::Result
nxCand(char c = {}, bool o = {});

skk::Result
pvCand(char c = {}, bool o = {});

skk::Result
cancelOkuri(char c = {}, bool o = {});

skk::Result
fixIt(char c = {}, bool o = {});

skk::Result
thruFixItToAsc(char c, bool o = {});

skk::Result
cancelSel(char c, bool o = {});

skk::Result
h2kkana(char c, bool o = {});
