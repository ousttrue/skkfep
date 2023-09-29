#include "app.h"
#include "connsh.h"
#include "fep.h"
#include "keybind.h"
#include "kkconv.h"
// #include "skklib.h"
#include "dictinary.h"
#include "stty.h"
#include "terms.h"
#include <signal.h>
#include <sstream>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <wait.h>

#define USER_DIC_NAME ".skk/SKK-JISYO.L"

#define IF_STOPPED(x) WIFSTOPPED(x)

App::App() {}

App::~App() {}

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
  reset_tty();
  fprintf(stderr, "%s\n", msg);
  Exit(-1);
}

void
App::PtyFree()
{
  freeDevice();
}

void
App::OpenDictionary(std::string_view path)
{
  if (path.empty()) {
    std::stringstream ss;
    ss << getenv("HOME") << "/" << USER_DIC_NAME;
    UserDicName = ss.str();
  }
  auto d = new Dictionary;
  if (d->load(UserDicName)) {
    UserDic = d;
  }
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
App::Initialize(std::string_view UserDicName, const char* cmd, char** args)
{
  /* Initialize */
  setKanaKey();
  if (!getTCstr()) {
    App::Instance().Exit(-1);
  }
  tty_ready();
  get_winsize();
  if (!set_tty()) {
    App::Instance().Exit(-1);
  }
  init_signals();

  OpenDictionary(UserDicName);

  toAsc({});

  auto ok = establishShell(cmd, args, [](int) {
    int cpid;
    int statusp;

    reset_tty_without_close();
#ifndef NO_SUSPEND
    cpid = wait((int*)&statusp);
    if (cpid != -1 && IF_STOPPED(statusp)) { /* suspend */
      kill(0, SIGTSTP);
    } else
#endif /* NO_SUSPEND */
      App::Instance().Exit(0);
  });
  return ok;
}

#define SH_BUF_SIZ 256

int
App::Run()
{
  // char c;
  // int i;
  fd_set selfds;
  // guess_system_kanji_code();
  //

  FD_ZERO(&selfds);
  int fdnum = Shellfd + 1;

  char shellBuf[SH_BUF_SIZ];

  /* Loop */
  for (;;) {
    FD_SET(0, &selfds);
    FD_SET(Shellfd, &selfds);
    int i = select(fdnum, &selfds, NULL, NULL, NULL);
    if (i == -1 && errno == EINTR)
      continue;
    if (FD_ISSET(Shellfd, &selfds) && !BlockTty) {
      /* Shell input is ready */
      if ((i = read(Shellfd, shellBuf, SH_BUF_SIZ)) > 0)
        writeShTty(shellBuf, i);
      if (i == SH_BUF_SIZ)
        continue;
    }
    if (!FD_ISSET(0, &selfds)) { /* Key input not ready */
      continue;
    }
    if (ioctl(0, FIONREAD, &i) == 0) {
      while (i) {
        char o = OkuriFirst;
        OkuriFirst = 0;
        int c = getchar();
        if (c & 0x80)
          thru(c);
        else
          (*CurrentKeymap[c])(c /*, o*/);
        i--;
      }
    }
  }
}
