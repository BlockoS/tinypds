#ifndef PDS_H
#define PDS_H

// [todo] some minor configuration stuffs.
// [todo] replace char by int8
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Status values.
 */
enum PDS_STATUS
{
    PDS_OK = 0,
    PDS_INVALID_VALUE,
    PDS_INVALID_RANGE,
	PDS_INVALID_ARGUMENT,
};
/**
 * Value types.
 */
enum PDS_VALUE_TYPE
{
	PDS_UNKNOWN_VALUE = 0,
	PDS_INTEGER_VALUE,
	PDS_REAL_VALUE,
	PDS_DATE_TIME_VALUE,
	PDS_TEXT_STRING_VALUE,
	PDS_SYMBOLIC_VALUE,
	PDS_IDENTIFIER_VALUE
};
/**
 * String.
 */
typedef struct
{
	/** Value type. Must be equal to PDS_TEXT_STRING_VALUE. **/
	int type;
	/** Pointer to the first character of the string. **/
	const char *first;
	/** Pointer to the last character of the string. **/
	const char *last;
} PDS_string;
/**
 * Integer value.
 */
typedef struct
{
	/** Value type. Must be equal to PDS_INTEGER_VALUE. **/
	int type;	
	/** Measurement unit. **/
	PDS_string unit;
	/** Integer value. **/
	int32_t value;
} PDS_integer;
/**
 * Real value.
 */
typedef struct
{
	/** Value type. Must be equal to PDS_REAL_VALUE. **/
	int type;	
	/** Measurement unit. **/
	PDS_string unit;
	/** Real (floating-point) value. **/
	float value;
} PDS_real;
/**
 * Time types.
 */
enum PDS_TIME_TYPE
{
	PDS_LOCAL_TIME = 0,
	PDS_UTC_TIME,
	PDS_ZONED_TIME
};
/**
 * Date and time structure.
 */
typedef struct
{
	/** Value type. Must be equal to PDS_DATE_TIME_VALUE. **/
	int type;	
	/** Year in 4 digits form. **/
	uint16_t year;
	/** Day of month (number between 1 and 31). **/
	uint16_t day;
	/** Month (number between 1 and 12). **/
	uint8_t month;
	/** Hour (number between 0 and 23). **/
	uint8_t hour;
	/** Minutes (number between 0 and 59). **/
	uint8_t minute;
	/** Seconds (number between 0 and 59). **/
	uint8_t second;
	/** Microseconds (number between 0 and 999999). **/
	uint32_t microsecond;
	/** Hour time offset (number between -12 and 12). **/
	int8_t hour_offset;
	/** Minute time offset (number between 0 and 59). **/
	uint8_t minute_offset;
	/** Time type @see PDS_TIME_TYPE. **/
	uint8_t time_type;
} PDS_datetime;
/**
 * Scalar value.
 */
typedef union
{
	/** Value type @see PDS_VALUE_TYPE. **/
	int type;
	/** Integer value. **/
	PDS_integer integer;
	/** Real value. **/
	PDS_real real;
	/** Date time. **/
	PDS_datetime date_time;
	/** Text string. **/
	PDS_string text;
	/** Symbolic literal. **/
	PDS_string symbolic;
	/** Identifier. **/
	PDS_string identifier;
} PDS_scalar;

/**
 * Remove leading and trailing white spaces in a string.
 * A white space is either a space (' '), form-feed ('\f'), newline ('\n'),
 * carriage return ('\r'), horizontal tab ('\t') or vertical tab ('\v').
 * @param [in] first Pointer to the first character of the input string.
 * @param [in] last Pointer to the last character of the input string.
 * @param [out] begin Stores the pointer to the first non-space character.
 * @param [out] end Stores the pointer to the last non-space character. 
 */
// [todo] remove if not used.
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


/* [todo] all-in-one value parser */
/* [todo] parse set */
/* [todo] parse sequence */

/* [todo] parse assignement */
/* [todo] parse pointer (rhs) */
/* [todo] parse object (rhs) */
/* [todo] parse group (rhs) */

/* [todo] parser run */

#ifdef __cplusplus
}
#endif

#endif /* PDS_H */

/*****************************************************************************/

#ifdef PDS_IMPL

