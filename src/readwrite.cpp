#include "readwrite.h"
#include "config.h"
#include "ctrlcode.h"
#include "fep.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#ifdef DEBUG
extern FILE* wrlog;
#endif

kanjicode OutCode = NATIVECODE, WriteCode = NATIVECODE;

struct KanjiStatus
{
  char kanjiMode;
  char lastByte;
#ifdef DEBUG
  char* name;
#endif
};

struct code_mode codeMode[8] = {
  NULL,  NULL,  "EUC", 0, 0, NULL,  NULL,  "SJIS", 0, 0,
  SIC_A, SOC_H, "JIS", 3, 3, SIC_A, SOC_J, "JIS",  3, 3,
  SIC_A, SOC_B, "JIS", 3, 3, SIC_B, SOC_H, "JIS",  3, 3,
  SIC_B, SOC_J, "JIS", 3, 3, SIC_B, SOC_B, "JIS",  3, 3,
};

extern int tty;

char
read1()
{
  char c;

  if (read(tty, &c, 1) != 0)
    return c;
  return EOF;
}

void
checkStatusAndConvCode(char c,
                       kanjicode ic,
                       kanjicode oc,
                       KanjiStatus* stat,
                       KanjiStatus* ostat,
                       char** optr,
                       int* olen)

{
  static char buf[6];

#ifdef DEBUG
  fprintf(wrlog,
          "%s(%s:%d)->%s(%s:%d): char %2x",
          stat->name,
          codeMode[ic].name,
          stat->kanjiMode,
          ostat->name,
          codeMode[oc].name,
          ostat->kanjiMode,
          ((int)c) & 0xff);
  if ((c & 0x80) == 0) {
    if (c < ' ')
      fprintf(wrlog, "(^%c)", c + '@');
    else
      fprintf(wrlog, "(%c)", c);
  }
  fputc('\n', wrlog);
#endif
  *olen = 0;
  if (!isJIS(ic)) {
    switch (stat->kanjiMode) {
      case KS_NOKANJI:
        if (c & 0x80) {
          stat->kanjiMode = KS_BYTE1;
          ostat->kanjiMode = KS_BYTE2;
          stat->lastByte = c;
          *optr = codeMode[oc].sicode;
          *olen = codeMode[oc].si_len;
        } else {
          buf[0] = c;
          *optr = buf;
          *olen = 1;
        }
        break;
      case KS_BYTE1:
        stat->kanjiMode = KS_BYTE2;
        ostat->kanjiMode = KS_BYTE2;
        convCode(stat->lastByte, c, buf, ic, oc);
        *optr = buf;
        *olen = 2;
        break;
      case KS_BYTE2:
        if (c & 0x80) {
          stat->kanjiMode = KS_BYTE1;
          stat->lastByte = c;
        }
#ifdef KANJIBS
        else if (KanjiBS && c == CTRL_H) {
          buf[0] = c;
          *optr = buf;
          *olen = 1;
        }
#endif
        else {
          stat->kanjiMode = KS_NOKANJI;
          ostat->kanjiMode = KS_NOKANJI;
          if (isJIS(oc))
            strncpy(buf, codeMode[oc].socode, codeMode[oc].so_len);
          buf[codeMode[oc].so_len] = c;
          *optr = buf;
          *olen = codeMode[oc].so_len + 1;
        }
        break;
    }
    return;
  }
  /* JIS code */
  switch (stat->kanjiMode) {
    case KS_NOKANJI:
      if (c == ESC_CODE)
        stat->kanjiMode = KS_ESC;
      else {
        buf[0] = c;
        *optr = buf;
        *olen = 1;
      }
      break;
    case KS_BYTE1:
      if (c == ESC_CODE)
        break;
      stat->kanjiMode = KS_BYTE2;
      convCode(stat->lastByte, c, buf, ic, oc);
      *optr = buf;
      *olen = 2;
      break;
    case KS_BYTE2:
      if (c == ESC_CODE)
        stat->kanjiMode = KS_KESC;
#ifdef KANJIBS
      else if (KanjiBS && c == CTRL_H) {
        buf[0] = c;
        *optr = buf;
        *olen = 1;
      }
#endif
      else if (iscntrl(c)) {
        stat->kanjiMode = KS_NOKANJI;
        ostat->kanjiMode = KS_NOKANJI;
        if (isJIS(oc))
          strncpy(buf, codeMode[oc].socode, codeMode[oc].so_len);
        buf[codeMode[oc].so_len] = c;
        *optr = buf;
        *olen = codeMode[oc].so_len + 1;
      } else {
        stat->kanjiMode = KS_BYTE1;
        stat->lastByte = c;
      }
      break;
    case KS_ESC:
      if (c == '$')
        stat->kanjiMode = KS_ESCDL;
      else if (c == '(') /* ) */
        stat->kanjiMode = KS_ESCPAR;
      else {
        buf[0] = ESC_CODE;
        buf[1] = c;
        stat->kanjiMode = KS_NOKANJI;
        *optr = buf;
        *olen = 2;
      }
      break;
    case KS_KESC:
      if (c == '(') /* ) */
        stat->kanjiMode = KS_KESCPAR;
      else if (c == '$')
        stat->kanjiMode = KS_KESCDL;
      else {
        buf[0] = ESC_CODE;
        buf[1] = c;
        stat->kanjiMode = KS_BYTE1;
        *optr = buf;
        *olen = 2;
      }
      break;
    case KS_ESCDL:
      stat->kanjiMode = KS_BYTE2;
      ostat->kanjiMode = KS_BYTE2;
      *optr = codeMode[oc].sicode;
      *olen = codeMode[oc].si_len;
      break;
    case KS_KESCPAR:
      stat->kanjiMode = KS_NOKANJI;
      ostat->kanjiMode = KS_NOKANJI;
      *optr = codeMode[oc].socode;
      *olen = codeMode[oc].so_len;
      break;
    case KS_ESCPAR:
      /* ignore redundant escape sequences */
      stat->kanjiMode = KS_NOKANJI;
      break;
    case KS_KESCDL:
      /* ignore redundant escape sequences */
      stat->kanjiMode = KS_BYTE2;
      break;
  }
}

