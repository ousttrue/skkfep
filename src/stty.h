#pragma once
void
set_winsize(int tty);

void
get_winsize();

void
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
set_int();

void
chld_changed(int);
