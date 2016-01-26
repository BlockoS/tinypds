#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

typedef struct
{
	int depth;
	int count;
	const char *name;
	const PDS_scalar *data;
} expected_t;

typedef struct
{
	int depth;
	int index;
	const expected_t *expected;
} state_t;
 
int sequence_begin(const char* first, const char *last, void *user_data)
{
	int ret;
	state_t *state = (state_t*)user_data;
	const expected_t *expected = state->expected;
	
	ret = PDS_string_compare(expected->name, expected->name+strlen(expected->name)-1, first, last);
	if(ret)
	{
		state->depth++;
		if(state->depth > expected->depth)
		{
			ret = 0;
		}
	}
	return ret;
}

int sequence_element(const PDS_scalar *scalar, void *user_data)
{
	int ret;
	state_t *state = (state_t*)user_data;
	const expected_t *expected = state->expected;
	const PDS_scalar *expected_scalar = 0;	
	if(state->index >= expected->count)
	{
		return 0;
	}
	expected_scalar = &(expected->data[state->index]);
	if(scalar->type != expected_scalar->type)
	{
		return 0;
	}
	ret = 0;
	switch(scalar->type)
	{
		case PDS_INTEGER_VALUE:
			if(scalar->integer.value == expected_scalar->integer.value)
			{
				if(    (scalar->integer.unit.first && expected_scalar->integer.unit.first)
				    && (scalar->integer.unit.last  && expected_scalar->integer.unit.last ) )
				{
					ret = PDS_string_compare(scalar->integer.unit.first, scalar->integer.unit.last, 
					                         expected_scalar->integer.unit.first, expected_scalar->integer.unit.last);
				}
				else if(    (0 == scalar->integer.unit.first) && (0 == expected_scalar->integer.unit.first)
                         && (0 == expected_scalar->integer.unit.first) && (0 == expected_scalar->integer.unit.last) )
				{
					ret = 1;
				} 
			}
			break;
		case PDS_REAL_VALUE:
			if(scalar->real.value == expected_scalar->real.value)
			{
				if(    (scalar->real.unit.first && expected_scalar->real.unit.first)
				    && (scalar->real.unit.last  && expected_scalar->real.unit.last ) )
				{
					ret = PDS_string_compare(scalar->real.unit.first, scalar->real.unit.last, 
					                         expected_scalar->real.unit.first, expected_scalar->real.unit.last);
				}
				else if(    (0 == scalar->real.unit.first) && (0 == expected_scalar->real.unit.first)
                         && (0 == expected_scalar->real.unit.first) && (0 == expected_scalar->real.unit.last) )
				{
					ret = 1;
				} 
			}
			break;
		case PDS_DATE_TIME_VALUE:
			if(   (scalar->date_time.time_type == expected_scalar->date_time.time_type)
			   && (scalar->date_time.year == expected_scalar->date_time.year) 
			   && (scalar->date_time.day == expected_scalar->date_time.day) 
			   && (scalar->date_time.month == expected_scalar->date_time.month) 
			   && (scalar->date_time.hour == expected_scalar->date_time.hour) 
			   && (scalar->date_time.minute == expected_scalar->date_time.minute) 
			   && (scalar->date_time.second == expected_scalar->date_time.second) 
			   && (scalar->date_time.microsecond == expected_scalar->date_time.microsecond) 
			   && (scalar->date_time.hour_offset == expected_scalar->date_time.hour_offset) 
			   && (scalar->date_time.minute_offset == expected_scalar->date_time.minute_offset) )
			{
				ret = 1;
			}
			break;
		case PDS_SYMBOLIC_VALUE:
			ret = PDS_string_compare(scalar->symbolic.first, scalar->symbolic.last, expected_scalar->symbolic.first, expected_scalar->symbolic.last);
			break;
		case PDS_TEXT_STRING_VALUE:
			ret = PDS_string_compare(scalar->text.first, scalar->text.last, expected_scalar->text.first, expected_scalar->text.last);
			break;
		case PDS_IDENTIFIER_VALUE:
			ret = PDS_string_compare(scalar->identifier.first, scalar->identifier.last, expected_scalar->identifier.first, expected_scalar->identifier.last);
			break;
	}
	state->index++;
	return ret;
}

int sequence_end(void *user_data)
{
	state_t *state = (state_t*)user_data;
	state->depth--;
	if(state->depth < 0)
	{
		return 0;
	}
	return 1;
}

int main()
{
	const char *unit_first = "Km";
	const char *unit_last  = unit_first + strlen(unit_first) - 1;

	const PDS_scalar sequence_1D[] = 
	{
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 51,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 8,
			.integer.unit.first = unit_first,
			.integer.unit.last  = unit_last
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 117,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 9.2,
			.real.unit.first = unit_first,
			.real.unit.last  = unit_last
		},
	};

	const PDS_scalar sequence_2D[] =
	{
		{	.integer.type  = PDS_INTEGER_VALUE,
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
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 0.6,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
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
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 1.2,
			.real.unit.first = 0,
			.real.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
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
		{	.real.type  = PDS_REAL_VALUE,
			.real.value = 2.4,
			.real.unit.first = 0,
			.real.unit.last  = 0
		}
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
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 5,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		},
		{	.integer.type  = PDS_INTEGER_VALUE,
			.integer.value = 6,
			.integer.unit.first = 0,
			.integer.unit.last  = 0
		}
	};


	const expected_t sequences[] = 
	{
		{	.depth = 1,
			.count = 4,
			.name  = "dummy",
			.data  = sequence_1D
		},
		{
			.depth = 2,
			.count = 9,
			.name  = "matrix",
			.data  = sequence_2D
		},
		{
			.depth = 2,
			.count = 6,
			.name  = "failure",
			.data  = failure
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
	end_test_data()

	PDS_parser parser;
	parser.error = dummy_error;
	parser.sequence_begin   = sequence_begin;
	parser.sequence_element = sequence_element;
	parser.sequence_end     = sequence_end;

	int i;
    test_foreach(i)
	{
		state_t state = { .depth = 0, .index = 0, .expected = test_expected(i) };

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
