#include "utils.h"
#include <stdio.h>
#include <string.h>

void print_tab(int depth) {
    while(depth--) {
        printf("    ");
    }
}

void print_string(const char *first, const char *last) {                                                                                                             
    for(; first<=last; first++) {                                                                                                         
        fputc(*first, stdout);                                                                                
    }                                                                                                         
}                                                                                                             
                                                                                                              
void print_scalar(const PDS_scalar *scalar) {                                                                                                             
    switch(scalar->type) {                                                                                                         
        case PDS_INTEGER_VALUE:                                                                               
            printf("%ld ", scalar->integer.value);
            if(scalar->integer.unit.first && scalar->integer.unit.last) {
                print_string(scalar->integer.unit.first, scalar->integer.unit.last);
            }                                                          
            break;                                                                                            
        case PDS_REAL_VALUE:                                                                                  
            printf("%f ", scalar->real.value);                                                                
            if(scalar->real.unit.first && scalar->real.unit.last) {
                print_string(scalar->real.unit.first, scalar->real.unit.last);
            }                                                          
            break;                                                                                            
        case PDS_DATE_TIME_VALUE:                                                                             
            printf("%04d-%02d-%02d ", scalar->date_time.year, scalar->date_time.month, scalar->date_time.day);
            printf("%02d:%02d:%02d:%02d ", scalar->date_time.hour, scalar->date_time.minute,
                                           scalar->date_time.second, scalar->date_time.microsecond);
            if(PDS_ZONED_TIME == scalar->date_time.time_type) {
                printf("%02d:%02d ", scalar->date_time.hour_offset, scalar->date_time.minute_offset);
            }
            else if(PDS_UTC_TIME == scalar->date_time.time_type) {
                printf("UTC");
            }
            break;                                                                                            
        case PDS_TEXT_STRING_VALUE:                                                                           
            printf("\"");                                                                                 
            print_string(scalar->text.first, scalar->text.last);                                              
            printf("\" ");                                                                                 
            break;                                                                                            
        case PDS_SYMBOLIC_VALUE:                                                                              
            printf("\'");                                                                                 
            print_string(scalar->text.first, scalar->text.last);                                              
            printf("\' ");                                                                                 
            break;                                                                                            
        case PDS_IDENTIFIER_VALUE:                                                                            
            print_string(scalar->identifier.first, scalar->identifier.last);                                  
            break;                                                                                            
    }                                                                                                         
    printf("\n");
}
