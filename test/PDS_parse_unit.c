#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define check(e) if(!(e)) { fprintf(stderr, "%s+%d: test failed \"%s\"\n", __FILE__, __LINE__, #e); return EXIT_FAILURE; }

int main()
{
    test_value_t tv[] = 
    {
        {"<W.m**-2.sr**-1>", 16, PDS_UNIT_W_m2_sr, PDS_OK },
        {    "<J/(m**2)/s>", 12, PDS_UNIT_J_m2_s,  PDS_OK },
        {     "<arcsecond>", 11, PDS_UNIT_arcsec,  PDS_OK },
        {  "<pixel/degree>", 14, PDS_UNIT_px_deg,  PDS_OK },
        {    "<pix/degree>", 12, PDS_UNIT_px_deg,  PDS_OK },
        {              "<K",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE },
        {            "foo>",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE },
        {            "foo>",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE },
        {         "m**3/kg",  0, PDS_UNIT_UNKNOWN, PDS_INVALID_VALUE },
    };

    double value;
    const char *first;
    const char *last;
    const char *end;
    int status; 
    int i;
    for(i=0; i<9; i++)
    {
        first  = tv[i].str;
        last   = tv[i].str + strlen(tv[i].str) - 1;
        status = PDS_OK;
    
        value  = PDS_parse_unit(first, last, &end, &status);
        check(tv[i].expected == value);
        check((first+tv[i].end) == end);
        check(tv[i].status == status);
    }
    return EXIT_SUCCESS;
}
