#ifndef PDS_H
#define PDS_H

#include <stdint.h>

/**
 * Status values.
 */
enum PDS_STATUS
{
    PDS_OK = 0,
    PDS_INVALID_VALUE,
    PDS_INVALID_RANGE,
	PDS_MISSING_SEPARATOR,
};
/**
 * Remove leading and trailing white spaces in a string.
 * A white space is either a space (' '), form-feed ('\f'), newline ('\n'),
 * carriage return ('\r'), horizontal tab ('\t') or vertical tab ('\v').
 * @param [in] first Pointer to the first character of the input string.
 * @param [in] last Pointer to the last character of the input string.
 * @param [out] begin Stores the pointer to the first non-space character.
 * @param [out] end Stores the pointer to the last non-space character. 
 */
void PDS_trim(const char *first, const char *last, const char **begin, const char **end);
/**
 * Find the first occurence of a character in a string.
 * @param [in] first Pointer to the first character of the input string.
 * @param [in] last  Pointer to the last character of the input string.
 * @param [in] c     Character to search.
 * @return A pointer to the matched character in the input string or 0 if the
 * character is not found.
 */
const char* PDS_find_first(const char *first, const char *last, char c);
/**
 * Compare two strings.
 * @param [in] f0 Pointer to the first characer of the 1st string.
 * @param [in] l0 Pointer to the last characer of the 1st string.
 * @param [in] f1 Pointer to the first characer of the 2nd string.
 * @param [in] l1 Pointer to the last characer of the 2nd string.
 * @return 1 if the strings are equal, 0 otherwise.
 */
int PDS_string_compare(const char *f0, const char *l0, const char *f1, const char *l1);
/**
 * Compare two strings ignoring case.
 * @param [in] f0 Pointer to the first characer of the 1st string.
 * @param [in] l0 Pointer to the last characer of the 1st string.
 * @param [in] f1 Pointer to the first characer of the 2nd string.
 * @param [in] l1 Pointer to the last characer of the 2nd string.
 * @return 1 if the strings are equal, 0 otherwise.
 */
int PDS_string_case_compare(const char *f0, const char *l0, const char *f1, const char *l1);
/**
 * Parse an integer number.
 * The number can be expressed in decimal or based notation.
 * A based integer is represented by a decimal number representing the base
 * followed by a number expressed in the specified base enclosed between '#'
 * characters.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid
 *                  character of the input string. 
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Parsed integer. In case of overflow INT32_MAX or (INT32_MIN for a
 * negative value) is returned, and the status variable is set to
 * PDS_INVALID_RANGE. If no conversion was performed, 0 is returned and the
 * status variable is set to PDS_INVALUD_VALUE.
 */
int32_t PDS_parse_int(const char* first, const char* last, const char** end, int *status);
/**
 * Parse a floating point value.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid
 *                  character of the input string. 
 * @param [in] base Base in which the unsigned integer is encoded. The base must
 *                  be in the range 2 to 16. 
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Parsed floating point value. If an error occured or no conversion occured
 * 0.0 is returned.
 */
double PDS_parse_real(const char *first, const char *last, const char **end, int *status);
/**
 * Parse identifier. 
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid 
 *                  character of the input string.
 * @param [in][out] status Status variable set to PDS_OK if the string contains
 *                         a valid unit or PDS_INVALID_VALUE.
 * @return Pointer to the beginning of the identifier or 0 if the string is invalid.
 */
const char* PDS_parse_identifier(const char *first, const char *last, const char **end, int *status);

/* [todo]
 * struct value
 * {
 *    union { int32_t, double, };
 *    type { INTEGER, REAL }
 *    unit { enum from pdsdd.full }
 * }
 */
/* [todo] parse string */
/* [todo] parse literal */
/* [todo] parse date */
/* [todo] parse sequence */
/* [todo] parse set */
/* [todo] parse assignement */
/* [todo] parse pointer */
/* [todo] parse object */
/* [todo] parse group */
#endif /* PDS_H */

