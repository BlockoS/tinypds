#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
 
int main()
{
    PDS_parser parser;
    begin_test_data(int)
        test_data(         "\"\" /*empty string*/",  2, 1, PDS_OK ),
        test_data(            "\"simple string\" ", 15, 1, PDS_OK ),
        test_data("\"multi\r\nline\r\n\tstring\" ", 22, 1, PDS_OK ),
        test_data(   "\"Some\\\\escaped\\nchars\"", 22, 1, PDS_OK ),
        test_data(  "\" I\nvalid \\espaced char\"",  0, 0, PDS_INVALID_VALUE ), 
        test_data(         "\"\n\n\nSame here\\\"",  0, 0, PDS_INVALID_VALUE ), 
    end_test_data()

    memset(&parser, 0, sizeof(PDS_parser));
    parser.user_data = 0;
    parser.error = dummy_error;

    int i;
    test_foreach(i)
    {
        parser.status   = PDS_OK;
        parser.first    = test_str(i);
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.line_num = 1;
 
        int ret = PDS_parse_string(&parser);
        
        check(test_expected(i) == ret);
        check(test_end(i) == parser.current);
        check(test_status(i) == parser.status);
        if(ret)
        {
            check(PDS_TEXT_STRING_VALUE == parser.scalar.type);
            check((parser.first+1) == parser.scalar.text.first);
            check((parser.current-2) == parser.scalar.text.last);
        }
    }
    return EXIT_SUCCESS;
}
