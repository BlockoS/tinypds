#include <pds.h>
#include "test.h"
#include "utils.h"

#define PDS_IMPL
#include <pds.h>
 
int main()
{
	const PDS_scalar data[] = 
	{
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 123.8,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 90.0,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 0.125,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 0.02,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 1,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 2,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 3,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 4,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		}
	};


	const expected_t set[] = 
	{
		{   .count  = 4,
            .name   = "dummy",
            .scalar = &data[0]
        },
		{   .count  = 6,
			.name   = "failure",
			.scalar = &data[4]
		}
	};
    
	begin_test_data(const expected_t*)
		test_data("dummy = { 123.8, 90.0, 0.125, 0.02 }\r\n", 38, &set[0], PDS_OK),
		test_data("failure = {\r\n", 0, &set[1], PDS_INVALID_VALUE),
		test_data("failure = {}\r\n", 0, &set[1], PDS_INVALID_VALUE),
		test_data("failure = { \t, }\r\n", 0, &set[1], PDS_INVALID_VALUE),
		test_data("failure = {{}}\r\n", 0, &set[1], PDS_INVALID_VALUE),
		test_data("failure = {1,,3}\r\n", 0, &set[1], PDS_INVALID_VALUE),
		test_data("failure = {{1,2,3,4,5,6}}\r\n", 0, &set[1], PDS_INVALID_VALUE),
	end_test_data()

	PDS_parser parser;
	parser.error = dummy_error;
	parser.set.begin   = set_begin_callback;
	parser.set.element = set_element_callback;
	parser.set.end     = set_end_callback;

	int i;
    test_foreach(i)
	{
		state_t state = { .index = 0, .expected = test_expected(i) };

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
