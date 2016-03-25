#include <tinypds.h>
#include "test.h"

#define TINY_PDS_IMPL
#include <tinypds.h>

typedef struct
{
    int     base;
    int64_t value;
} integer_t;
 
int main()
{
    begin_test_data(integer_t)
        test_data(   " \t-714@_",  6, test_pair(10,   -714), PDS_OK ),
        test_data(        "bEeF",  4, test_pair(16, 0xbeef), PDS_OK ),
        test_data(    "10001111",  8, test_pair( 2,   0x8f), PDS_OK ),
        test_data(    "+1339/6 ",  4, test_pair( 8,     91), PDS_OK ),
        test_data(       "dummy",  0, test_pair( 7,      0), PDS_INVALID_VALUE ),
        test_data(  "21874836501234567890", 20, test_pair(10, INT64_MAX), PDS_INVALID_RANGE ),
        test_data( "-30000000000000000000", 21, test_pair(10, INT64_MIN), PDS_INVALID_RANGE ),
    end_test_data()

    size_t i;
    test_foreach(i)
    {
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
        int status = PDS_OK;
    
        int64_t value  = PDS_parse_int_base(first, last, &end, test_expected(i).base, &status);
        check(test_expected(i).value == value);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
