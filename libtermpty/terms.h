#pragma once

bool
getTCstr();

void
adjstr(char** buf, char** ptr);

void
toMsg();

void
fromMsg();

void
initFep();

void
termFep();

void
underline(int ctl);

void
standout(int ctl);

void
saveCsr();

void
restoreCsr();

void
clearToEnd();

void
csrLeft(int n);

void
csrRight(int n);

void
cls();

void
erase(int n);

void
rubout(int n);

void
bell();

void
showmessage(const char* s);

void
kanjiInputEffect(int on);

void
kanjiSelectionEffect(int on);
