#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pds.h>
#define PDS_IMPL
#include <pds.h>
    
typedef struct
{
    const char *str;
    int         base;
    off_t       end;
    int32_t     expected;
    int         status;
} test_value_t;

#define check(e) if(!(e)) { fprintf(stderr, "%s+%d: test failed \"%s\"\n", __FILE__, __LINE__, #e); return EXIT_FAILURE; }

int main()
{
    test_value_t tv[] = 
    {
        {   " \t-714@_", 10,  6,      -714, PDS_OK },
        {        "bEeF", 16,  4,    0xbeef, PDS_OK },
        {    "10001111",  2,  8,      0x8f, PDS_OK },
        {    "+1339/6 ",  8,  4,        91, PDS_OK },
        {       "dummy",  7,  0,         0, PDS_INVALID_VALUE },
        {  "2187483650", 10, 10, INT32_MAX, PDS_INVALID_RANGE },
        { "-3000000000", 10, 11, INT32_MIN, PDS_INVALID_RANGE }
    };
    int32_t value;
    const char *first;
    const char *last;
    const char *end;
    int status; 
    int i;
    for(i=0; i<7; i++)
    {
        first  = tv[i].str;
        last   = tv[i].str + strlen(tv[i].str) - 1;
        status = PDS_OK;
    
        value  = parse_int(first, last, &end, tv[i].base, &status);
        check(tv[i].expected == value);
        check((first+tv[i].end) == end);
        check(tv[i].status == status);
    }
    return EXIT_SUCCESS;
}
