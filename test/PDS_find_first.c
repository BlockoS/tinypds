#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

int main()
{
    begin_test_data(char)
        test_data(                      "  b1234\t`#_Q3#[S}",  9, '#', PDS_OK ),
        test_data( "namespace:VALUE_1    =  'literal'  \r\n", 21, '=', PDS_OK ),
        test_data(                          "I hate clowns!",  0, 'I', PDS_OK ), 
        test_data(           "Klatu verata ni-mumble mumble", -1, '*', PDS_INVALID_VALUE ),
    end_test_data()


    int i;
    test_foreach(i)
    {
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;

        const char *found = PDS_find_first(first, last, test_expected(i));
        if(PDS_OK != test_status(i))
        {
            check(0 == found);
        }
        else
        {
            check(test_end(i) == found);
        }
    }
    return EXIT_SUCCESS;
}
