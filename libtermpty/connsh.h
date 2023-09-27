#pragma once
#include <stdio.h>

extern int ShellPID;
extern FILE* Shellout;

void
establishShell(void (*onSigChild)(int));

void
getDevice(int* master, int* slave);

void
freeDevice();
