#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    

int main()
{
    begin_test_data(int32_t)
        test_data(   "   511-", 6,       511, PDS_OK ),
        test_data("2#+0101#@ ", 8,         5, PDS_OK ),
        test_data(    "8#foo#", 2,         0, PDS_INVALID_VALUE ),
        test_data(   "10#-9a#", 5, INT32_MAX, PDS_INVALID_VALUE ), 
        test_data(    "16#ffc", 6, INT32_MAX, PDS_INVALID_VALUE ), 
        test_data(       "4##", 2,         0, PDS_INVALID_VALUE ),
    end_test_data()

	int i;
    test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
		int status = PDS_OK;
        
		int32_t value = PDS_parse_int(first, last, &end, &status);
        
		check(test_expected(i) == value);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
