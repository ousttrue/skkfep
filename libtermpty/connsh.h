#pragma once
#include <stdio.h>

extern int tty, Shellfd;
extern int ShellPID;
extern FILE* Shellout;

bool
establishShell(const char* ShellName, char** ShellArg, void (*onSigChild)(int));

void
getDevice(int* master, int* slave);

void
freeDevice();