#define PDS_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// [todo] make functions ?
#define PDS_isspace(c) (    (' '  == (c))  || ('\t' == (c)) \
                         || ('\r' == (c))  || ('\n' == (c)) \
                         || ('\f' == (c))  || ('\v' == (c)) )
#define PDS_isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define PDS_islower(c) (((c) >= 'a') && ((c) <= 'z'))
#define PDS_isupper(c) (((c) >= 'A') && ((c) <= 'Z'))
#define PDS_isalpha(c) (PDS_islower(c) || PDS_isupper(c))
#define PDS_isalnum(c) (PDS_isalpha(c) || PDS_isdigit(c))
#define PDS_toupper(c) (PDS_islower(c) ? ('A' + ((c)-'a')) : (c))
#define PDS_tolower(c) (PDS_isupper(c) ? ('a' + ((c)-'A')) : (c))

#if 0
static const char *PDS_special_chars    = "={}()+-<>.\"\'_,/*:#&^";
static const char *PDS_spacing_chars    = " \t";
static const char *PDS_format_effectors = "\r\n\f\v";
static const char *PDS_other_chars      = "!$%;?@[]`|~";
#endif

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
/**
 * PDS token.
 */
typedef struct
{
	/** Pointer to the first character of the token. **/ 
	const char *first;
	/** Pointer to the last character of the token. **/
	const char *last;
	/** Token type (@see PDS_TOKEN_TYPE). **/
	int type;
	/** Flag (begin/end, ...). **/
	int flag;
} PDS_token;
/**
 * PSD parser.
 */
typedef struct
{
	/** Current status (@see PDS_STATUS). **/
	int status;
	/** Pointer to the first character of the input text. **/
	const char *first;
	/** Pointer to the last character of the input text. **/
	const char *last;
	/** Pointer to the current character. **/
	const char *current;
	/** Line that is being parsed.**/
	int line;
	/** Last valid line. **/
	int last_line;

	// [todo] scratchpad mem

	/** Current token. **/
	PDS_token token;
	/** Current scalar value. **/
	PDS_scalar scalar;
	/** User data. **/
	void *user_data;
	
	// [todo] end callback

	/** New attribute callback. **/
	int (*attribute) (const char *id, const PDS_scalar *scalar, void *user_data);

	// int (*pointer) (const char *id, const PDS_value *value, void *user_data);

	// int (*set_begin) (const char *id, int element_count, void *user_data);
	// int (*set_element) (const PDS_scalar *scalar, void *user_data);
	// int (*set_end) (const char *id, void *user_data);
	
	// int (*sequence_begin) (const char *id, int element_count, void *user_data);
	// int (*sequence_element) (const PDS_scalar *scalar, void *user_data);
	// int (*sequence_end) const char *id, void *user_data);

	// int (*group_begin) (const char *id, void *user_data);
	// int (*group_end) (const char *id, void *user_data);

	// int (*object_begin) (const char *id, void *user_data);
	// int (*object_end) (const char *id, void *user_data);
	
	/** Display error message. **/
	void (*error)(int line, const char *text, void *user_data);
} PDS_parser;

// [todo] create
// [todo] destroy

/**
 * Set error and run error display callback.
 * @param [in][out] parser Parser.
 * @param [in]      error Error status @see PDS_STATUS.
 * @param [in]      message Error message.
 */
static void PDS_error(PDS_parser *parser, int error, const char *message)
{
	parser->status = error;
	if(0 != parser->error)
	{
		parser->error(parser->line, message, parser->user_data);
	}
}

/*****************************************************************************/

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

/*****************************************************************************/

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
/**
 * Parse an integer number.
 * The number can be expressed in decimal or based notation.
 * A based integer is represented by a decimal number representing the base
 * followed by a number expressed in the specified base enclosed between '#'
 * characters.
 * @param [in][out] parser Parser.
 * @return Parsed integer. In case of overflow INT32_MAX or (INT32_MIN for a
 * negative value) is returned, and the status variable is set to
 * PDS_INVALID_RANGE. If no conversion was performed, 0 is returned and the
 * status variable is set to PDS_INVALUD_VALUE.
 */
