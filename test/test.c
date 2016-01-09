#include "test.h"

void dummy_error(int line, const char *msg, void *user_data)
{
    fprintf(stderr, "line %d: %s\n", line, msg);
}
