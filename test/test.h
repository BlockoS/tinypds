#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tinypds.h>

#define check(e) if(!(e)) { fprintf(stderr, "%s+%d: test failed \"%s\"\n", __FILE__, __LINE__, #e); return EXIT_FAILURE; }

#define begin_test_data(expected_t) \
struct \
{ \
    const char *str; \
    off_t       end; \
    expected_t  expected; \
    int         status; \
} tv[] = {

#define test_data(s, e, x, a) { .str=s, .end=e, .expected=x, .status=a }

#define test_pair(a,b) { a, b }

#define end_test_data() };

#define test_str(i) (tv[(i)].str)
#define test_end(i) (tv[(i)].str+tv[(i)].end)
#define test_status(i) (tv[(i)].status)
#define test_expected(i) (tv[(i)].expected)
#define test_foreach(i) for(i=0; i<(sizeof(tv)/sizeof(tv[0])); ++i) 

void dummy_error(const PDS_error_description* desc, void *user_data);

#endif /* TEST_H */
