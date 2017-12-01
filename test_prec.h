#ifndef test_prec_h
#define test_prec_h

#include <stdio.h>
#include <stdlib.h>
#include "prec.h"

#define	RED	"\x1b[31m"
#define	GREEN	"\x1b[32m"
#define	RESET	"\x1b[0m"

#define TEST_EQUAL(name,value1,value2)                                                  \
	{                                                                                   \
        if (value1!=value2)                                                                 \
            {                                                                               \
            count_errors++;                                                                 \
            printf(RED  "Failed test - %s, line: %d!",name, __LINE__);                      \
            printf(RESET    "\n");                                                          \
            }                                                                               \
        else                                                                                \
            {                                                                               \
            count_ok++;                                                                     \
            printf(GREEN    "Passed test - %s, line: %d!", name, __LINE__);                 \
            printf(RESET    "\n");                                                          \
            }                                                                               \
}

#endif
