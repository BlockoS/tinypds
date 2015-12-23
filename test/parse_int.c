#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

typedef struct
{
	int     base;
	int32_t value;
} integer_t;
 
int main()
{
	static const integer_t expected[] = {
		{ 10,      -714 },
		{ 16,    0xbeef },
		{  2,      0x8f },
		{  8,        91 },
		{  7,         0 },
		{ 10, INT32_MAX },
		{ 10, INT32_MIN }
	};

    begin_test_data(integer_t)
        test_data(   " \t-714@_",  6, expected[0], PDS_OK ),
        test_data(        "bEeF",  4, expected[1], PDS_OK ),
        test_data(    "10001111",  8, expected[2], PDS_OK ),
        test_data(    "+1339/6 ",  4, expected[3], PDS_OK ),
        test_data(       "dummy",  0, expected[4], PDS_INVALID_VALUE ),
        test_data(  "2187483650", 10, expected[5], PDS_INVALID_RANGE ),
        test_data( "-3000000000", 11, expected[6], PDS_INVALID_RANGE ),
    end_test_data()

    int i;
    test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
		int status = PDS_OK;
    
        int32_t value  = parse_int(first, last, &end, test_expected(i).base, &status);
		
		check(test_expected(i).value == value);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
