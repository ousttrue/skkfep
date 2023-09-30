#pragma once
#include <functional>
#include <string>

using OutFunc = std::function<void(std::string_view s)>;

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
hira2kata(char* buf);

void
toggleKana();

} // namespace
