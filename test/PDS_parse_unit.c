#include <tinypds.h>
#include "test.h"

#define TINY_PDS_IMPL
#include <tinypds.h>
    

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

    PDS_parser parser;
    parser.error = dummy_error;

    size_t i;
    test_foreach(i)
    {
        parser.line_num = 1;
        parser.first    = test_str(i);
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;

        parser.scalar.type = PDS_INTEGER_VALUE;
        parser.scalar.integer.unit.first = 0;
        parser.scalar.integer.unit.last  = 0;

        int ret = PDS_parse_unit(&parser);
        
        check(test_expected(i) == ret);
        check(test_end(i) == parser.current);
        check(test_status(i) == parser.status);
    }
    return EXIT_SUCCESS;
}
