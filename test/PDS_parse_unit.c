#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
int main()
{
    begin_test_data(int)
        test_data("<W.m**-2.sr**-1>", 16, PDS_UNIT_W_m2_sr, PDS_OK ),
        test_data(    "<J/(m**2)/s>", 12, PDS_UNIT_J_m2_s,  PDS_OK ),
        test_data(     "<arcsecond>", 11, PDS_UNIT_arcsec,  PDS_OK ),
        test_data(  "<pixel/degree>", 14, PDS_UNIT_px_deg,  PDS_OK ),
        test_data(    "<pix/degree>", 12, PDS_UNIT_px_deg,  PDS_OK ),
        test_data(              "<K",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE ),
        test_data(            "foo>",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE ),
        test_data(            "foo>",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE ),
        test_data(         "m**3/kg",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE ),
        test_data(                 0,  0,                0, 0 ),
    end_test_data()

    int i;
    test_foreach(i)
	{
        const char *first = test_str(i); 
        const char *last  = first + strlen(first) - 1; 
        const char *end   = 0; 
        int status = PDS_OK; 
 
        int value = PDS_parse_unit(first, last, &end, &status);
        
		check(test_expected(i) == value);
        check(test_end(i) == end);
        check(test_status(i) == status);
    }
    return EXIT_SUCCESS;
}
