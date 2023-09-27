#include "app.h"
#include "keybind.h"
#include "kkconv.h"
#include "readwrite.h"
#include "terms.h"
#include "version.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

char UserDicName[64];

#define DEFAULT_SHELL "/bin/bash"
char ShellName[64] = "";
char** ShellArg = NULL;

static kanjicode
decideCode(const char* s)
{
  if (!strcmp(s, "sj"))
    return sj;
  else if (!strcmp(s, "euc"))
    return euc;
  else if (!strncmp(s, "jis", 3)) {
    switch (s[3]) {
      case 'A':
      case '@':
        switch (s[4]) {
          case 'H':
            return jisAH;
          case 'J':
            return jisAJ;
          case 'B':
            return jisAB;
        }
        break;
      case 'B':
        switch (s[4]) {
          case 'H':
            return jisBH;
          case 'J':
            return jisBJ;
          case 'B':
            return jisBB;
        }
        break;
    }
  }
  return jisAJ; /* default */
}

static void
ParseArgs(int argc, char** argv)
{
  for (int i = 1; i < argc; i++) {
    if (!strncmp(argv[i], "-o", 2))
      OutCode = decideCode(argv[i] + 2);
    else if (!strncmp(argv[i], "-f", 2))
      WriteCode = decideCode(argv[i] + 2);
    else if (!strncmp(argv[i], "-k", 2))
      KanaKey = argv[i] + 2;
    else if (!strcmp(argv[i], "-udic"))
      strcpy(UserDicName, argv[++i]);
    else if (!strcmp(argv[i], "-bs"))
      KanjiBS = ~KANJIBS_DEFAULT;
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

static const char*
kanjicode2string(int code)
{
  switch (code) {
    case euc:
      return "EUC";
    case sj:
      return "MS-KANJI(ShiftJIS)";
    case jisAH:
      return "JIS(@H)";
    case jisAJ:
      return "JIS(@J)";
    case jisAB:
      return "JIS(@B)";
    case jisBH:
      return "JIS(BH)";
    case jisBJ:
      return "JIS(BJ)";
    case jisBB:
      return "JIS(BB)";
  }

  return "error";
}

int
main(int argc, char* argv[], char* envp[])
{
  ParseArgs(argc, argv);

  printf("SKKFEP version %s\n", version);
  printf("Display: %s, FileIO: %s.\n",
         kanjicode2string(OutCode),
         kanjicode2string(WriteCode));

  if (!App::Instance().Initialize(UserDicName, ShellName, ShellArg)) {
    return 1;
  }

  // mainloop
  return App::Instance().Run();
}
