#include "readwrite.h"
#include "connsh.h"
#include "ctrlcode.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int tty;

#define WBUFSIZ 512

static void
writeTTYorg(int ttyno, const char* s, int l)
{
  write(ttyno, s, l);
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

void
writes(const char* s)
{
  int l = strlen(s);

  writeTTYorg(tty, s, l);
}

void
write1(char c)
{
  writeTTYorg(tty, &c, 1);
}

void
writeShTty(const char* s, int l)
{
  writeTTYorg(tty, s, l);
}

void
writeTty(const char* s, int l)
{
  writeTTYorg(tty, s, l);
}

void
writeShell1(char c)
{
  writeTTYorg(fileno(Shellout), &c, 1);
}

void
writeShells(const char* s)
{
  int l = strlen(s);
  writeTTYorg(fileno(Shellout), s, l);
}

void
writeTtyShell1(char c)
{
  writeTTYorg(fileno(Shellout), &c, 1);
}

void
writeTtyShells(const char* s)
{
  int l = strlen(s);
  writeTTYorg(fileno(Shellout), s, l);
}

void
bell()
{
  write1('\007');
}