void
syncStatus(int ttyno,
           kanjicode ic,
           kanjicode oc,
           KanjiStatus* istatus,
           KanjiStatus* ostatus)
{
  if (!isJIS(oc) || istatus->kanjiMode == ostatus->kanjiMode)
    return;

  switch (istatus->kanjiMode) {
    case KS_NOKANJI:
    case KS_ESC:
    case KS_ESCDL:
      if (ostatus->kanjiMode != KS_NOKANJI) {
#ifdef DEBUG
        fprintf(wrlog,
                "%s(%s:%d)->%s(%s:%d): sync ",
                istatus->name,
                codeMode[ic].name,
                istatus->kanjiMode,
                ostatus->name,
                codeMode[oc].name,
                ostatus->kanjiMode);
        fputsEchoCtl(codeMode[oc].socode, codeMode[oc].so_len, wrlog);
        fputc('\n', wrlog);
#endif
        write(ttyno, codeMode[oc].socode, codeMode[oc].so_len);
        ostatus->kanjiMode = KS_NOKANJI;
      }
      break;
    default:
      if (ostatus->kanjiMode == KS_NOKANJI) {
#ifdef DEBUG
        fprintf(wrlog,
                "%s(%s:%d)->%s(%s:%d): sync",
                istatus->name,
                codeMode[ic].name,
                istatus->kanjiMode,
                ostatus->name,
                codeMode[oc].name,
                ostatus->kanjiMode);
        fputsEchoCtl(codeMode[oc].sicode, codeMode[oc].si_len, wrlog);
        fputc('\n', wrlog);
#endif
        write(ttyno, codeMode[oc].sicode, codeMode[oc].si_len);
        ostatus->kanjiMode = KS_BYTE2;
      }
      break;
  }
}

#define WBUFSIZ 512

void
writeTTYorg(int ttyno,
            const char* s,
            int l,
            kanjicode ic,
            kanjicode oc,
            KanjiStatus* istatus,
            KanjiStatus* ostatus)
{
  int i, j, b, len;
  char* p;
  static char wbuf[WBUFSIZ];

  b = 0;
  syncStatus(ttyno, ic, oc, istatus, ostatus);
  for (i = 0; i < l; i++) {
    checkStatusAndConvCode(s[i], ic, oc, istatus, ostatus, &p, &len);
    if (len > 0) {
      if (b + len > WBUFSIZ) {
        write(ttyno, wbuf, b);
        b = 0;
      }
      for (j = 0; j < len; j++)
        wbuf[b++] = p[j];
#ifdef DEBUG
      fprintf(wrlog,
              "%s(%s:%d)->%s(%s:%d): write ",
              istatus->name,
              codeMode[ic].name,
              istatus->kanjiMode,
              ostatus->name,
              codeMode[oc].name,
              ostatus->kanjiMode);
      fputsEchoCtl(p, len, wrlog);
      fputc('\n', wrlog);
#endif
    }
  }
  if (b > 0) {
    write(ttyno, wbuf, b);
  }
}

