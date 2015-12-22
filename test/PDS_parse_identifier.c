#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
typedef struct
{
    const char *str;
    off_t       end;
    int         expected;
    int         status;
} test_value_t;

int main()
{
    test_value_t tv[] = 
    {
        {                    "ROSETTA:",  8, 1, PDS_OK },
        { "SPACECRAFT_CLOCK_STOP_COUNT", 27, 1, PDS_OK },
        {             "INVALID_ENDING_",  0, 0, PDS_INVALID_VALUE },  
        {           "DOUBLE__SEPARATOR",  0, 0, PDS_INVALID_VALUE }, 
        {                 "INVALID_@ID",  0, 0, PDS_INVALID_VALUE },
        {                 "_INVALID_ID",  0, 0, PDS_INVALID_VALUE },
        {                             0,  0, 0, 0 }
    };

	const char *ptr;
    const char *first;
    const char *last;
    const char *end;
    int status; 
    int i;
    
    for(i=0; 0!=tv[i].str; i++)
    {
        first  = tv[i].str;
        last   = tv[i].str + strlen(tv[i].str) - 1;
        status = PDS_OK;
    
        ptr = PDS_parse_identifier(first, last, &end, &status);
        check(tv[i].expected == (ptr!=0));
        check((first+tv[i].end) == end);
        check(tv[i].status == status);
    }
    return EXIT_SUCCESS;
}
