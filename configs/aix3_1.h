/*
 * Configuration for AIX 3.1
 */

/*#define BSD		/* My machine is BSD-like system */
#define SYSV		/* My machine is System-V */
/*#define SVR3		/* System-V Release 3 based system */
/*#define SVR4		/* System-V Release 4 based system */
/*#define BSD42		/* Old 4.2-BSD (not 4.3 or later) */
/*#define HPUX		/* My machine is HPUX */
#define AIX		/* AIX 3.x */
/*#define APOLLO	/* Old APOLLO DOMAIN-IX OS */
/*#define OLD_SELECT	/* 4.2-type select(), no FD_SET(),FD_CLR,etc. */
/*#define INT_SIGNAL	/* signal() returns (int*) */
/*#define NO_USLEEP	/* My machine has no usleep() */
#define TERMIOS		/* My machine has new termios */
/*#define TERMIO	/* System-V termio */
#define TERMINFO	/* My machine uses terminfo */
/*#define NO_SUSPEND	/* My machine has no suspend facility */
#define SYS_SELECT	/* My machine has <sys/select.h> */

#define HAVE_PUTENV	/* My machine has putenv() */
/*#define HAVE_SETENV	/* My machine has setenv() instead of putenv() */
#define NO_VFORK	/* My machine has no vfork() */
/*#define NO_SETEUID 	/* My machine has no seteuid() */
#define HAVE_SETREUID 	/* My machine has setreuid() */
#define HAVE_SETRESUID 	/* My machine has setresuid() */
/*#define OLD_CTYPE	/* old tolower(), toupper() */

#if makefile_parameter
SYSDEFINE=-D_BSD
#endif