#ifdef DEBUG
fputsEchoCtl(p, len, wrlog) char* p;
int len;
FILE* wrlog;
{
  int j;
  char c;

  for (j = 0; j < len; j++) {
    c = p[j];
    if ((c & 0x80) == 0) {
      if (c < ' ')
        fprintf(wrlog, "^%c", c + '@');
      else
        fprintf(wrlog, "%c", c);
    } else
      fprintf(wrlog, "[%X]", ((int)c & 0xff));
  }
}
#endif

#ifndef DEBUG
static struct KanjiStatus SInter_Stat, TInter_Stat, To_SH_Stat, From_SH_Stat,
  To_TTY_Stat, From_TTY_Stat;
#else
static struct KanjiStatus SInter_Stat = { 0, 0, "SInter" },
                          TInter_Stat = { 0, 0, "TInter" },
                          To_SH_Stat = { 0, 0, ">SH" },
                          From_SH_Stat = { 0, 0, "SH>" },
                          To_TTY_Stat = { 0, 0, ">TTY" },
                          From_TTY_Stat = { 0, 0, "TTY>" };
#endif

void
writes(const char* s)
{
  int l = strlen(s);

  writeTTYorg(tty, s, l, INTERNALCODE, OutCode, &TInter_Stat, &To_TTY_Stat);
}

void
write1(char c)
{
  writeTTYorg(tty, &c, 1, INTERNALCODE, OutCode, &TInter_Stat, &To_TTY_Stat);
}

void
writeShTty(const char* s, int l)
{
  writeTTYorg(tty, s, l, WriteCode, OutCode, &From_SH_Stat, &To_TTY_Stat);
}

void
writeTty(const char* s, int l)
{
  writeTTYorg(tty, s, l, INTERNALCODE, OutCode, &TInter_Stat, &To_TTY_Stat);
}

void
writeShell1(char c)
{
  writeTTYorg(fileno(Shellout),
              &c,
              1,
              INTERNALCODE,
              WriteCode,
              &SInter_Stat,
              &To_SH_Stat);
}

void
writeShells(const char* s)
{
  int l = strlen(s);
  writeTTYorg(
    fileno(Shellout), s, l, INTERNALCODE, WriteCode, &SInter_Stat, &To_SH_Stat);
}

void
writeTtyShell1(char c)
{
  writeTTYorg(
    fileno(Shellout), &c, 1, OutCode, WriteCode, &From_TTY_Stat, &To_SH_Stat);
}

void
writeTtyShells(const char* s)
{
  int l = strlen(s);
  writeTTYorg(
    fileno(Shellout), s, l, OutCode, WriteCode, &From_TTY_Stat, &To_SH_Stat);
}

void
bell()
{
  write1('\007');
}

void
convCode(unsigned char c1,
         unsigned char c2,
         char* obuf,
         kanjicode ic,
         kanjicode oc)
{
  if (ic == oc || (isJIS(ic) && isJIS(oc))) {
    obuf[0] = c1;
    obuf[1] = c2;
    return;
  }
  if (ic != sj) {
    c1 &= 0x7f;
    c2 &= 0x7f;
    if (isJIS(oc)) {
      obuf[0] = c1;
      obuf[1] = c2;
      return;
    } else if (oc == euc) {
      obuf[0] = (c1 | 0x80);
      obuf[1] = (c2 | 0x80);
      return;
    } else { /* SJIS */
      c1 -= 0x20;
      c2 -= 0x20;

      if ((c1 & 1) == 0)
        c2 += 94;
      c1 = ((c1 - 1) >> 1) + 0x81;
      c2 += 0x3f;
      if (c1 > 0x9f)
        c1 += 0x40;
      if (c2 > 0x7e)
        c2++;
      obuf[0] = c1;
      obuf[1] = c2;
    }
  } else { /* ic == sj */
    if (c1 > 0x9f)
      c1 -= 0x9f;
    c1 = (c1 - 0x81) << 1;

    if (c2 > 0x7e)
      c2--;
    if (c2 > 0x9d) {
      c2 -= 0x9d;
      c1 += 2;
    } else {
      c2 -= 0x3f;
      c1++;
    }
    if (oc == euc) {
      obuf[0] = c1 + 0xa0;
      obuf[1] = c2 + 0xa0;
    } else {
      obuf[0] = c1 + 0x20;
      obuf[1] = c2 + 0x20;
    }
  }
}
