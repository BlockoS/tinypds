#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
typedef struct
{
    const char *str;
    off_t       end;
    double      expected;
    int         status;
} test_value_t;

int main()
{
    test_value_t tv[] = 
    {
        {     "   511-", 6,     511.0, PDS_OK },
        {    "-71.600 ", 7,     -71.6, PDS_OK },
        {     "+0.0004", 7,    0.0004, PDS_OK },  
        {       "1.e3@", 4,    1000.0, PDS_OK },  
        {    "211.32e4", 8, 2113200.0, PDS_OK },  
        {      "-1.e-5", 6,    -1.e-5, PDS_OK },  
        {      "-.2e-1", 6,     -0.02, PDS_OK },  
        {      "-.01e3", 6,     -10.0, PDS_OK },  
        {       "af.01", 0,       0.0, PDS_OK },  
        {         "-.e", 0,       0.0, PDS_INVALID_VALUE },  
        {     "8.-0710", 0,       0.0, PDS_INVALID_VALUE },
        {             0, 0,       0.0, 0 }
    };

    double value;
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
    
        value  = PDS_parse_real(first, last, &end, &status);
        check(tv[i].expected == value);
        check((first+tv[i].end) == end);
        check(tv[i].status == status);
    }
    return EXIT_SUCCESS;
}
