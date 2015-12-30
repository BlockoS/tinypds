#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
int main()
{
    begin_test_data(int)
        test_data(                    "ROSETTA:",  8, 1, PDS_OK ),
        test_data( "SPACECRAFT_CLOCK_STOP_COUNT", 27, 1, PDS_OK ),
        test_data(             "INVALID_ENDING_",  0, 0, PDS_INVALID_VALUE ),  
        test_data(           "DOUBLE__SEPARATOR",  0, 0, PDS_INVALID_VALUE ), 
        test_data(                 "INVALID_@ID",  0, 0, PDS_INVALID_VALUE ),
        test_data(                 "_INVALID_ID",  0, 0, PDS_INVALID_VALUE ),
    end_test_data()

    int i;
    test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
		const char *end   = 0;
        int status = PDS_OK;
    
        const char *ptr = PDS_parse_identifier(first, last, &end, &status);
        
		check(test_expected(i) == (ptr!=0));
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
