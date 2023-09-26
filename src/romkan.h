#pragma once

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
