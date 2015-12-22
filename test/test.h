#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check(e) if(!(e)) { fprintf(stderr, "%s+%d: test failed \"%s\"\n", __FILE__, __LINE__, #e); return EXIT_FAILURE; }

#endif /* TEST_H */
