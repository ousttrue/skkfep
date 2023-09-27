#include "app.h"
#include "connsh.h"
#include "skklib.h"
#include <signal.h>
#include <sstream>

#define USER_DIC_NAME ".skk/SKK-JISYO.L"

App::App(){}

App::~App(){}

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
App::PtyFree()
{
  freeDevice();
}

void
App::OpenDictionary(const char* path)
{
  if (path && path[0]) {
    std::stringstream ss;
    ss << getenv("HOME") << "/" << USER_DIC_NAME;
    UserDicName = ss.str();
  }
  UserDic = openSKK(UserDicName.c_str());
}

void
App::SaveJisyo()
{
  if (UserDic) {
    printf("Saving JISYO...\n");
    closeSKK(UserDic, UserDicName.c_str());
  }
}