#ifdef PDS_IMPL

#define PDS_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define PDS_isspace(c) (    (' '  == (c))  || ('\f' == (c)) \
                         || ('\n' == (c))  || ('\r' == (c)) \
                         || ('\t' == (c))  || ('\v' == (c)) )
#define PDS_isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define PDS_islower(c) (((c) >= 'a') && ((c) <= 'z'))
#define PDS_isupper(c) (((c) >= 'A') && ((c) <= 'Z'))
#define PDS_isalpha(c) (PDS_islower(c) || PDS_isupper(c))
#define PDS_isalnum(c) (PDS_isalpha(c) || PDS_isdigit(c))
#define PDS_toupper(c) (PDS_islower(c) ? ('A' + ((c)-'a')) : (c))
#define PDS_tolower(c) (PDS_isupper(c) ? ('a' + ((c)-'A')) : (c))

/*
 * Remove leading and trailing white spaces in a string.
 * A white space is either a space (' '), form-feed ('\f'), newline ('\n'),
 * carriage return ('\r'), horizontal tab ('\t') or vertical tab ('\v').
 */
void PDS_trim(const char *first, const char *last, const char **begin, const char **end)
{
    for(; (first <= last) && PDS_isspace(*first); ++first) 
    {}
    for(; (last>=first) && PDS_isspace(*last); --last)
    {}
	*begin = (first<=last) ? first : 0;
	*end   = (first<=last) ? last  : 0;
}
/*
 * Find the first occurence of a character in a string.
 */
const char* PDS_find_first(const char *first, const char *last, char c)
{
	while(first<=last)
	{
		if(c == *first++)
		{
			return first-1;
		}
	}
	return 0;
}
/*
 * Compare two strings.
 */
int PDS_string_compare(const char *f0, const char *l0, const char *f1, const char *l1)
{
	for(; (f0<=l0) && (f1<=l1) && (*f0==*f1); ++f0, ++f1)
	{}
	return ((f0>l0)&&(f1>l1));
}
/*
 * Compare two strings ignoring case.
 */
int PDS_string_case_compare(const char *f0, const char *l0, const char *f1, const char *l1)
{
	for(; (f0<=l0) && (f1<=l1) && (PDS_toupper(*f0) == PDS_toupper(*f1)); ++f0, ++f1)
	{}
	return ((f0>l0)&&(f1>l1));
}
/*
 * Parse an integer in the specified base.
 */
static int32_t parse_int(const char *first, const char *last, const char **end, int base, int *status)
{
    const char *ptr;
    int64_t current;
    int64_t accumulator;
    int64_t cutoff;
    int64_t cutlimit;
    int any;
    int neg;
    /* Sanity check. */
    if(PDS_OK != *status)
    {
        return 0;
    }
    if((base<2) || (base>16))
    {
        *status = PDS_INVALID_VALUE;
        return INT32_MAX;
    }
    /* Trim spaces. */
    for(ptr=first; (ptr<=last) && PDS_isspace(*ptr); ++ptr)
    {}
    /* Check for possible sign. */
    if('-' == *ptr)
    {
        neg = 1;
        ++ptr;
    }
    else 
    {
        neg = 0;
        if('+' == *ptr)
        {
            ++ptr;
        }
    }
    /*
     * Compute the cutoff value between legal and illegal number of the
     * accumulator prior to adding the last read digit. If the accumulator is
     * egal to this cutoff value, the last read digit must not produce an
     * invalid result.
     * In other words, the cuttoff value is MAX_INT/base and the limit is
     * MAX_INT%base. 
     */
    cutoff   = neg ? -(int64_t)INT32_MIN : INT32_MAX; 
    cutlimit = cutoff % base;
    cutoff  /= base; 
    /* Parse integer. */
    for(accumulator=0, any=0; ptr<=last; ++ptr)
    {
        if(PDS_isdigit(*ptr))
        {
            current = *ptr - '0';
        }
        else if(PDS_isalpha(*ptr))
        {
            current = *ptr - (PDS_islower(*ptr) ? ('a'-10) : ('A'-10));
        }
        else
        {
            break;
        }
        if(current >= base)
        {
            break;
        }
        if(any >= 0)
        {
            if(   (accumulator >  cutoff)
               || ((accumulator == cutoff) && (current > cutlimit)) )
            {
                any = -1;
            }
            else
            {
                any = 1;
                accumulator *= base;
                accumulator += current;
            }
        }
    }
    if(0 != end)
    {
        *end = any ? ptr : first;
    }
    if(any < 0)
    {
        *status = PDS_INVALID_RANGE;
        return neg ? INT32_MIN : INT32_MAX; 
    }
    else
    {
        *status = any ? PDS_OK : PDS_INVALID_VALUE;
        return (int32_t)(neg ? -accumulator : accumulator);
    }
}
/*
 * Parse an integer number.
 */
