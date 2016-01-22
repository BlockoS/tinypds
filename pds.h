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
	double value;
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

/* [todo] parser run */

#ifdef __cplusplus
}
#endif

#endif /* PDS_H */

/*****************************************************************************/

#ifdef PDS_IMPL

#define PDS_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static inline int PDS_isspace(char c)
{
	return ( (' '  == (c))  || ('\t' == (c))
          || ('\r' == (c))  || ('\n' == (c)) 
          || ('\f' == (c))  || ('\v' == (c)) );
}
static inline int PDS_isdigit(char c)
{
	return ((c) >= '0') && ((c) <= '9');
}
static inline int PDS_islower(char c)
{
	return ((c) >= 'a') && ((c) <= 'z');
}
static inline int PDS_isupper(char c)
{
	return ((c) >= 'A') && ((c) <= 'Z');
}
static inline int PDS_isalpha(char c)
{
	return PDS_islower(c) || PDS_isupper(c);
}
static inline int PDS_isalnum(char c)
{
	return PDS_isalpha(c) || PDS_isdigit(c);
}
static inline int PDS_toupper(char c)
{
	return PDS_islower(c) ? ('A' + ((c)-'a')) : (c);
}
static int PDS_tolower(char c)
{
	return PDS_isupper(c) ? ('a' + ((c)-'A')) : (c);
}

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
	// int (*attribute) (const char *id, const PDS_scalar *scalar, void *user_data);

	// int (*pointer) (const char *id, const PDS_value *value, void *user_data);

	// int (*set_begin) (const char *id, int element_count, void *user_data);
	// int (*set_element) (const PDS_scalar *scalar, void *user_data);
	// int (*set_end) (const char *id, void *user_data);

	/** Sequence start callback. **/	
	int (*sequence_begin) (const char *first, const char *last, void *user_data);
	/** Element sequence callback. **/
	int (*sequence_element) (const PDS_scalar *scalar, void *user_data);
	/** Sequence end callback. **/
	int (*sequence_end) (void *user_data);

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
 * @return 1 if the string contains a valid integer, 0 otherwise.
 */
static int PDS_parse_int(PDS_parser *parser)
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
 * @param [in][out] parser Parser.
 * @return 1 if a floating point value was successfully parser, or 0 if an error occured. 
 */