static int32_t PDS_parse_int(PDS_parser *parser)
{
    const char *ptr;
	int32_t value;
    /* Sanity check. */
    if(PDS_OK != parser->status)
    {
        return 0;
    }
    /* Parse value. */
    value = parse_int(parser->current, parser->last, &ptr, 10, &parser->status);
    if(PDS_OK != parser->status)
    {
		PDS_error(parser, parser->status, "invalid integer value");
        return 0;
    }
    /* 
     * If the character following the current number is a '#', this may mean
     * that the number is in fact expressed in based notation.
     */
    if((ptr < parser->last) && (*ptr == '#'))
    {
		if((value < 2) || (value > 16))
	    {
			PDS_error(parser, PDS_INVALID_RANGE, "invalid integer base");
			return 0;
		}
		const char *current = ptr+1;
		value = parse_int(current, parser->last, &ptr, value, &parser->status);
		if(PDS_OK == parser->status)
		{
			/* Check that the number is followed by a closing '#'. */
			if('#' != *ptr)
			{
				PDS_error(parser, PDS_INVALID_VALUE, "invalid of missing delimiter");
				return 0;
			}
			else
			{
				++ptr;
			}
		}
		else
		{
			PDS_error(parser, parser->status, "invalid integer value");
			return 0;
		}
	}
	parser->current = ptr;
	parser->scalar.type = PDS_INTEGER_VALUE;
	parser->scalar.integer.value = value;
	parser->scalar.integer.unit.first = parser->scalar.integer.unit.last = 0;
    return 1;
}
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
// [todo] replace args by parser
static double PDS_parse_real(const char *first, const char *last, const char **end, int *status) 
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
/**
 * Parse identifier. 
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid 
 *                  character of the input string.
 * @param [in][out] status Status variable set to PDS_OK if the string contains
 *                         a valid identifier or PDS_INVALID_VALUE.
 * @return Pointer to the beginning of the identifier or 0 if the string is invalid.
 */
static const char* PDS_parse_identifier(const char *first, const char *last, const char **end, int *status)
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
 * Parse measurement unit.
 * If the parsing was succesfull, the content of the measurement string can be retreived from first+1 to 
 * end-1(excluded). 
 * @param [in][out] parser Parser.
 * @return 1 if a valid unit was parsed, 0 if the string is invalid.
 */
static int PDS_parse_unit(PDS_parser *parser)
{
	const char *first = parser->current;
	if(PDS_OK != parser->status)
	{
		return 0;
	}
	if((PDS_INTEGER_VALUE != parser->scalar.type) && (PDS_REAL_VALUE != parser->scalar.type))
	{
		PDS_error(parser, PDS_INVALID_ARGUMENT, "invalid argument");
		return 0;
	}
	if('<' != *first)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "invalid unit delimiter");
		return 0;
	}
	++first;
	if(first >= parser->last)
	{
		parser->current = first;
		return 0;
	}
	parser->scalar.integer.unit.first = first;	
	while(first<parser->last)
	{
		/* unit factor */
		for(; (first<parser->last) && (PDS_isalpha(*first) || ('_'==*first)); first++)
		{}
		/* Check for multiplier, exponent or divisor. */
		if(first < parser->last)
		{
			if('*' == *first)
			{
				++first;
				if('*' == *first)
				{
					const char *next = 0;
					(void)parse_int(first+1, parser->last, &next, 10, &parser->status);
					if(PDS_OK != parser->status)
					{
						PDS_error(parser, parser->status, "invalid unit exponent");
						return 0;
					}
					first = next;
				}
				else if(!PDS_isalpha(*first))
				{
					PDS_error(parser, PDS_INVALID_VALUE, "invalid unit");
					return 0;
				}
			}
			else if('/' == *first)
			{
				++first;
				if(!PDS_isalpha(*first))
				{
					PDS_error(parser, PDS_INVALID_VALUE, "invalid unit divisor");
					return 0;
				}
			}
			else if('>' == *first)
			{
				break;
			}
			else
			{
				PDS_error(parser, PDS_INVALID_VALUE, "unauthorized unit character");
				return 0;
			}
		}
	}
	if((first>parser->last) || ('>' != *first))
	{
		PDS_error(parser, PDS_INVALID_VALUE, "missing or invalid unit delimiter");
		return 0;
	}
	parser->scalar.integer.unit.last = first;
	parser->current = first+1;
	return 1;
}
/**
 * Parse a literal symbol.
 * If the parsing was succesfull, the content of the literal value can be retreived from first+1 to 
 * end-1(excluded). 
 * @param [in][out] parser Parser.
 * @return 1 if the string contains a valid literal symbol, 0 if the string is invalid.
 */
