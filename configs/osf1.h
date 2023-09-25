/*
 * Configuration for OSF/1
 */

#include <sys/param.h>	/* for BSD macro */
#ifndef BSD
# define BSD		/* My machine is BSD-like system */
#endif
/*#define SYSV		/* My machine is System-V */
/*#define SVR3		/* System-V Release 3 based system */
/*#define SVR4		/* System-V Release 4 based system */
/*#define BSD42		/* Old 4.2-BSD (not 4.3 or later) */
/*#define HPUX		/* My machine is HPUX */
/*#define APOLLO	/* Old APOLLO DOMAIN-IX OS */
/*#define OLD_SELECT	/* 4.2-type select(), no FD_SET(),FD_CLR,etc. */
/*#define INT_SIGNAL	/* signal() returns (int*) */
/*#define NO_USLEEP	/* My machine has no usleep() */
#define TERMIOS		/* My machine has new termios */
/*#define TERMIO	/* System-V termio */
/*#define TERMINFO	/* My machine uses terminfo */

#define HAVE_PUTENV	/* My machine has putenv() */
/*#define HAVE_SETENV	/* My machine has setenv() instead of putenv() */
/*#define NO_VFORK	/* My machine has no vfork() */
/*#define NO_SETEUID 	/* My machine has no seteuid() */
/*#define HAVE_SETREUID 	/* My machine has setreuid() */
/*#define HAVE_SETRESUID 	/* My machine has setresuid() */
/*#define OLD_CTYPE	/* old tolower(), toupper() */
#define HAVE_BSD_OPENPTY /* openpty() with BSD-like interface */

#include <stdlib.h>	/* Alpha AXP is a 64bit archtecture, */
#include <string.h>	/* and missing of function declarations */
			/* may cause serious troubles */
#if makefile_parameter
SYSDEFINE=-D_BSD
#endif
