/*
 * Configuration for BSDI BSD/OS 2.0.1
 */

#define BSD		/* My machine is BSD-like system */
#define TERMIOS		/* My machine has new termios */

#define HAVE_SETENV	/* My machine has setenv() */

#define NO_MALLOC_H	/* no <malloc.h>  */
			/* (has <sys/malloc.h> but not needed) */
#define HAVE_BSD_OPENPTY /* BSD type openpty() */

#if makefile_parameter
SYSLIBS=-lutil
#endif
