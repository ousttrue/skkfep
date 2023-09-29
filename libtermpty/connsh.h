#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int Shellfd;
extern int ShellPID;
extern FILE* Shellout;

bool
establishShell(const char* ShellName, char** ShellArg, void (*onSigChild)(int));

void
getDevice(int* master, int* slave);

void
freeDevice();

inline void
writeShell1(char c)
{
  write(fileno(Shellout), &c, 1);
}

inline void
writeShells(const char* s)
{
  int l = strlen(s);
  write(fileno(Shellout), s, l);
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
