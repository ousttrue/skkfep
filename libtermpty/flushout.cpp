#include "flushout.h"
#include "connsh.h"
#include "stty.h"
#include <sys/select.h>
#include <time.h>

#define FLUSH_TIMEOUT 300000 /* timeout ( micro-sec ) */

void
flushOut(int minchar)
{
  int i, nchar, ntimeout;
  fd_set readfds;
  struct timeval timeout;

  FD_ZERO(&readfds);
  FD_SET(child::Shellfd, &readfds);
  timeout.tv_sec = FLUSH_TIMEOUT / 1000000;
  timeout.tv_usec = FLUSH_TIMEOUT % 1000000;
  ntimeout = 0;
  nchar = 0;

  char shellBuf[256];
  while (ntimeout < minchar) {
    i = select(child::Shellfd + 1, &readfds, NULL, NULL, &timeout);
    if (i == -1 && errno == EINTR)
      continue;
    if (i > 0) {
      i = read(child::Shellfd, shellBuf, std::size(shellBuf));
      terminal::writeShTty(shellBuf, i);
      nchar += i;
      if (nchar >= minchar)
        break;
    } else
      ntimeout++;
  }
}
