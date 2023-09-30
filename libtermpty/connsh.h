#pragma once
#include "termsize.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

extern int Shellfd;
extern int ShellPID;
extern FILE* Shellout;

bool
establishShell(const TermSize& size,
               const char* ShellName,
               char** ShellArg,
               void (*onSigChild)(int),
               const char* version);

void
freeDevice();

inline void
writeShell1(char c)
{
  write(fileno(Shellout), &c, 1);
}

inline void
writeShells(std::string_view s)
{
  write(fileno(Shellout), s.data(), s.size());
}

inline void
writeTtyShell1(char c)
{
  write(fileno(Shellout), &c, 1);
}

inline void
writeTtyShells(const char* s)
{
  int l = strlen(s);
  write(fileno(Shellout), s, l);
}

void
set_termsize(int tty, const TermSize& size);

void
flushOut(int minchar);

void
thru(char c);
