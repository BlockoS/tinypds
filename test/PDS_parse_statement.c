#include <pds.h>
#include "test.h"
#include "utils.h"

#define PDS_IMPL
#include <pds.h>

typedef struct
{
	const char *name;
	const PDS_scalar *scalar;
	int count;
} expected_t;

typedef struct
{
	int index;
	const expected_t *expected;
} state_t;
 
int dummy_attribute(const char* first, const char *last, const PDS_scalar *scalar, void *user_data)
{
	state_t *state = (state_t*)user_data;
	const expected_t *expected = state->expected;
	if(!PDS_string_compare(first, last, expected->name, expected->name+strlen(expected->name)-1))
	{
		fprintf(stderr, "scalar name mismatch\n");
		return 0;
	}
	if(!compare_scalar(scalar, expected->scalar))
	{
		fprintf(stderr, "scalar value mismatch\n");
		return 0;
	}
	printf("%s = ", expected->name);
	print_scalar(scalar);
	return 1;
}
  
int main()
{
	const char *degrees = "DEGREES";
	const char *symbolic = "dummy";

	const PDS_scalar data[] =
	{
		{	.date_time.type = PDS_DATE_TIME_VALUE,
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
		{	.real.type  = PDS_REAL_VALUE,
			.real.unit.first = degrees,
			.real.unit.last  = degrees + strlen(degrees)-1, 
			.real.value = 12.0
		},
		{	.symbolic.type  = PDS_SYMBOLIC_VALUE,
			.symbolic.first = symbolic,
			.symbolic.last  = symbolic + strlen(symbolic)-1
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.unit.first = 0,
			.integer.unit.last  = 0,
			.integer.value      = 327128
		}
	};
	const expected_t expected[] = 
	{
		{	.name   = "LOCAL_NAMESPACE:DATE_TIME_ATTR",
			.scalar = &data[0],
			.count  = 1
		},
		{	.name   = "ELEVATION_FOV", 
			.scalar = &data[1],
			.count  = 1
		},
		{	.name   = "DUMMY:SYMBOLIC_LITERAL", 
			.scalar = &data[2],
			.count  = 1
		},
		{	.name   = "IMAGE",
			.scalar = &data[3],
			.count  = 1
		}
	};

    begin_test_data(const expected_t*)
// [todo]
#if 0
        test_data(" \r\n        group\t = dummy_group_0\r\n", 20, 1, PDS_OK ),
        test_data(       "end_group=dummy_group_0\t    \r\n", 20, 1, PDS_OK ),
        test_data(                      "OBJECT = IMAGE\r\n", 20, 1, PDS_OK ),
        test_data(                  "END_OBJECT = IMAGE\r\n", 20, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = {'element#0'\t,\r\n'element#1'    }\r\n", 28, 1, PDS_OK ),
		test_data("   DATUM  = \t( 1,2,  3)\r\n\r\n", 28, 1, PDS_OK),
		test_data("   DATA  = (\t( 1,2,  3),\r\n"
                  "              (4,5,6),\r\n   "
                  "              (7,\t8,9) )\r\n", 28, 1, PDS_OK),
#endif
        test_data("LOCAL_NAMESPACE:DATE_TIME_ATTR = 2016-01-3T18:21:49.1299+2:10 \r\n", 64, &expected[0], PDS_OK ),
        test_data("ELEVATION_FOV                  = 12.000 <DEGREES>\r\n", 51, &expected[1], PDS_OK ),
        test_data("DUMMY:SYMBOLIC_LITERAL = 'dummy' \r\n", 35, &expected[2], PDS_OK ),
        test_data("/* comment line #0 */\r\n/* comment line #1 */\r\n^IMAGE = \t 327128    \r\n", 69, &expected[3], PDS_OK ),
    end_test_data()

	PDS_parser parser;
	memset(&parser, 0, sizeof(PDS_parser));
	parser.error = dummy_error;
	parser.attribute = dummy_attribute;
	parser.pointer = dummy_attribute;

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
