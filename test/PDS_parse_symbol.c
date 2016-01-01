#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    

int main()
{
    begin_test_data(int)
        test_data( "' \t _/<@>)?!#$.;'", 17, 1, PDS_OK ),
        test_data(   "'Itchy&Scratchy'", 16, 1, PDS_OK ),
        test_data("'New York 1997'    ", 15, 1, PDS_OK ),
        test_data(            "'%'    ",  3, 1, PDS_OK ),
        test_data(    "'ANEW-LINE\r\n'",  0, 0, PDS_INVALID_VALUE ), 
        test_data("'' /*empty string*/",  0, 0, PDS_INVALID_VALUE ), 
    end_test_data()

	int i;
    test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
		int status = PDS_OK;
        
		int ret = PDS_parse_symbol(first, last, &end, &status);
		
		check(test_expected(i) == ret);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
