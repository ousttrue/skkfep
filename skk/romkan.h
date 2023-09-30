#pragma once
#include <functional>

using OutFunc = std::function<void(const char* s)>;

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
inputKanaVowel(char c, const OutFunc& output);

void
inputKanaConso(char c, const OutFunc& output, void (*flush)(int));

void
flushLastConso(char c, const OutFunc& output, void (*flush)(int));

void
tglK(char c);

void
cancelConso();

void
kanaBS(char c);

void
inputZenkakuAlpha(char c, const OutFunc& output);

void
inputZenkakuEx(char c, const OutFunc& output);

void
codeinMsg();

void
inputCode(char);

void
codein(char c);

void
enterCode(char);

void
hira2kata(char* buf);

void
toggleKana();

} // namespace