static int PDS_parse_symbol(PDS_parser *parser)
{
	const char *first = parser->current;
	if(PDS_OK != parser->status)
	{
		return 0;
	}
	/* The string must start with an apostrophe. */
	if('\'' != *first)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "invalid delimiter");
		return 0;
	}
	++first;
	parser->scalar.symbolic.first = first;
	/* The string must contain at least one valid character. */
	if((first>=parser->last) || ('\'' == *first))
	{
		PDS_error(parser, PDS_INVALID_VALUE, "empty literal symbol");
		return 0;
	}
	/* The string may contain spacing, alpha-numeric, other and special characters except the apostrophe. */
	for(; (first<parser->last) && ('\''!=*first) && (('\t'==*first) || ((*first>=0x20) && (*first<=0x7e))); first++)
	{}
	
	if((first>parser->last) || ('\'' != *first))
	{
		PDS_error(parser, PDS_INVALID_VALUE, "missing literal symbol delimiter");
		return 0;
	}
	parser->scalar.symbolic.last = first;
	parser->scalar.type = PDS_SYMBOLIC_VALUE;
	
	parser->current = first+1;
	
	return 1;
}
/**
 * Parse a quoted string.
 * If the parsing was succesfull, the content of the quoted text can be retreived from first+1 to 
 * end-1(excluded). 
 * @param [in][out] parser Parser.
 * @return 1 if the string contains a valid quoted text, 0 if the string is invalid.
 */
static int PDS_parse_string(PDS_parser *parser)
{
	const char *first = parser->current;
	const char *last  = parser->last;
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	if('"' != *first++)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "invalid or missing string delimiter");
		return 0;
	}

	parser->scalar.text.first = first;

	for(; (first<last) && ('"' != *first) && (PDS_isspace(*first) || ((*first>=0x20) && (*first<=0x7e))); first++)
	{
		/* Check escaped character validity. */
		if('\\' == *first)
		{
			++first;
			if(first < last)
			{
				if(   ('n' != *first) && ('t' != *first) && ('f' != *first) 
				   && ('v' != *first) && ('\\'!= *first) )
				{
					PDS_error(parser, PDS_INVALID_VALUE, "invalid escape character");
					return 0;
				}
			}
		}
		else if('\n' == *first)
		{
			++parser->line;
		} 
	}
	
	if((first>last) || ('"' != *first))
	{
		PDS_error(parser, PDS_INVALID_VALUE, "missing string delimiter");
		return 0;
	}
	
	parser->scalar.type = PDS_TEXT_STRING_VALUE;
	parser->scalar.text.last  = first;
 
	parser->current = first+1;
	return 1;
}
/**
 * Parse date.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid 
 *                  character of the input string.
 * @param [out] date Date.
 * @param [in][out] status Status variable set to PDS_OK if the string contains
 *                         a valid date or PDS_INVALID_VALUE.
 * @return 1 if the string contains a valid date, 0 if the string is invalid.
 */
static int parse_date(const char *first, const char *last, const char **end, PDS_datetime *date, int *status)
{
	int32_t value;
	const char *next = 0;
	*end = first;
	if(PDS_OK != *status)
	{
		return 0;
	}
	/* Year */
	value = parse_int(first, last, &next, 10, status);
	if(PDS_OK != *status)
	{
		return 0;
	}
	/* Separator */
	first = next;
	if((first >= last) || ('-' != *first))
	{
		/* No error is set because this may be a time value. */
		if(':' != *first)
		{
			*status = PDS_INVALID_VALUE;
		}
		return 0;
	}
	if((value < 1970) || (value > 9999))
	{
		*status = PDS_INVALID_RANGE;
		return 0;
	}
	date->year = (uint16_t)value;
	++first;
	/* Month or day-of-year. */
	value = parse_int(first, last, &next, 10, status);
	if(PDS_OK != *status)
	{
		return 0;
	}
	/* If the next character is a date separator (-), we have a month */
	/* else it's a day-of-year. */ 
	if('-' == *next)
	{
		/* Month */
		if((value >= 1) && (value <= 12))
		{
			date->month= (uint8_t)value;
			/* Day of month. */
			value = parse_int(next+1, last, &next, 10, status);
			if(PDS_OK != *status)
			{
				return 0;
			}
			if((value >= 1) && (value <= 31))
			{
				date->day = (uint16_t)value;
				*end = next;
				return 1;
			}
		}
		*status = PDS_INVALID_RANGE;
		return 0;
	}
	else if((value >= 1) && (value <= 366))
	{
		/* day-of-year. */
		date->day   = (uint16_t)value;
		date->month = 0;
		*end = next;
	}
	else
	{
		*status = PDS_INVALID_RANGE;
		return 0;
	}
	return 1;
}
/**
 * Parse time.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid 
 *                  character of the input string.
 * @param [out] date Date.
 * @param [in][out] status Status variable set to PDS_OK if the string contains
 *                         a valid date or PDS_INVALID_VALUE.
 * @return 1 if the string contains a valid date, 0 if the string is invalid.
 */
