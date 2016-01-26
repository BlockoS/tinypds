#include <pds.h>
#include "test.h"
#include "utils.h"

#define PDS_IMPL
#include <pds.h>

typedef struct
{
	int count;
	const char *name;
	const PDS_scalar *data;
} expected_t;

typedef struct
{
	int index;
	const expected_t *expected;
} state_t;
 
int set_begin(const char* first, const char *last, void *user_data)
{
	int ret;
	state_t *state = (state_t*)user_data;
	const expected_t *expected = state->expected;
	ret = PDS_string_compare(expected->name, expected->name+strlen(expected->name)-1, first, last);
	return ret;
}

int set_element(const PDS_scalar *scalar, void *user_data)
{
	int ret;
	state_t *state = (state_t*)user_data;
	const expected_t *expected = state->expected;
	if(state->index >= expected->count)
	{
		return 0;
	}
	ret = compare_scalar(scalar, &(expected->data[state->index])); 
	state->index++;
	return ret;
}

int set_end(void *user_data)
{
	return 1;
}

int main()
{
	const PDS_scalar set[] = 
	{
		{	.integer.type  = PDS_REAL_VALUE,
			.integer.value = 123.8,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_REAL_VALUE,
			.integer.value = 90.0,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_REAL_VALUE,
			.integer.value = 0.125,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 0.02,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
	};

	const PDS_scalar failure[] = 
	{
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


	const expected_t sequences[] = 
	{
		{   .count = 0,
            .name  = "empty",
            .data  = set
        },
		{	.count = 4,
			.name  = "dummy",
			.data  = set
		},
		{   .count = 6,
			.name  = "failure",
			.data  = failure
		}
	};
    
	begin_test_data(const expected_t*)
		test_data("empty = { 123.8, 90.0, 0.125, 0.02 }\r\n", 13, &sequences[0], PDS_OK),
		test_data("failure = {\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
		test_data("failure = {}\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
		test_data("failure = { \t, }\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
		test_data("failure = {{}}\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
		test_data("failure = {1,,3}\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
		test_data("failure = {{1,2,3,4,5,6}}\r\n", 0, &sequences[2], PDS_INVALID_VALUE),
	end_test_data()

	PDS_parser parser;
	parser.error = dummy_error;
	parser.set_begin   = set_begin;
	parser.set_element = set_element;
	parser.set_end     = set_end;

	int i;
    test_foreach(i)
	{
		state_t state = { .index = 0, .expected = test_expected(i) };

		parser.line      = 1;
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
