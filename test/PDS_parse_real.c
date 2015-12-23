#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
int main()
{
	begin_test_data(double)
        test_data( "   511-", 6,     511.0, PDS_OK ),
        test_data("-71.600 ", 7,     -71.6, PDS_OK ),
        test_data( "+0.0004", 7,    0.0004, PDS_OK ),  
        test_data(   "1.e3@", 4,    1000.0, PDS_OK ),  
        test_data("211.32e4", 8, 2113200.0, PDS_OK ),  
        test_data(  "-1.e-5", 6,    -1.e-5, PDS_OK ),  
        test_data(  "-.2e-1", 6,     -0.02, PDS_OK ),  
        test_data(  "-.01e3", 6,     -10.0, PDS_OK ),  
        test_data(   "af.01", 0,       0.0, PDS_OK ),  
        test_data(     "-.e", 0,       0.0, PDS_INVALID_VALUE ),  
        test_data( "8.-0710", 0,       0.0, PDS_INVALID_VALUE ),
    end_test_data()

    int i;
    test_foreach(i)
	{
        const char *first = test_str(i); 
        const char *last  = first + strlen(first) - 1; 
        const char *end   = 0; 
        int status = PDS_OK; 
    
        double value  = PDS_parse_real(first, last, &end, &status);
        
		check(test_expected(i) == value);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