static int parse_time(const char *first, const char *last, const char **end, PDS_datetime *date, int *status)
{
	int32_t value = 0;
	const char *next = 0;

	*end = first;

	if(PDS_OK != *status)
	{
		return 0;
	}

	/* Hour. */
	value = parse_int(first, last, &next, 10, status);
	if(PDS_OK != *status)
	{
		return 0;
	}
	if((value < 0) || (value > 23))
	{
		*status = PDS_INVALID_RANGE;
		return 0;
	}
	date->hour = (uint8_t)value;
	/* Separator. */
	if(':' != *next)
	{
		*status = PDS_INVALID_VALUE;
		return 0;
	}
	/* Minutes. */
	value = parse_int(next+1, last, &next, 10, status);
	if(PDS_OK != *status)
	{
		return 0;
	}
	if((value < 0) || (value > 59))
	{
		*status = PDS_INVALID_RANGE;
		return 0;
	}
	date->minute = (uint8_t)value;
	/* Seconds? */
	if(':' == *next)
	{
		first = next+1;
		value = parse_int(next+1, last, &next, 10, status);
		if('.' == *next)
		{		
			int32_t tmp, i;
			first = next+1;
			*status = PDS_OK;
			tmp = parse_int(first, last, &next, 10, status);
			if(PDS_OK != *status)
			{
				return 0;
			}
			if((tmp < 0) || (tmp > 999999))
			{
				*status = PDS_INVALID_RANGE;
				return 0;
			}
			for(i=(int32_t)(next-first); i<6; i++, tmp*=10)
			{}
			date->microsecond = (uint32_t)tmp;
		}
		if(PDS_OK != *status)
		{
			return 0;
		}
		if((value < 0) || (value > 59))
		{
			*status = PDS_INVALID_RANGE;
			return 0;
		}
		date->second = (uint8_t)value; 
	}
	/* UTC time. */
	if('Z' == *next)
	{
		next++;
		date->time_type = PDS_UTC_TIME;
	}
	else if(('+' == *next) || ('-' == *next))
	{
		date->time_type = PDS_ZONED_TIME;
		/* Hour offset. */
		value = parse_int(next, last, &next, 10, status);
		if(PDS_OK != *status)
		{
			return 0;
		}
		if((value < -12) || (value > 12))
		{
			*status = PDS_INVALID_RANGE;
			return 0;
		}
		date->hour_offset = (int8_t)value;
		/* Minute offset? */
		if(':' == *next)
		{
			value = parse_int(next+1, last, &next, 10, status);
			if(PDS_OK != *status)
			{
				return 0;
			}
			if((value < 0) || (value > 59))
			{
				*status = PDS_INVALID_RANGE;
				return 0;
			}
			date->minute_offset = (int8_t)((date->hour_offset<0) ? -value : value);
		}
	}
	else
	{
		date->time_type = PDS_LOCAL_TIME;
	}

	*end = next;
	return 1;
}
/**
 * Parse a date and time.
 * A date is either a date, a time or a combinate of date and time string.
 * @param [in][out] parser Parser.
 * @return 1 if the string contains a valid date, 0 if the string is invalid.
 */