int32_t PDS_parse_int(const char* first, const char* last, const char** end, int *status)
{
    const char *ptr;
    int32_t value;
    /* Sanity check. */
    if(PDS_OK != *status)
    {
        return INT32_MAX;
    }
    if(0 == end)
    {
        *status = PDS_INVALID_VALUE;
        return INT32_MAX;   
    }
    *end = first;
    /* Parse value. */
    value = parse_int(first, last, &ptr, 10, status);
    if(PDS_OK != *status)
    {
        return INT32_MAX;
    }
    /* 
     * If the character following the current number is a '#', this may mean
     * that the number is in fact expressed in based notation.
     */
    if((ptr >= last) || (*ptr != '#'))
    {
        *end = ptr;
        return value;
    }
    if((value < 2) || (value > 16))
    {
        *status = PDS_INVALID_RANGE;
        return INT32_MAX;
    }
    ++ptr;
    value = parse_int(ptr, last, end, value, status);
    if(PDS_OK == *status)
    {
        /* Check that the number is followed by a closing '#'. */
        if('#' != **end)
        {
            *status = PDS_INVALID_VALUE;
            value = INT32_MAX;
        }
        else
        {
            ++*end;
        }
    }
    return value;
}
/*
 * Parse a floating point value.
 */
double PDS_parse_real(const char *first, const char *last, const char **end, int *status) 
{
    double value;
    int neg = 0;
    int div;
    int exponent;
    int32_t integer;
    int32_t decimal;
    const char *ptr;
    *end = first;
    /* Integer part (can be negative). */   
    integer = parse_int(first, last, &ptr, 10, status);
    /* The integer part can be empty (ex: .03). */
    if(ptr == first)
    {
        integer = 0;
        *status = PDS_OK;
        if('-' == *ptr)
        {
            neg = 1;
            ++ptr;
        }
        else if('+' == *ptr)
        {
            ++ptr;
        }
    }
    else if(integer < 0)
    {
        neg = 1;
    }
    if(PDS_OK != *status)
    {
        return 0.0;
    }
    /* Check for decimal part. */
    if('.' != *ptr)
    {
        *end = ptr;
        return (PDS_OK == *status) ? (double)integer : 0.0;
    }

    first = ptr+1;
    decimal = parse_int(first, last, &ptr, 10, status);
    if((ptr == first) && (0 == decimal))
    {
        *status = PDS_OK;
    }
    if(PDS_OK != *status)
    {
        return 0.0;
    }
    if(decimal < 0)
    {
        *status = PDS_INVALID_VALUE;
        return 0.0;
    }
    for(div=1; first<ptr; ++first, div*=10)
    {}
    value = integer + ((neg?-decimal:decimal) / (double)div);   
    
    /* And lastly the exponent. */
    exponent = 1;
    if(('e' == *ptr) || ('E' == *ptr))
    {
        int32_t i;
        int32_t n;
        first = ptr+1;
        n = parse_int(first, last, &ptr, 10, status);
        if(PDS_OK != *status)
        {
            *status = PDS_INVALID_VALUE;
            return 0.0;
        }
        if(n < 0)
        {
            for(i=0, div=1; i>n; --i, div*=10)
            {}
            value /= (double)div;
        }
        else
        {
            for(i=0, exponent=1; i<n; ++i, exponent*=10)
            {}
            value *= (double)exponent;
        }
    }
    *end = ptr;
    return value;   
}
/*
 * Parse identifier. 
 */
