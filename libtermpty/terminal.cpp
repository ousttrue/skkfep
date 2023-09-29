#include "terminal.h"
#include "statusline.h"
#include <termcap.h>

bool
Capability::getstr(const char* name)
{
  char buf[1024];
  auto bp = buf;
  if (auto str = tgetstr(name, &bp)) {
    seq = str;
    return true;
  }
  return false;
}

Capability::operator bool() const
{
  return seq.size() > 0;
}

std::string_view
Capability::xy(int x, int y) const
{
  m_buf = tgoto(seq.c_str(), x, y);
  return m_buf;
}

bool
Terminal::load(const char* term)
{
  if (!term) {
    return false;
  }

  char entry[1024];
  auto ret = tgetent(entry, term);
  if (ret < 0) {
    // error
    return false;
  }
  if (ret == 0) {
    // not found
    return false;
  }

  ce.getstr("ce");        /* clear to the end of line */
  if (!kr.getstr("nd")) { /* cursor right */
    kr.getstr("kr");
  }
  if (tgetflag("bs"))
    kl.seq = "\b"; /* cursor left */
  else {
    if (!kl.getstr("le")) {
      if (!kl.getstr("kb")) {
        kl.getstr("kl");
      }
    }
  }

  cr.getstr("cr"); /* carrige return */
  ta.getstr("ta"); /* tab */
  bt.getstr("bt"); /* backtab */

  sc.getstr("sc"); /* save cursor */
  rc.getstr("rc"); /* restore cursor */
  if (!sc || !rc) {
    fprintf(stderr, "Your terminal cannot save/restore cursor.\n");
    return false;
  }

  so.getstr("so");        /* standout mode */
  se.getstr("se");        /* standout mode end */
  us.getstr("us");        /* underline mode */
  ue.getstr("ue");        /* underline mode end */
  cl.getstr("cl");        /* clear screen */
  cm.getstr("cm");        /* cursor motion */
  cs.getstr("cs");        /* change scroll region */
  if (!ts.getstr("ts")) { /* to status line */
    // ts.seq = "\033[?T";
  }
  if (!fs.getstr("fs")) { /* from status line */
    // fs.seq = "\033[?F";
  }
  if (!ss.getstr("ss")) { /* show status line */
    ss.seq = "\033[?S";
  }
  if (!hs.getstr("hs")) {   /* hide status line */
    if (!hs.getstr("ds")) { /* disable status line */
      hs.seq = "\033[?H";
    }
  }
  es.getstr("es"); /* erase status line */

  if (ts && fs) {
    status::type(StatusType::HaveStatusLine);
  } else if (cs && cm && sc && rc) {
    status::type(StatusType::UseBottomLine);
  } else {
    status::type(StatusType::NoStatusLine);
  }

  return true;
}
