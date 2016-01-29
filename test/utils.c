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
			if(scalar->integer.unit.first && scalar->integer.unit.last)
			{
				print_string(scalar->integer.unit.first, scalar->integer.unit.last);
			}                                                          
            break;                                                                                            
        case PDS_REAL_VALUE:                                                                                  
            printf("%f ", scalar->real.value);                                                                
			if(scalar->real.unit.first && scalar->real.unit.last)
			{
				print_string(scalar->real.unit.first, scalar->real.unit.last);
			}                                                          
            break;                                                                                            
        case PDS_DATE_TIME_VALUE:                                                                             
            printf("%04d-%02d-%02d ", scalar->date_time.year, scalar->date_time.month, scalar->date_time.day);
            printf("%02d:%02d:%02d:%02d ", scalar->date_time.hour, scalar->date_time.minute,
			                               scalar->date_time.second, scalar->date_time.microsecond);
			if(PDS_ZONED_TIME == scalar->date_time.time_type)
			{
				printf("%02d:%02d ", scalar->date_time.hour_offset, scalar->date_time.minute_offset);
			}
			else if(PDS_UTC_TIME == scalar->date_time.time_type)
			{
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

int compare_scalar(const PDS_scalar *s0, const PDS_scalar *s1)
{
	if(s0->type != s1->type)
	{
		return 0;
	}
	switch(s0->type)
	{
		case PDS_INTEGER_VALUE:
			if(s0->integer.value == s1->integer.value)
			{
				if(    (s0->integer.unit.first && s1->integer.unit.first)
				    && (s0->integer.unit.last  && s1->integer.unit.last ) )
				{
					return PDS_string_compare(s0->integer.unit.first, s0->integer.unit.last, 
					                          s1->integer.unit.first, s1->integer.unit.last);
				}
				else if(    (0 == s0->integer.unit.first) && (0 == s1->integer.unit.first)
                         && (0 == s1->integer.unit.first) && (0 == s1->integer.unit.last) )
				{
					return 1;
				} 
			}
			return 0;
		case PDS_REAL_VALUE:
			if(s0->real.value == s1->real.value)
			{
				if(    (s0->real.unit.first && s1->real.unit.first)
				    && (s0->real.unit.last  && s1->real.unit.last ) )
				{
					return PDS_string_compare(s0->real.unit.first, s0->real.unit.last, 
					                          s1->real.unit.first, s1->real.unit.last);
				}
				else if(    (0 == s0->real.unit.first) && (0 == s1->real.unit.first)
                         && (0 == s1->real.unit.first) && (0 == s1->real.unit.last) )
				{
					return 1;
				} 
			}
			return 0;
		case PDS_DATE_TIME_VALUE:
			return (   (s0->date_time.time_type == s1->date_time.time_type)
			        && (s0->date_time.year == s1->date_time.year) 
			        && (s0->date_time.day == s1->date_time.day) 
			        && (s0->date_time.month == s1->date_time.month) 
			        && (s0->date_time.hour == s1->date_time.hour) 
			        && (s0->date_time.minute == s1->date_time.minute) 
			        && (s0->date_time.second == s1->date_time.second) 
			        && (s0->date_time.microsecond == s1->date_time.microsecond) 
			        && (s0->date_time.hour_offset == s1->date_time.hour_offset) 
			        && (s0->date_time.minute_offset == s1->date_time.minute_offset) );
		case PDS_SYMBOLIC_VALUE:
			return PDS_string_compare(s0->symbolic.first, s0->symbolic.last, s1->symbolic.first, s1->symbolic.last);
		case PDS_TEXT_STRING_VALUE:
			return PDS_string_compare(s0->text.first, s0->text.last, s1->text.first, s1->text.last);
		case PDS_IDENTIFIER_VALUE:
			return PDS_string_compare(s0->identifier.first, s0->identifier.last, s1->identifier.first, s1->identifier.last);
		default:
			return 0;
	}
}
