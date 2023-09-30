#include "signal_setup.h"
#include "connsh.h"
// #include "etc.h"
#include "statusline.h"
#include "stty.h"
#include "terms.h"
#include <signal.h>

#ifndef NO_SUSPEND
void
suspend(int)
{
  terminal::reset_tty_without_close();
  signal(SIGTSTP, SIG_DFL);
  kill(0, SIGTSTP);
}

void
susp_cont(int)
{
  signal(SIGTSTP, suspend);
  terminal::reset_tty_without_close();
  if (!terminal::set_tty()) {
    // App::Instance().Exit(-1);
    exit(-1);
  }
  kill(child::ShellPID, SIGCONT);
}
#endif

void
winchange(int)
{
  signal(SIGWINCH, SIG_IGN);
  auto size = get_termsize();
  if (status::type() == StatusType::UseBottomLine) {
    --size.Rows;
  }

  int tty;
  if (child::Shellout != NULL)
    tty = fileno(child::Shellout);
  else
    tty = 1; /* stdout */
  child::set_termsize(tty, size);
  initFep();
  // showcurmode();
  signal(SIGWINCH, winchange);
}

void
sig_usr1(int)
{
  signal(SIGUSR1, SIG_IGN);
  initFep();
  // g_skk.toggleEscape(ViEsc);
  signal(SIGUSR1, sig_usr1);
}

void
sig_usr2(int)
{
  signal(SIGUSR2, SIG_IGN);
  initFep();
  // g_skk.toggleEscape(EmacsEsc);
  signal(SIGUSR2, sig_usr2);
}

void
sig_int(int)
{
  signal(SIGHUP, SIG_IGN);
  initFep();
  // g_skk.toggleEscape(NoEsc);
  signal(SIGHUP, sig_int);
}

std::function<void(const char*)> g_reset;

void
init_signals(const std::function<void(const char*)>& reset)
{
  g_reset = reset;

  signal(SIGHUP, [](int) { g_reset("Hungup"); });
  signal(SIGINT, sig_int);
  /*	signal(SIGQUIT,iot_exit); */
  signal(SIGILL, [](int) {
      terminal::reset_tty();
    signal(SIGCHLD, SIG_DFL);
    // App::Instance().Exit(-1);
    exit(-1);
  });
  signal(SIGIOT, [](int) {
      terminal::reset_tty();
    fprintf(stderr, "Abort.\n");
    // App::Instance().Abort();
    abort();
  });
  signal(SIGFPE, [](int) {
      terminal::reset_tty();
    signal(SIGCHLD, SIG_DFL);
    // App::Instance().Exit(-1);
    exit(-1);
  });
  signal(SIGSEGV, [](int) {
      terminal::reset_tty();
    fprintf(stderr, "SEGMENTATION VIOLATION\n");
    // App::Instance().Abort();
    abort();
  });
  signal(SIGPIPE, [](int) { g_reset("Pipe down"); });
  signal(SIGTERM, [](int) { g_reset("Terminate"); });
#ifndef NO_SUSPEND
  signal(SIGTSTP, suspend);
  signal(SIGCONT, susp_cont);
#endif
  signal(SIGWINCH, winchange);
  signal(SIGUSR1, sig_usr1);
  signal(SIGUSR2, sig_usr2);
}
