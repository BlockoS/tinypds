#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
   
// [todo] expected structure
void print_string(const char *first, const char *last)
{
	for(; first<=last; first++)
	{
		fputc(*first, stdout);
	}
}

void print_scalar(const PDS_scalar *scalar)
{
	switch(scalar->type)
	{
		case PDS_INTEGER_VALUE:
			printf("%d ", scalar->integer.value);
			break;    
		case PDS_REAL_VALUE:
			printf("%f ", scalar->real.value);
			break;
		case PDS_DATE_TIME_VALUE:
			printf("date: ");
			break;
		case PDS_TEXT_STRING_VALUE:
			printf("text: ");
			print_string(scalar->text.first, scalar->text.last);
			break;
		case PDS_SYMBOLIC_VALUE:
			printf("literal: ");
			print_string(scalar->text.first, scalar->text.last);
			break;
		case PDS_IDENTIFIER_VALUE:
			printf("id: ");
			print_string(scalar->identifier.first, scalar->identifier.last);
			break;
	}
}
// [todo] user_data => add depth for tab print
int sequence_begin(const char* first, const char *last, void *user_data)
{
	printf("sequence : ");
	print_string(first, last);
	fputc('\n', stdout);
	return 1;
}

int sequence_element(const PDS_scalar *scalar, void *user_data)
{
	print_scalar(scalar);
	return 1;
}

int sequence_end(void *user_data)
{
	printf("\nsequence end\n");
	return 1;
}

int main()
{
    begin_test_data(int32_t)
#if 0
        test_data(" \r\n        group\t = dummy_group_0\r\n", 20, 1, PDS_OK ),
        test_data(       "end_group=dummy_group_0\t    \r\n", 20, 1, PDS_OK ),
        test_data(                      "OBJECT = IMAGE\r\n", 20, 1, PDS_OK ),
        test_data(                  "END_OBJECT = IMAGE\r\n", 20, 1, PDS_OK ),
        test_data("^image /* comment */ = \t 327128    \r\n", 25, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 'dummy' \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 1234 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 2#-1010# \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = -32 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = +.82e4 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = .2 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 100.4e1 \r\n", 28, 1, PDS_OK ),
        test_data("ELEVATION_FOV                 = 12.000 <DEGREES>\r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = 2016-01-3T18:21:49.1299+2:10 \r\n", 28, 1, PDS_OK ),
        test_data("LOCAL_NAMESPACE:ATTRIBUTE = {'element#0'\t,\r\n'element#1'    }\r\n", 28, 1, PDS_OK ),
#endif
		test_data("   DATA  = (\t( 1,2,  3),\r\n"
                  "              (4,5,6),\r\n   "
                  "              (7,\t8,9) )\r\n", 28, 1, PDS_OK),
    end_test_data()

	PDS_parser parser;
	parser.error = dummy_error;
	parser.sequence_begin   = sequence_begin;
	parser.sequence_element = sequence_element;
	parser.sequence_end     = sequence_end;

	int i;
    test_foreach(i)
	{
		parser.line    = 1;
        parser.first   = test_str(i);
        parser.last    = parser.first + strlen(parser.first) - 1;
        parser.current = parser.first;
		parser.status  = PDS_OK;
        
		int ret = PDS_parse_statement(&parser);
        check(test_status(i) == parser.status);
        if(ret)
		{
//			check(test_end(i) == parser.current);
		}
	}
    return EXIT_SUCCESS;
}