const char* PDS_parse_identifier(const char *first, const char *last, const char **end, int *status)
{
	const char *ptr;
	char c, previous;
	
	if(PDS_OK != *status)
	{
		*end = first;
		return 0;
	}
	
	ptr = first;	
	/* First character must be alphanumeric. */
	previous = c = *ptr++;
	if(!PDS_isalnum(c))
	{
		*end    = first;
		*status = PDS_INVALID_VALUE;
		return 0;
	}
	
	/* Then follows alphanumeric or '_'. */
	while(ptr<=last)
	{
		previous = c;
		c = *ptr++;
		
		if('_' == c)
		{
			if(!PDS_isalnum(previous))
			{
				*status = PDS_INVALID_VALUE;
				break;
			}
		}
		else if(!PDS_isalnum(c))
		{
				break;
		}
	}
	/* The last valid character must be alphanumeric. */
	if((PDS_OK == *status) && PDS_isalnum(previous) && ('_' != c))
	{
		*end = ptr;
		return first;
	}
	*end    = first;
	*status = PDS_INVALID_VALUE;
	return 0;
}

/**
 * PDS token type.
 * These types correspond to the type of assignment they belong to.
 */
enum PDS_TOKEN_TYPE
{
	PDS_TOKEN_UNKNOWN = 0,
	PDS_TOKEN_ATTRIBUTE,
	PDS_TOKEN_POINTER,
	PDS_TOKEN_GROUP,
	PDS_TOKEN_OBJECT,
};
// [todo] subtype/flags
/**
 * PDS token.
 */
typedef struct
{
	/** Pointer to the first character of the token. **/ 
	const char *first;
	/** Pointer to the last character of the token. **/
	const char *last;
	/** Token type. **/
	int type;
	// [todo] subtype/flags
	// [todo] type
	// [todo] multiline?
} PDS_token;
// [todo] struct PDS_lexer
/**
 * Parse pointer identifier.
 * @param [in][out] lhs Token to be parsed.
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return 1 if the pointer identifier was succesfully parsed, or 0 otherwise.
 */
static int parse_lhs_pointer(PDS_token *lhs, int *status)
{
	const char *begin = 0;
	const char *end   = 0;	

	if(PDS_OK != *status)
	{
		return 0;
	}
		
	begin = PDS_parse_identifier(lhs->first+1, lhs->last, &end, status);
	if(PDS_OK != *status)
	{
		return 0;
	}
	else if ((begin != lhs->first+1) || (end != lhs->last+1))
	{
		*status = PDS_INVALID_VALUE;
		return 0;
	}
	else
	{
		/* Skip pointer character (^). */
		++lhs->first;
		return 1;
	}
} 
/**
 * Parse attribute name.
 * A attribute name is an identifier or the concatenation of a namespace and an identifier separated by ':'.
 * @param [in][out] lhs Token to be parsed.
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return 1 if the pointer identifier was succesfully parsed, or 0 otherwise.
 */
