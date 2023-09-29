#include "termsize.h"
#include "stty.h"
#include <sys/ioctl.h>

TermSize g_size = {};
bool ReverseStatus = true;

TermSize
current_termsize()
{
  return g_size;
}

TermSize
get_termsize()
{
  struct winsize ws;
  ioctl(g_term_tty, TIOCGWINSZ, &ws);
  g_size.Rows = ws.ws_row;
  g_size.Cols = ws.ws_col;
  return g_size;
}
