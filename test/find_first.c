#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

int main()
{
	const struct
	{
		const char *str;
		off_t       expected;
		char        c;	
	} tv [] = 
	{
		{                      "  b1234\t`#_Q3#[S}",  9, '#' },
		{ "namespace:VALUE_1    =  'literal'  \r\n", 21, '=' },
        {                          "I hate clowns!",  0, 'I' }, 
		{           "Klatu verata ni-mumble mumble", -1, '*' },
		{ 0, 0, '\0' },
	};	


    int i;
	test_foreach(i)
	{
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        off_t index       = test_expected(i);

        const char *found = find_first(first, last, tv[i].c);
		if(index < 0)
		{
			check(0 == found);
		}
		else
		{
			check(found == (first+index));
		}
    }
    return EXIT_SUCCESS;
}
