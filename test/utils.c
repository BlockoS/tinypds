#include "utils.h"
#include <stdio.h>

void print_string(const char *first, const char *last)                                                        
{                                                                                                             
    for(; first<=last; first++)                                                                               
    {                                                                                                         
        fputc(*first, stdout);                                                                                
    }                                                                                                         
}                                                                                                             
                                                                                                              
void print_scalar(const PDS_scalar *scalar)                                                                   
{                                                                                                             
    switch(scalar->type)                                                                                      
    {                                                                                                         
        case PDS_INTEGER_VALUE:                                                                               
            printf("%d ", scalar->integer.value);                                                             
            break;                                                                                            
        case PDS_REAL_VALUE:                                                                                  
            printf("%f ", scalar->real.value);                                                                
            break;                                                                                            
        case PDS_DATE_TIME_VALUE:                                                                             
            printf("date: ");
			// [todo]                                                                                 
            break;                                                                                            
        case PDS_TEXT_STRING_VALUE:                                                                           
            printf("text: ");                                                                                 
            print_string(scalar->text.first, scalar->text.last);                                              
            break;                                                                                            
        case PDS_SYMBOLIC_VALUE:                                                                              
            printf("literal: ");                                                                              
            print_string(scalar->text.first, scalar->text.last);                                              
            break;                                                                                            
        case PDS_IDENTIFIER_VALUE:                                                                            
            printf("id: ");                                                                                   
            print_string(scalar->identifier.first, scalar->identifier.last);                                  
            break;                                                                                            
    }                                                                                                         
}
