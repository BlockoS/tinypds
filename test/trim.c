#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

int main()
{
	const struct
	{
		const char *str;
		struct
		{
			off_t begin;
			off_t end;
		} expected;
	} tv [] = 
	{
		{ "\t\n   \r test_\t  fin", { 7, 17 } },
		{     "\t  [ string ]\r\n", { 3, 12 } },
		{           "another test", { 0, 11 } },
		{           "_+=*\t\t\t\n", { 0,  3 } },
		{          "   \r\v\t \n ", {-1, -1 } },
		{ 0, { 0, 0 } },
	};	


    int i;
	test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
		const char *begin = 0;
		const char *end   = 0;

        trim(first, last, &begin, &end);
		if(test_expected(i).begin < 0)
		{
			check(0 == begin);
			check(0 == end);
		}
		else
		{
			check((first+test_expected(i).begin) == begin);
			check((first+test_expected(i).end) == end);
		}
	}
    return EXIT_SUCCESS;
}
