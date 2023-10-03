#include "app.h"
#include "connsh.h"
#include "dictinary.h"
#include "kkconv.h"
#include "signal_setup.h"
#include "skk.h"
#include "statusline.h"
#include "stty.h"
#include "terms.h"
#include "termsize.h"
#include <signal.h>
#include <sstream>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <wait.h>

#define IF_STOPPED(x) WIFSTOPPED(x)

App::
App()
{
}

App::~
App()
{
}

void
App::Exit(int code)
{
  PtyFree();
  SaveJisyo();
  exit(code);
}

void
App::Abort()
{
  PtyFree();
  SaveJisyo();
  signal(SIGQUIT, SIG_DFL);
  signal(SIGIOT, SIG_DFL);
  abort();
}

void
App::Reset(const char* msg)
{
  terminal::reset_tty();
  fprintf(stderr, "%s\n", msg);
  Exit(-1);
}

void
App::PtyFree()
{
  child::freeDevice();
}

void
App::SaveJisyo()
{
  if (UserDic) {
    printf("Saving JISYO...\n");
    delete UserDic;
  }
}

bool
App::Initialize(const std::string& UserDicName,
                const std::string& cmd,
                char** args,
                const char* version)
{
  if (!getTCstr()) {
    App::Instance().Exit(-1);
  }
  terminal::tty_ready();
  if (!terminal::set_tty()) {
    App::Instance().Exit(-1);
  }
  auto win_size = get_termsize();
  if (status::type() == StatusType::UseBottomLine) {
    --win_size.Rows;
  }

  // fork
  init_signals([](const char* msg) { App::Instance().Reset(msg); });
  auto callback = [](int) {
    int cpid;
    int statusp;

    terminal::reset_tty_without_close();
#ifndef NO_SUSPEND
    cpid = wait((int*)&statusp);
    if (cpid != -1 && IF_STOPPED(statusp)) { /* suspend */
      kill(0, SIGTSTP);
    } else
#endif /* NO_SUSPEND */
      App::Instance().Exit(0);
  };
  if (!child::establishShell(win_size, cmd, args, callback, version)) {
    return false;
  }

  m_skk.open_dictionary(UserDicName);
  m_skk.initialize();
  initFep();

  return true;
}

#define SH_BUF_SIZ 256

int
App::Run()
{
  fd_set selfds;
  FD_ZERO(&selfds);
  int fdnum = child::Shellfd + 1;

  std::string last_statusline;
  char okuri = 0;
  for (;;) {
    FD_SET(0, &selfds);
    FD_SET(child::Shellfd, &selfds);
    int i = select(fdnum, &selfds, NULL, NULL, NULL);
    if (i == -1 && errno == EINTR) {
      continue;
    }

    // read pty out
    if (FD_ISSET(child::Shellfd, &selfds)) {
      if (!BlockTty) {
        char shellBuf[SH_BUF_SIZ];
        auto i = read(child::Shellfd, shellBuf, SH_BUF_SIZ);
        if (i > 0) {
          terminal::writeShTty(shellBuf, i);
        }
      }
      continue;
    }

    // read stdin
    if (FD_ISSET(0, &selfds)) {
      int i;
      if (ioctl(0, FIONREAD, &i) == 0) {
        for (; i; --i) {
          OkuriFirst = 0;

          int c = getchar();

          if (c & 0x80) {
            // not ascii
            // thru(c);
            write(fileno(child::Shellout), &c, 1);
          } else {
            // ascii
            // key input may has side effect
            auto [confirmed, unconfirmed] = m_skk.input(c, okuri);

            if (unconfirmed.size()) {
              // TODO: decoration. ▽, ▼ standout, underline...etc
              terminal::writes(unconfirmed);
            }

            if (confirmed.size()) {
              child::writeShells(confirmed);
            }

            auto statusline = m_skk.statusline();
            if (statusline != last_statusline) {
              showmessage(statusline);
              last_statusline = statusline;
            }

            okuri = OkuriFirst;
          }
        }
      }
    }
  }
}
