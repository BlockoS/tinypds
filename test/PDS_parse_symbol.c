#include <tinypds.h>
#include "test.h"

#define TINY_PDS_IMPL
#include <tinypds.h>
    

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

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    size_t i;
    test_foreach(i)
    {
        parser.scalar.type = PDS_UNKNOWN_VALUE;

        parser.line_num = 1;
        parser.first    = test_str(i);
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;
        
        int ret = PDS_parse_symbol(&parser);
        
        check(test_expected(i) == ret);
        check(test_end(i) == parser.current);
        check(test_status(i) == parser.status);
        if(ret)
        {
            check(PDS_SYMBOLIC_VALUE == parser.scalar.type);
            check((test_str(i)+1) == parser.scalar.symbolic.first);
            check((test_end(i)-2) == parser.scalar.symbolic.last);
        }
    }
    return EXIT_SUCCESS;
}
