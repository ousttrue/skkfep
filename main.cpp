#include "app.h"
#include "keybind.h"
#include "kkconv.h"
#include "terms.h"
#include "version.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

std::string UserDicName;

#define DEFAULT_SHELL "/bin/bash"
char ShellName[64] = "";
char** ShellArg = NULL;

static void
ParseArgs(int argc, char** argv)
{
  for (int i = 1; i < argc; i++) {
    if (!strncmp(argv[i], "-k", 2))
      KanaKey = argv[i] + 2;
    else if (!strcmp(argv[i], "-udic"))
      UserDicName = argv[++i];
    else if (!strcmp(argv[i], "-e")) {
      strcpy(ShellName, argv[++i]);
      ShellArg = argv + i;
      break;
    } else if (!strcmp(argv[i], "-P")) {
      PreserveOnFailure ^= 1;
    } else if (!strcmp(argv[i], "-esc")) {
      setEscape(SimpleEsc);
      LastEscapeBehavior = NoEsc;
    } else if (!strcmp(argv[i], "-viesc")) {
      setEscape(ViEsc);
      LastEscapeBehavior = NoEsc;
    } else if (!strcmp(argv[i], "-emacsesc")) {
      setEscape(EmacsEsc);
      LastEscapeBehavior = NoEsc;
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
      exit(1);
    }
  }

  if (ShellName[0] == '\0') {
    if (auto p = getenv("SHELL")) {
      strcpy(ShellName, p);
      ShellArg = NULL;
    } else {
      auto pwent = getpwuid(getuid());
      if (pwent && pwent->pw_shell)
        strcpy(ShellName, pwent->pw_shell);
      if (ShellName[0] == '\0')
        strcpy(ShellName, DEFAULT_SHELL);
      ShellArg = NULL;
    }
  }
}

int
main(int argc, char* argv[], char* envp[])
{
  ParseArgs(argc, argv);

  printf("SKKFEP version %s\n", version);

  if (!App::Instance().Initialize(UserDicName, ShellName, ShellArg)) {
    return 1;
  }

  // mainloop
  return App::Instance().Run();
}
