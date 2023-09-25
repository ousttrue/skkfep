/*
 * Configuration for NetBSD-1.2 (x68k)
 */

#define BSD		/* My machine is BSD system */
#define TERMIOS		/* My machine has new termios */

#define HAVE_SETENV	/* My machine has setenv() */
#define HAVE_BSD_OPENPTY /* BSD type openpty() */

#include <stdlib.h>	/* just in case of NetBSD/Alpha */
#include <util.h>	/* for openpty() */

#if makefile_parameter
SYSLIBS=-lutil
#endif
