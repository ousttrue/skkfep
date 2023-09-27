#pragma once
#include <stdio.h>

extern int Shellfd;
extern int ShellPID;
extern FILE* Shellout;

bool
establishShell(const char* ShellName, char** ShellArg, void (*onSigChild)(int));

void
getDevice(int* master, int* slave);

void
freeDevice();

void
writeShell1(char c);

void
writeShells(const char* s);

void
writeTtyShell1(char c);


void
writeTtyShells(const char* s);

