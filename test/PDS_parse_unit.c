#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    

int main()
{
    begin_test_data(int)
        test_data( "<measurement_unit>", 18, 1, PDS_OK ),
        test_data(         "<km/s>\r\n",  6, 1, PDS_OK ),
        test_data(         "<m**3/kg> ",  9, 1, PDS_OK ), 
        test_data(    "<GM*CM*SEC**-2>", 15, 1, PDS_OK ), 
        test_data(          "<km*s**t>",  0, 0, PDS_INVALID_VALUE ), 
        test_data(         "<foor%bar>",  0, 0, PDS_INVALID_VALUE ), 
        test_data(        "<unit_a//5>",  0, 0, PDS_INVALID_VALUE ), 
        test_data(          "<MICRON/>",  0, 0, PDS_INVALID_VALUE ), 
        test_data(               "<m*>",  0, 0, PDS_INVALID_VALUE ), 
        test_data(       "<degree*808>",  0, 0, PDS_INVALID_VALUE ), 
    end_test_data()

	int i;
    test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
		int status = PDS_OK;
        
		int ret = PDS_parse_unit(first, last, &end, &status);
		
		check(test_expected(i) == ret);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
