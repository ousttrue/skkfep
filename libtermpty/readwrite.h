#pragma once
#include "kanjicode.h"
struct KanjiStatus;
extern kanjicode OutCode, WriteCode;
#define KANJIBS_DEFAULT 0     /* 0 -> shift out / 1 -> don't shift out  */
extern char KanjiBS;

char
read1();

void
checkStatusAndConvCode(char c,
                       kanjicode ic,
                       kanjicode oc,
                       KanjiStatus* stat,
                       KanjiStatus* ostat,
                       char** optr,
                       int* olen);

void
syncStatus(int ttyno,
           kanjicode ic,
           kanjicode oc,
           KanjiStatus* istatus,
           KanjiStatus* ostatus);

void
writeTTYorg(int ttyno,
            const char* s,
            int l,
            kanjicode ic,
            kanjicode oc,
            KanjiStatus* istatus,
            KanjiStatus* ostatus);

void
writes(const char* s);

void
write1(char c);

void
writeShTty(const char* s, int l);

void
writeTty(const char* s, int l);

void
writeShell1(char c);

void
writeShells(const char* s);

void
writeTtyShell1(char c);

void
writeTtyShells(const char* s);

void
bell();

void
convCode(unsigned char c1,
         unsigned char c2,
         char* obuf,
         kanjicode ic,
         kanjicode oc);
