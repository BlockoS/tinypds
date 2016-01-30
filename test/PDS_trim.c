#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

int main()
{
    begin_test_data(off_t)
        test_data( "\t\n   \r test_\t  fin", 17, 7, PDS_OK ),
        test_data(     "\t  [ string ]\r\n", 12, 3, PDS_OK ),
        test_data(           "another test", 11, 0, PDS_OK ),
        test_data(           "_+=*\t\t\t\n",  3, 0, PDS_OK ),
        test_data(          "   \r\v\t \n ", -1,-1, PDS_INVALID_VALUE ),    
    end_test_data() 

    int i;
    test_foreach(i)
    {
        const char *first = test_str(i);
        const char *last  = first + strlen(first) - 1;
        const char *begin = 0;
        const char *end   = 0;

        PDS_trim(first, last, &begin, &end);
        if(PDS_OK != test_status(i))
        {
            check(0 == begin);
            check(0 == end);
        }
        else
        {
            check(first+test_expected(i) == begin);
            check(test_end(i) == end);
        }
    }
    return EXIT_SUCCESS;
}
