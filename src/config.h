/*
 * Configuration for SKKFEP
 */

#include "configs/linux.h"

#if makefile_parameter
CC = gcc DEFINE = -DUSE_LOCALE OPTIMIZE = -g EXTRALIBS =
#endif

#define DEFAULT_KANAKEY "^j"

#define NET_LEARN_MODE LearnPermanent /* Write back words obtained from */
                                      /* SKK server			  */
