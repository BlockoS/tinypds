#include <math.h>
#include <tinypds.h>
#include "test.h"

#define TINY_PDS_IMPL
#include <tinypds.h>
    
int main()
{
    begin_test_data(double)
        test_data( "0.6170492162", 12,  0.6170492162, PDS_OK),
        test_data("-0.3187327298", 13, -0.3187327298, PDS_OK),
        test_data("-0.5045034227", 13, -0.5045034227, PDS_OK),
        test_data("-0.5129678433", 13, -0.5129678433, PDS_OK),
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
    end_test_data()

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    size_t i;
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
            check(fabs(test_expected(i) - parser.scalar.real.value) < 1.e-6);
        }
    }
    return EXIT_SUCCESS;
}
