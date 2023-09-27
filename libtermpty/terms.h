#pragma once
extern char ReverseStatus;

bool
getTCstr();

void
setEnvirons();

void
adjstr(char** buf, char** ptr);

int
msgLine();

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
