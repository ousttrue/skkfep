#include "app.h"
#include "conversion/kkconv.h"
#include "statusline.h"
#include "version.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

struct Args
{
  std::string KanaKey;
  std::string UserDicName;
  std::string ShellName;
  char** ShellArg = NULL;

  bool Parse(int argc, char** argv)
  {
    if (auto p = getenv("SHELL")) {
      ShellName = p;
    } else {
      auto pwent = getpwuid(getuid());
      if (pwent && pwent->pw_shell)
        ShellName = pwent->pw_shell;
      if (ShellName.empty())
        ShellName = "/bin/bash";
    }

    for (int i = 1; i < argc; i++) {
      if (!strncmp(argv[i], "-k", 2))
        KanaKey = argv[i] + 2;
      else if (!strcmp(argv[i], "-udic"))
        UserDicName = argv[++i];
      else if (!strcmp(argv[i], "-e")) {
        ShellName = argv[++i];
        ShellArg = argv + i;
        break;
      } else if (!strcmp(argv[i], "-P")) {
        PreserveOnFailure ^= 1;
      } else if (!strcmp(argv[i], "-rs")) {
        ReverseStatus = 1;
      }
#ifdef DEBUG
      else if (!strcmp(argv[i], "-deb"))
        debfile = argv[++i];
#endif
      else {
        fprintf(stderr, "skkfep: bad option %s\n", argv[i]);
        fprintf(stderr, "\
usage: skkfep [options]\n\
options:\n\
	-o{euc|sj|jis{A@B}{HJB}}	DISPLAY KANJI CODE\n\
	-f{euc|sj|jis{A@B}{HJB}}	FILEIO KANJI CODE\n\
	-udic <dictionary>		USER DICTIONARY\n\
	-bs				TOGGLE BS BEHAVIOR\n\
	-k<char>			SKK TOGGLE KEY\n\
	-e cmd				EXECUTED COMMAND\n\
	-lm {none|temporal|permanent}   LEARN MODE\n\
	-P				TOGGLE BEHAVIOR OF CONVERSION FAILURE\n\
	-esc				ENABLE THROUGH ESCAPE\n\
	-viesc				ENABLE THROUGH ESCAPE and iaoIASR\n\
	-emacsesc			ENABLE THROUGH ESCAPE and etc.\n\
	-rs				REVERSE STATUS LINE\n\
");
        return false;
      }
    }

    if (ShellName[0] == '\0') {
    }

    return true;
  }
};

int
main(int argc, char* argv[], char* envp[])
{
  Args args;
  if (!args.Parse(argc, argv)) {
    return 1;
  }

  printf("SKKFEP version %s\n", version);

  if (!App::Instance().Initialize(
        args.UserDicName, args.ShellName, args.ShellArg, version)) {
    return 2;
  }

  // mainloop
  return App::Instance().Run();
}
