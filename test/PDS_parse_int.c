#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
typedef struct
{
    const char *str;
    off_t       end;
    int32_t     expected;
    int         status;
} test_value_t;

int main()
{
    test_value_t tv[] = 
    {
        {     "   511-", 6,       511, PDS_OK },
        {  "2#+0101#@ ", 8,         5, PDS_OK },
        {      "8#foo#", 2,         0, PDS_INVALID_VALUE },  
        {     "10#-9a#", 5, INT32_MAX, PDS_INVALID_VALUE },  
        {      "16#ffc", 6, INT32_MAX, PDS_INVALID_VALUE },  
        {         "4##", 2,         0, PDS_INVALID_VALUE },
        {             0, 0,         0, 0 }
    };

    int32_t value;
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
    
        value  = PDS_parse_int(first, last, &end, &status);
        check(tv[i].expected == value);
        check((first+tv[i].end) == end);
        check(tv[i].status == status);
    }
    return EXIT_SUCCESS;
}
