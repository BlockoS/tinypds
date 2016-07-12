#include <tinypds.h>

#include "test.h"
#include "utils.h"

#define TINY_PDS_IMPL
#include <tinypds.h>

int main()
{
    const char *unit_first = "Km";
    const char *unit_last  = unit_first + strlen(unit_first) - 1;

    const PDS_scalar sequence_1D[] = 
    {
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 51,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 8,
            .integer.unit.first = unit_first,
            .integer.unit.last  = unit_last
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 117,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 9.2,
            .real.unit.first = unit_first,
            .real.unit.last  = unit_last
        },
    };

    const PDS_scalar sequence_2D[] =
    {
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 4,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {
            .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 2048,
            .integer.unit.first = 0,
            .integer.unit.last  = 0 
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 0.6,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 14,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {
            .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 1024,
            .integer.unit.first = 0,
            .integer.unit.last  = 0 
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 1.2,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 24,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {
            .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 256,
            .integer.unit.first = 0,
            .integer.unit.last  = 0 
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 2.4,
            .real.unit.first = 0,
            .real.unit.last  = 0
        }
    };

    const PDS_scalar failure[] = 
    {
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 1,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 2,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 3,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 4,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 5,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 6,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        }
    };

    const expected_t sequences[] = 
    {
        {   .depth  = 1,
            .count  = 4,
            .name   = "dummy",
            .scalar = sequence_1D
        },
        {
            .depth  = 2,
            .count  = 9,
            .name   = "matrix",
            .scalar = sequence_2D
        },
        {
            .depth  = 2,
            .count  = 6,
            .name   = "failure",
            .scalar = failure
        }
    };
    
    begin_test_data(const expected_t*)
        test_data("   dummy  = \t( 51, 8 <Km>, \t117, 9.2 <Km>)\r\n", 44, &sequences[0], PDS_OK),
        test_data("matrix = (\r\n\t(4,2048,.6),\r\n\t(14,1024,1.2),\r\n\t(24,256,2.4) )\r\n", 61, &sequences[1], PDS_OK),
        test_data("failure = (\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = ()\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = (())\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = ((1,2)\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = (1,,3),\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = ((1,2,3,),(4,5,6))\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = ((1,2,3),(4,5,6)\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = (1,2,3,(4,5,6))\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
        test_data("failure = ((1,2),3,4,5,6)\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
    end_test_data()

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);
    PDS_set_sequence_callbacks(&parser.callbacks, sequence_begin_callback, sequence_end_callback);
    PDS_set_scalar_callback(&parser.callbacks, sequence_element_callback);
  
    size_t i;
    test_foreach(i)
    {
        state_t state = { .depth = 0, .index = 0, .expected = test_expected(i) };

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
