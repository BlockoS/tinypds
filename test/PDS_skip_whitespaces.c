#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h> 
	
typedef struct
{
	int ret;
	int line;
} expected_t;

int main()
{
	begin_test_data(expected_t)
		test_data(                                        "nothing to skip!",  0, test_pair(1,1), PDS_OK ),
		test_data(                                          "\t    \r\n end",  8, test_pair(1,2), PDS_OK ),
		test_data(                                                "/**/ end",  5, test_pair(1,1), PDS_OK ),
		test_data(                   "/* a\tlonger\tcomment */\t\r\n\r\nend", 27, test_pair(1,3), PDS_OK ),
		test_data("  \t/*line 1 */\n \t\t/*line 2*/\n \t\t\t/*line 3*/\nend", 44, test_pair(1,4), PDS_OK ),
		test_data(                               "  /* /* nested comment */",  5, test_pair(0,1), PDS_INVALID_VALUE ),
		test_data(                            "  /* muti-line \n comment */", 15, test_pair(0,1), PDS_INVALID_VALUE ),
	end_test_data()

	PDS_parser parser;
	parser.error = dummy_error;

	int i;
	test_foreach(i)
	{
		parser.line    = 1;
		parser.first   = test_str(i);
		parser.current = parser.first;
		parser.last    = parser.first + strlen(parser.first) - 1;
		parser.status  = PDS_OK;
		int ret;
		
		ret = PDS_skip_whitespaces(&parser);
		
		check(test_status(i) == parser.status);	
		check(test_end(i) == parser.current);	
		check(test_expected(i).ret == ret);	
		check(test_expected(i).line == parser.line);	
	}

	return EXIT_SUCCESS;
}
