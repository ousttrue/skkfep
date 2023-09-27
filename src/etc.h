#pragma once
#include "romkan.h"

void
sleep_ms(int n);

void
showmode(SkkModes s);

void
showcurmode();

void
showlastmode();

void
showmessage(const char* s);

void
kanjiInputEffect(int on);

void
kanjiSelectionEffect(int on);