static int parse_lhs_attribute(PDS_token *lhs, int *status)
{
	const char *begin = 0;
	const char *end   = 0;

	if(PDS_OK != *status)
	{
		return 0;
	}

	begin = PDS_parse_identifier(lhs->first, lhs->last, &end, status);
	if(0 == begin)
	{
		return 0;
	}
	/* If the string was not fully parsed, check for "namespace:identifier" form. */
	if(end < lhs->last)
	{
		/* Check for namespace separator. */
		if(':' != *end)
		{
			*status = PDS_INVALID_VALUE;
			return 0;
		}
		/* Parse attribute identifier. */
		begin = PDS_parse_identifier(end+1, lhs->last, &end, status);
		if(0 == begin)
		{
			return 0;
		}
	}
	/* At this point the whole string should have been parsed. */
	if(end != lhs->last+1)
	{
		*status = PDS_INVALID_VALUE;
		return 0;
	}
	return 1;	
}
/**
 * Parse left hand side token.
 * [todo] format rules
 * lhs_token   := attribute_id | group | object | pointer
 * attibute_id := identifier | namespace:identifier
 * namespace   := identifier
 * group       := 'begin_group' | 'end_group'
 * object      := 'begin_object' | 'end_object'
 * pointer     := ^identifier
 * @param [in][out] token [todo].
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return  
 */
static int parse_lhs(PDS_token *lhs, int *status)
{
	static const char name_buffer[] = "^end_groupend_object";

	static const struct
	{
		off_t first;
		off_t last;
		int   type;
		// [todo] subtype/flags
		int (*parse) (PDS_token *token, int *status);
	} lhs_parser[] =
	{
		{ 0, 0, PDS_TOKEN_POINTER,   parse_lhs_pointer },   /* pointer    */
		{ 5, 9, PDS_TOKEN_GROUP,     0 },                   /* group      */
		{ 1, 9, PDS_TOKEN_GROUP,     0 },                   /* end_group  */
		{14,19, PDS_TOKEN_OBJECT,    0 },                   /* object     */
		{10,19, PDS_TOKEN_OBJECT,    0 },                   /* end_object */
		{-1,-1, PDS_TOKEN_ATTRIBUTE, parse_lhs_attribute }, /* attribute  */
	};
	int i;
	int ret;
	/* Sanity check. */
	if(PDS_OK != *status)
	{
		return 0;
	}

	for(i=0; i<PDS_ARRAY_SIZE(lhs_parser); i++)
	{
		ret = (lhs_parser[i].first >= 0)
		      ? PDS_string_case_compare(lhs->first, lhs->last, name_buffer+lhs_parser[i].first, name_buffer+lhs_parser[i].last)
			  : 0;

		if(0 == ret)
		{
			if((0 != lhs_parser[i].parse) && (0 == lhs_parser[i].parse(lhs, status)))
			{
				// Parsing failed!
				break;
			}
			lhs->type = lhs_parser[i].type;
			// [todo] subtype/flags ...
			return 1;	 
		}
	}

	/* No valid token found. */ 
	if(PDS_OK == *status)
	{
		*status = PDS_INVALID_VALUE;
	}
	return 0;
}
#if 0
static int skip_whitespaces(/*[todo]*/, const char **end, int *status)
{
	while(first<=last)
	{
		// Skip spaces and keep track of the current line number. 
		for(;(first<=last) && PDS_isspace(*first); *first++)
		{
			// if(*first == '\n') { ++line; }
		}
		// Skip comment.
		if((first<=last) && ('/' == *first++))
		{
			if(first>=last)
			{
				return 0;
			}
			if('*' != *first++)
			{
				return 0;
			}
			for( ;first<=last; first++)
			{
				if(('\r' == *first) || ('\n' == *first))
				{
					return 0;
				}
				if('/' == *first)
				{
					if('*' == *(first-1))
					{
						break;
					}
					if((first < last) && ('*' == *(first+1))
					{
						// [todo] nested comment
						return 0;
					}
				}
			}
		}
		else
		{
			/* Not a comment. */
			break;
		}	
	}
	*end = first;
	return 1;
}
#endif

#endif /* PDS_IMPL */