static int PDS_parse_real(PDS_parser *parser) 
{
    double value;
    int neg = 0;
    int div;
    int exponent;
    int32_t integer;
    int32_t decimal;
    const char *ptr;
    /* Integer part (can be negative). */   
    integer = parse_int(parser->current, parser->last, &ptr, 10, &parser->status);
    /* The integer part can be empty (ex: .03). */
    if(ptr == parser->current)
    {
        integer = 0;
        parser->status = PDS_OK;
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
    if(PDS_OK != parser->status)
    {
		PDS_error(parser, parser->status, "invalid value");
        return 0;
    }
    
	value = (double)integer;
	/* Check for decimal part. */
    if('.' == *ptr)
    {
		const char *first = ptr+1;
		decimal = parse_int(first, parser->last, &ptr, 10, &parser->status);
		if((ptr == first) && (0 == decimal))
		{
			parser->status = PDS_OK;
		}
		if(PDS_OK != parser->status)
		{
			PDS_error(parser, parser->status, "invalid decimal value");
			return 0;
		}
		if(decimal < 0)
		{
			PDS_error(parser, PDS_INVALID_VALUE, "negative decimal value");
			return 0;
		}
		for(div=1; first<ptr; ++first, div*=10)
		{}
		value += ((neg?-decimal:decimal) / (double)div);   
    
		/* And lastly the exponent. */
		exponent = 1;
		if(('e' == *ptr) || ('E' == *ptr))
		{
			int32_t i;
			int32_t n;
			first = ptr+1;
			n = parse_int(first, parser->last, &ptr, 10, &parser->status);
			if(PDS_OK != parser->status)
			{
				PDS_error(parser, PDS_INVALID_VALUE, "invalid exponent value");
				return 0;
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
	}
    parser->current = ptr;
	parser->scalar.type = PDS_REAL_VALUE;
	parser->scalar.real.value = value;
	parser->scalar.real.unit.first = parser->scalar.real.unit.last = 0;
    return 1;   
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
	char c;
	if(PDS_OK != *status)
	{
		*end = first;
		return 0;
	}
	
	ptr = first;	
	/* First character must be an alphabetic character. */
	c = *ptr++;
	if(PDS_isalpha(c))
	{
		/* Then follows alphanumeric or '_'. */
		for(; ptr<=last; ptr++)
		{
			c = *ptr;
			if(!PDS_isalnum(c))
			{
				if('_' == c)
				{
					++ptr;
					c = *ptr;
					if(!PDS_isalnum(c))
					{
						*status = PDS_INVALID_VALUE;
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		/* The last valid character must be alphanumeric. */
		if(PDS_OK == *status)
		{
			*end = ptr;
			return first;
		}
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
/*****************************************************************************/
/**
 * Parse attribute name.
 * A attribute name is an identifier or the concatenation of a namespace and an identifier separated by ':'.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid 
 *                  character of the input string.
 * @param [in][out] status Status variable set to PDS_OK if an integer was
 *                  successfully parsed. If an error occured it is set to 
 *                  PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Pointer to the beginning of the attribute name, 0 otherwise.
 */
static const char* parse_lhs_attribute(const char *first, const char *last, const char **end, int *status)
{
	const char *begin;
	if(PDS_OK != *status)
	{
		return 0;
	}

	begin = PDS_parse_identifier(first, last, end, status);
	if(0 == begin)
	{
		return 0;
	}
	/* Check for "namespace:identifier" form. */
	if(*end < last)
	{
		/* Check for namespace separator. */
		if(':' == **end)
		{
			if(0 == PDS_parse_identifier(*end+1, last, end, status))
			{
				*end = first;
				return 0;
			}
		}
	}
	return begin;	
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
 * @param [in][out] parser Parser.
 * @return 1 if the string contains a valid token, 0 otherwise.
 */
static int parse_lhs(PDS_parser *parser)
{
	static const char name_buffer[] = "end_groupend_object";

	static const struct
	{
		off_t first;
		off_t last;
		int   type;
	} lhs_parser[] =
	{
		{ 4, 8, PDS_TOKEN_GROUP  }, /* group      */
		{ 0, 8, PDS_TOKEN_GROUP  }, /* end_group  */
		{13,18, PDS_TOKEN_OBJECT }, /* object     */
		{ 9,18, PDS_TOKEN_OBJECT }, /* end_object */
	};
	int i;
	/* Sanity check. */
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	PDS_token *lhs = &parser->token;
	lhs->first = parser->current;
	/* 1. pointer */
	if('^' == *lhs->first)
	{
		lhs->first = PDS_parse_identifier(lhs->first+1, parser->last, &parser->current, &parser->status);
		if(PDS_OK != parser->status)
		{
			PDS_error(parser, parser->status, "invalid pointer name");
			return 0;
		}
		lhs->last = parser->current-1;
		lhs->type = PDS_TOKEN_POINTER;
		return 1;
	}
	/* 2. group/object */
	for(lhs->last=lhs->first; (lhs->last<=parser->last) && !(PDS_isspace(*lhs->last) || ('/'==*lhs->last)); lhs->last++)
	{}
	lhs->last--;
	for(i=0; i<PDS_ARRAY_SIZE(lhs_parser); i++)
	{
		if(PDS_string_case_compare(lhs->first, lhs->last, name_buffer+lhs_parser[i].first, name_buffer+lhs_parser[i].last))
		{
			parser->current = lhs->last+1;
			lhs->type = lhs_parser[i].type; 
			return 1;
		}
	}
	/* 3. attribute */
	lhs->first = parse_lhs_attribute(parser->current, parser->last, &parser->current, &parser->status);
	if(PDS_OK == parser->status)
	{
		lhs->last = parser->current-1;
		lhs->type = PDS_TOKEN_ATTRIBUTE;
		return 1;
	}

	/* No valid token found. */ 
	PDS_error(parser, PDS_INVALID_VALUE, "no valid token found");
	return 0;
}

/**
 * [todo]
 */
static int parse_scalar_value(PDS_parser *parser)
{
	int ret = 0;
	char c;

	c = *parser->current;
	switch(c)
	{
		case '"':
			ret = PDS_parse_string(parser);
			break;
		case '\'':
			ret = PDS_parse_symbol(parser);
			break;
		default:
			if(PDS_isalpha(c))
			{
				parser->scalar.identifier.first = PDS_parse_identifier(parser->current, parser->last, &parser->current, &parser->status);
				if(0 == parser->scalar.identifier.first)
				{
					ret = 0;
				}
				else
				{
					parser->scalar.identifier.last = parser->current-1;
					parser->scalar.type = PDS_IDENTIFIER_VALUE;
					ret = 1;
				}
			}
			else if(PDS_isdigit(c) || ('-' == c) || ('+' == c) || ('.' == c))
			{
				const char *ptr = parser->current+1;
				const char *eol = PDS_find_first(ptr, parser->last, '\n'); // [todo] store in parser
				if(0 == eol)
				{
					eol = parser->last;
				}
	
				for(; (ptr <= parser->last) && PDS_isdigit(*ptr); ptr++)
				{}
				if(PDS_isdigit(c) && (('-' == *ptr) || (':' == *ptr)))
				{
					ret = PDS_parse_datetime(parser);
				}
				else
				{
					if(('.' == c) || ('.' == *ptr))
					{
						ret = PDS_parse_real(parser);
					}
					else
					{
						ret = PDS_parse_int(parser);
					}
					if(ret)
					{
						ptr = PDS_find_first(parser->current, eol, '<');
						if(ptr)
						{
							parser->current = ptr;
							ret = PDS_parse_unit(parser);
						}
					}
				}
			}
			else
			{
				// [todo]
			}
			break;
	}
	return ret;
}
/**
 * [todo]
 */
static int PDS_parse_set(PDS_parser *parser)
{
	int ret = 1;
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	if('{' != *parser->current++)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "missing set separator");
		return 0;
	}
	
	ret = skip_whitespaces(parser);
	if('}' == *parser->current)
	{
		parser->current++;
		return 1;
	}

	while(ret && (parser->current<=parser->last))
	{
		ret = skip_whitespaces(parser);
		if(ret)
		{
			ret = parse_scalar_value(parser);
			if(ret)
			{
				ret = skip_whitespaces(parser);
				if(ret)
				{
					char c = *parser->current++;
					if('}' == c)
					{
						break;
					}
					else if(',' != c)
					{
						ret = 0;
						PDS_error(parser, PDS_INVALID_VALUE, "invalid element separator");
					}
				}
			}
		}
	}
	return ret;
}
/**
 * [todo]
 */
static int PDS_parse_sequence(PDS_parser *parser)
{
	int ret = 1;
	int depth = 1;
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	if('(' != *parser->current++)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "missing sequence separator");
		return 0;
	}
	if(0 != parser->sequence_begin)
	{
		ret = parser->sequence_begin(parser->token.first, parser->token.last, parser->user_data);
		if(!ret)
		{
			return 0;
		}	
	}
	while(ret && (depth > 0) && (parser->current <= parser->last))
	{
		ret = skip_whitespaces(parser);
		if(ret)
		{
			if('(' == *parser->current)
			{
				++parser->current;
				++depth;
				ret = parser->sequence_begin(parser->token.first, parser->token.last, parser->user_data);
				if(!ret)
				{
					break;
				}
			}
			else
			{
				ret = parse_scalar_value(parser);
				if(ret)
				{
					if(0 != parser->sequence_element)
					{
						ret = parser->sequence_element(&parser->scalar, parser->user_data);
						if(!ret)
						{
							break;
						}
					}
					ret = skip_whitespaces(parser);
					if(ret)
					{
						char c;
						while(((c = *parser->current++) == ')') && (depth > 0))
						{
							--depth;
							if(0 != parser->sequence_end)
							{
								ret = parser->sequence_end(parser->user_data);
								if(!ret)
								{
									break;
								}
							}
							if(depth)
							{
								ret = skip_whitespaces(parser);
							}
						}
						if(ret && (depth > 0) && (',' != c))
						{
							ret = 0;
							PDS_error(parser, PDS_INVALID_VALUE, "invalid element separator");
						}
					}
				}
			}
		}
	}
	return ret;
}
/**
 * [todo]
 */
static int parse_rhs(PDS_parser *parser)
{
	int ret = 0;
	char c;

	c = *parser->current;
	switch(c)
	{
		case '(':
			ret = PDS_parse_sequence(parser);
			break;
		case '{':
			ret = PDS_parse_set(parser);
			break;
		default:
			ret = parse_scalar_value(parser);
			break;
	}
	return ret;
}
/*****************************************************************************/
/**
 * [todo]
 */
static int PDS_parse_statement(PDS_parser *parser)
{
	int ret;
	if(PDS_OK != parser->status)
	{
		return 0;
	}

	ret = skip_whitespaces(parser);
	if(!ret)
	{
		return 0;
	}

	ret = parse_lhs(parser);
	if(!ret)
	{
		PDS_error(parser, parser->status, "invalid left-hand value");
		return 0;
	}
	// [todo] if (END) { STOP }
	
	ret = skip_whitespaces(parser);
	if(!ret)
	{
		return 0;
	}

	if('=' != *parser->current)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "missing or invalid statement delimiter");
		return 0;
	}
	++parser->current;

	ret = skip_whitespaces(parser);
	if(!ret)
	{
		return 0;
	}
	
	// [todo] callbacks!
	switch(parser->token.type)
	{
		case PDS_TOKEN_ATTRIBUTE:
		case PDS_TOKEN_POINTER:
			if(0 == parse_rhs(parser))
			{
				PDS_error(parser, parser->status, "invalid right value");
				return 0;
			}
			break;
		case PDS_TOKEN_GROUP:
		case PDS_TOKEN_OBJECT:
			parser->scalar.identifier.first = PDS_parse_identifier(parser->current, parser->last, &parser->current, &parser->status);
			if(PDS_OK != parser->status)
			{
				PDS_error(parser, PDS_INVALID_VALUE, "invalid group identifier");
				return 0;
			}
			parser->scalar.identifier.last = parser->current-1;
			parser->scalar.type = PDS_IDENTIFIER_VALUE;
			break;
		default:
			// [todo] error;
			return 0;
	}

	int line = parser->line;
	ret = skip_whitespaces(parser);
	if(!ret)
	{
		return 0;
	}
	/* Check for new line. */
	if(parser->line <= line)
	{
		PDS_error(parser, PDS_INVALID_VALUE, "no newline at the end of statement");
		return 0;	
	}

	return 1;
}

#endif /* PDS_IMPL */
