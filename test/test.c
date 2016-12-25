#include "test.h"

void dummy_error(const PDS_error_description *desc, void *unused)
{
    (void)unused;
    fprintf(stderr, "line %d:%d: %s\n", desc->number, desc->position, desc->msg);
}
