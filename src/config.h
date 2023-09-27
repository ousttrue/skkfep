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

#define KANJIBS               /* do/don't shuft-out BS code in JIS mode */
#define KANJIBS_DEFAULT 0     /* 0 -> shift out / 1 -> don't shift out  */
#define PRESERVE_ON_FAILURE 1 /* preserve candidate on conversion failure */

#define NET_LEARN_MODE LearnPermanent /* Write back words obtained from */
                                      /* SKK server			  */
