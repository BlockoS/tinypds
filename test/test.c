#include "test.h"

void dummy_error(int line, const char *msg, void *unused)
{
    (void)unused;
    fprintf(stderr, "line %d: %s\n", line, msg);
}
