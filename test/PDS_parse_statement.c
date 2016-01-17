#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
   
// [todo] expected structure

int main()
{
    begin_test_data(int32_t)
#if 0
        test_data(" \r\n        group\t = dummy_group_0\r\n", 20, 1, PDS_OK ),
        test_data(       "end_group=dummy_group_0\t    \r\n", 20, 1, PDS_OK ),
        test_data(                      "OBJECT = IMAGE\r\n", 20, 1, PDS_OK ),
        test_data(                  "END_OBJECT = IMAGE\r\n", 20, 1, PDS_OK ),
        test_data("^image /* comment */ = \t 327128    \r\n", 25, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 'dummy' \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 1234 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 2#-1010# \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = -32 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = +.82e4 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = .2 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 100.4e1 \r\n", 28, 1, PDS_OK ),
        test_data("ELEVATION_FOV                 = 12.000 <DEGREES>\r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 2016-01-3T18:21:49.1299+2:10 \r\n", 28, 1, PDS_OK ),
#endif
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = {'element#0'\t,\r\n'element#1'    }\r\n", 28, 1, PDS_OK ),
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
        
		int ret = PDS_parse_statement(&parser);
        check(test_status(i) == parser.status);
        if(ret)
		{
//			check(test_end(i) == parser.current);
		}
	}
    return EXIT_SUCCESS;
}
