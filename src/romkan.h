#pragma once

#define SKK_MODE 0
#define KANA_MODE 1
#define ZENEI_MODE 2
#define KKANA_MODE 3
#define KINPUT_MODE 4
#define KSELECT_MODE 5

#define MAX_CONSO 5
extern char LastConso[MAX_CONSO]; /* LastConso[0] is always '\0' */
extern short Nconso;
extern char SmallTU;
extern short Kindex;


namespace romkan {

void
iKanaV(char c);

void
iKanaC(char c);

void
iZenAl(char c);

void
iZenEx(char c);

void
flthru(char c);

void
flushKana();

void
inputKanaVowel(char c, void (*output)(const char*));

void
inputKanaConso(char c, void (*output)(const char*), void (*flush)(int));

void
flushLastConso(char c, void (*output)(const char*), void (*flush)(int));

void
toKana(char c);

void
tglK(char c);

void
cancelConso();

void
kanaBS(char c);

void
inputZenkakuAlpha(char c, void (*output)(const char*));

void
inputZenkakuEx(char c, void (*output)(const char*));

void
codeinMsg();

void
inputCode(char);

void
cancelCode(char);

void
codein(char c);

void
enterCode(char);

void
hira2kata(char* buf);

} // namespace
