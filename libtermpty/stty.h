#pragma once

extern int g_term_tty;

bool
set_tty();

void
reset_tty();

void
reset_tty_without_close();

void
set_tty_sane(int tty);

void
tty_ready();

void
init_signals();

void
writes(const char* s);

void
write1(char c);

void
writeShTty(const char* s, int l);

void
writeTty(const char* s, int l);
