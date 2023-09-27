/*
 * Configuration for SKKFEP
 */

#include "configs/linux.h"

#if makefile_parameter
CC = gcc DEFINE = -DUSE_LOCALE OPTIMIZE = -g EXTRALIBS =
#endif

#define DEFAULT_KANAKEY "^j"

/* By Y. Kaneko */
/*#define KUTOUTEN        /* "."->maru, ","->ten */


#define NET_LEARN_MODE LearnPermanent /* Write back words obtained from */
                                      /* SKK server			  */
