/*
 * Configuration for FreeBSD
 */

#define BSD		/* My machine is BSD-like system */
#define TERMIOS		/* My machine has new termios */

#define HAVE_SETENV	/* My machine has setenv() */

#define HAVE_BSD_OPENPTY /* BSD type openpty() */
/*#include <libutil.h>	/* for openpty() */

#if makefile_parameter
SYSLIBS=-lutil
#endif