static int PDS_parse_datetime(PDS_parser *parser)
{
	int ret = 0;
	const char *next = 0;

	PDS_datetime *date = &parser->scalar.date_time;
	parser->scalar.type = PDS_UNKNOWN_VALUE;
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	date->year = date->month = 0;
	date->day  = 0;
	
	date->hour = date->minute = date->second = 0;
	date->microsecond = 0;
	date->hour_offset = date->minute_offset = 0;

	date->time_type = PDS_LOCAL_TIME;
	
	ret = parse_date(parser->current, parser->last, &next, date, &parser->status);	
	if(PDS_OK != parser->status)
	{
		PDS_error(parser, parser->status, "invalid date");
		return 0;
	}
	if(ret)
	{	
		if('T' == *next)
		{
			++next;
		}
		else 
		{
			parser->scalar.type = PDS_DATE_TIME_VALUE;
			parser->current = next;
			return 1;
		}
	}
	ret = parse_time(next, parser->last, &next, date, &parser->status);
	if(PDS_OK == parser->status)
	{
		parser->scalar.type = PDS_DATE_TIME_VALUE;
		parser->current = next;
	}
	else
	{
		PDS_error(parser, parser->status, "invalid time");	
	}
	return ret;
}
/**
 * [todo]
 */
static int PDS_parse_set(const char *first, const char *last, const char **end, /*[todo] parser, */int *status)
{
	return 1;
}
/**
 * [todo]
 */
static int PDS_parse_sequence(const char *first, const char *last, const char **end, /* [todo] parser, */ int *status)
{
	return 1;
}
/**
 * [todo]
 */
static int PDS_parse_statement(PDS_parser *parser, int *status)
{
	// skip_whitespace
	// parse_lhs
	// if (END) { STOP }
	// skip_whitespace
	// check for equal sign
	// skip_whitespace
	// switch(type)
	//		case begin/end_object: parse identifier
	//		case begin/end_group: parse identifier
	//		case pointer : parse ?
	//		case attribute : check first character
	//							( => sequence
	//							{ => set
	//							" => quote
	//							' => symbolic literal
	//							-/+/[0-9] => scalar value
	//							[a-zA-Z] => identifier
	// skip_whiespace
	// if no line jump => error
	return 1;
}

/*****************************************************************************/
/**
 * Parse pointer identifier.
 * @param [in][out] lhs Token to be parsed.
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return 1 if the pointer identifier was succesfully parsed, or 0 otherwise.
 */
// [todo] replace args by parser
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
// [todo] replace args by parser
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
 !* @param [in][out] token [todo].
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return  
 */
// [todo] END token!!!!!
// [todo] replace args by parser
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
/*****************************************************************************/
/**
 * Skip whitespaces and comments from string.
 * A white space is either a space (' '), form-feed ('\f'), newline ('\n'),
 * carriage return ('\r'), horizontal tab ('\t') or vertical tab ('\v').
 * Comments are C-like comments, but unlike them they must fit on a single line.
 * Nested comments are forbidden.
 * @param [in][out] parser PDS parser context.
 * @return 0 if an error occured, 1 upon success.
 */
static int skip_whitespaces(PDS_parser *parser)
{
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	for( ;parser->current <= parser->last; parser->current++)
	{
		/* Skip spaces and keep track of the current line number. */
		if(PDS_isspace(*parser->current))
		{
			if('\n' == *parser->current)
			{
				++parser->line; 
			}
		}
		/* Skip comment. */
		else if((parser->current <= parser->last) && ('/' == *parser->current))
		{
			++parser->current;
			if(parser->current >= parser->last)
			{
				PDS_error(parser, PDS_INVALID_VALUE, "premature end of input");
				return 0;
			}
			if('*' != *parser->current)
			{
				PDS_error(parser, PDS_INVALID_VALUE, "invalid input");
				return 0;
			}
			++parser->current;
			for( ;parser->current <= parser->last; parser->current++)
			{
				if(('\r' == *parser->current) || ('\n' == *parser->current))
				{
					PDS_error(parser, PDS_INVALID_VALUE, "multi-line comment");
					return 0;
				}
				if('/' == *parser->current)
				{
					if('*' == *(parser->current-1))
					{
						break;
					}
					if((parser->current < parser->last) && ('*' == *(parser->current+1)))
					{
						PDS_error(parser, PDS_INVALID_VALUE, "nested comments");
						return 0;
					}
				}
			}
		}
		else
		{
			/* Not a space nor a comment. */
			break;
		}
	}
	return 1;
}

#endif /* PDS_IMPL */
