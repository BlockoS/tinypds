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

    PDS_parser parser;
    parser.error = dummy_error;

    int i;
    test_foreach(i)
    {
        parser.line_num = 1;
        parser.first    = test_str(i); 
        parser.last     = parser.first + strlen(parser.first) - 1; 
        parser.current  = parser.first; 
        parser.status   = PDS_OK; 
    
        int ret = PDS_parse_real(&parser);
        check(test_status(i) == parser.status);
        check(test_end(i) == parser.current);
        if(ret)
        {
            check(PDS_REAL_VALUE == parser.scalar.type);
            check(test_expected(i) == parser.scalar.real.value);
        }
    }
    return EXIT_SUCCESS;
}
