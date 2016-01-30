#include <pds.h>
#include "test.h"
#include "utils.h"

#define PDS_IMPL
#include <pds.h>

int main()
{
    const char *degrees = "DEGREES";
    const char *symbolic = "dummy";
    const char *group = "dummy_group_0";
    const char *object = "IMAGE";

    const PDS_scalar data[] =
    {
        {   .date_time.type = PDS_DATE_TIME_VALUE,
            .date_time.year  = 2016,
            .date_time.day   = 3,
            .date_time.month = 1,
            .date_time.hour   = 18,
            .date_time.minute = 21,
            .date_time.second = 49,
            .date_time.microsecond = 129900,
            .date_time.hour_offset   =  2,
            .date_time.minute_offset = 10,
            .date_time.time_type = PDS_ZONED_TIME
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.unit.first = degrees,
            .real.unit.last  = degrees + strlen(degrees)-1, 
            .real.value = 12.0
        },
        {   .symbolic.type  = PDS_SYMBOLIC_VALUE,
            .symbolic.first = symbolic,
            .symbolic.last  = symbolic + strlen(symbolic)-1
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.unit.first = 0,
            .integer.unit.last  = 0,
            .integer.value      = 327128
        },
        {
            .identifier.type  = PDS_IDENTIFIER_VALUE,
            .identifier.first = group,
            .identifier.last  = group + strlen(group)-1
        },
        {
            .identifier.type  = PDS_IDENTIFIER_VALUE,
            .identifier.first = object,
            .identifier.last  = object + strlen(object)-1
        }
    };
    const expected_t expected[] = 
    {
        {   .name   = "LOCAL_NAMESPACE:DATE_TIME_ATTR",
            .scalar = &data[0],
            .count  = 1
        },
        {   .name   = "ELEVATION_FOV", 
            .scalar = &data[1],
            .count  = 1
        },
        {   .name   = "DUMMY:SYMBOLIC_LITERAL", 
            .scalar = &data[2],
            .count  = 1
        },
        {   .name   = "IMAGE",
            .scalar = &data[3],
            .count  = 1
        },
        {
            .name   = "group",
            .scalar = &data[4],
            .count  = 1
        },
        {
            .name   = "object",
            .scalar = &data[5],
            .count  = 1
        }
    };

    begin_test_data(const expected_t*)
        test_data("LOCAL_NAMESPACE:DATE_TIME_ATTR = 2016-01-3T18:21:49.1299+2:10 \r\n", 64, &expected[0], PDS_OK ),
        test_data("ELEVATION_FOV                  = 12.000 <DEGREES>\r\n", 51, &expected[1], PDS_OK ),
        test_data("DUMMY:SYMBOLIC_LITERAL = 'dummy' \r\n", 35, &expected[2], PDS_OK ),
        test_data("/* comment line #0 */\r\n/* comment line #1 */\r\n^IMAGE = \t 327128    \r\n", 69, &expected[3], PDS_OK ),
        test_data(" \r\n        group\t = dummy_group_0\r\n", 35, &expected[4], PDS_OK ),
        test_data("end_group=dummy_group_0\t    \r\n", 30, &expected[4], PDS_OK ),
        test_data("OBJECT = IMAGE\r\n", 16, &expected[5], PDS_OK ),
        test_data("END_OBJECT = IMAGE\r\n", 20, &expected[5], PDS_OK ),
    end_test_data()

    PDS_parser parser;
    state_t state;
    
    memset(&state, 0, sizeof(state_t));

    memset(&parser, 0, sizeof(PDS_parser));
    parser.error = dummy_error;
    parser.attribute = attribute_callback;
    parser.pointer = attribute_callback;
    parser.group.begin = group_begin_callback;
    parser.group.end   = group_end_callback;
    parser.object.begin = object_begin_callback;
    parser.object.end   = object_end_callback;

    int i;
    test_foreach(i)
    {
        state.index = 0;
        state.expected = test_expected(i);

        parser.line_num  = 1;
        parser.first     = test_str(i);
        parser.last      = parser.first + strlen(parser.first) - 1;
        parser.current   = parser.first;
        parser.status    = PDS_OK;
        parser.user_data = &state;

        int ret = PDS_parse_statement(&parser);
        check(test_status(i) == parser.status);
        if(ret)
        {
            check(test_end(i) == parser.current);
        }
    }
    return EXIT_SUCCESS;
}
