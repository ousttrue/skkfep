/*
 * Configuration for NeXT (NEXTSTEP 3.0J)
 */

#define BSD		/* My machine is BSD-like system */
			/* but no TIOCSCTTY, like SysV */

/*#define TERMIOS*/	/* Has but partial */

#ifdef __STRICT_BSD__	/* with -bsd */
# define INT_SIGNAL	/* signal() returns (int (*)()) */
# define OLD_CTYPE	/* old tolower(), toupper() */
#endif

#define NO_MALLOC_H	/* no <malloc.h>  */
			/* (has <sys/malloc.h> but not needed) */
