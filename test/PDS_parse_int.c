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

	PDS_parser parser;
	parser.error = dummy_error;

	int i;
    test_foreach(i)
	{
		parser.line    = 1;
        parser.first   = test_str(i);
        parser.last    = parser.first + strlen(parser.first) - 1;
        parser.current = parser.first;
		parser.status  = PDS_OK;
        
		int ret = PDS_parse_int(&parser);
        check(test_status(i) == parser.status);
        if(ret)
		{
			check(test_end(i) == parser.current);
			check(PDS_INTEGER_VALUE == parser.scalar.type);
			check(test_expected(i) == parser.scalar.integer.value);
		}
	}
    return EXIT_SUCCESS;
}
