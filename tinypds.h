/* 
 * Tiny PDS3 parser
 *
 * This file provides both the interface and the implementation.
 * To instantiate the implementation add the following in *ONE* source file before including this file 
 *     #define TINY_PDS_IMPL
 *
 * Licensed under the MIT License
 * (c) 2016 Vincent Cruz
 */
#ifndef TINY_PDS_H
#define TINY_PDS_H
#include <stdint.h>
#include <sys/types.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Status values.
 */
enum PDS_STATUS
{
    PDS_END = -1,
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
    /** Unknown value. **/
    PDS_UNKNOWN_VALUE = 0,
    /** Integer value stored as a signed 64 bits integer. **/
    PDS_INTEGER_VALUE,
    /** Real value stored as a double precision floating point. **/
    PDS_REAL_VALUE,
    /** Date/time value @see PDS_datetime **/
    PDS_DATE_TIME_VALUE,
    /** Quoted string. **/
    PDS_TEXT_STRING_VALUE,
    /** Symbolic literal. **/
    PDS_SYMBOLIC_VALUE,
    /** Identifier. **/
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
    struct { const char *first, *last; } unit;
    /** Integer value. **/
    int64_t value;
} PDS_integer;
/**
 * Real value.
 */
typedef struct
{
    /** Value type. Must be equal to PDS_REAL_VALUE. **/
    int type;   
    /** Measurement unit. **/
    struct { const char *first, *last; } unit;
    /** Real (floating-point) value. **/
    double value;
} PDS_real;
/**
 * Time types.
 */
enum PDS_TIME_TYPE
{
    /** Local timezone. **/
    PDS_LOCAL_TIME = 0,
    /** UTC time. **/
    PDS_UTC_TIME,
    /** Zoned time (UTC+offset). **/
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
    int8_t minute_offset;
    /** Time type @see PDS_TIME_TYPE **/
    uint8_t time_type;
} PDS_datetime;
/**
 * Scalar value.
 */
typedef union
{
    /** Value type @see PDS_VALUE_TYPE **/
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

/** New element callback. **/
typedef int (*PDS_begin_callback) (const char *first, const char *last, void *user_data);
/** End of current element callback. **/
typedef int (*PDS_end_callback) (const char *first, const char *last, void *user_data);
/** New scalar callback. **/
typedef int (*PDS_scalar_callback) (const PDS_scalar *scalar, void *user_data);
/** New collection callback. **/
typedef int (*PDS_collection_begin_callback) (void *user_data);
/** End of collection callback. **/
typedef int (*PDS_collection_end_callback) (void *user_data);
/** Error callback. **/
typedef void (*PDS_error_callback)(int line, const char *text, void *user_data);
/**
 * PSD parser callbacks.
 */
typedef struct
{
    struct
    {   /** Declaration start callback. **/
        PDS_begin_callback begin;
        /** Declaration end callback.   **/
        PDS_end_callback end;
    } attribute /** Attribute callbacks. **/
    , pointer   /** Pointer callbacks.   **/
    , group     /** Group callbacks.     **/ 
    , object;   /** Object callbacks.    **/
    struct
    {   /** Collection start callback. **/
        PDS_collection_begin_callback begin;
        /** Collection element callback. **/
        PDS_scalar_callback element;
        /** Collection end callback.   **/
        PDS_collection_end_callback end;
    } set       /** Set callbacks.       **/ 
    , sequence; /** Sequence callbacks.  **/
    /** Scalar callback. **/
    PDS_scalar_callback scalar;
    /** Display error message. **/
    PDS_error_callback error;
} PDS_callbacks;
/**
 * Remove leading and trailing white spaces in a string.
 * A white space is either a space (' '), form-feed ('\\f'), newline ('\\n'),
 * carriage return ('\\r'), horizontal tab ('\\t') or vertical tab ('\\v').
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
 * Set attribute callbacks.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     begin     Attribute start callback.
 * @param [in]     end       Attribute end callback.
 */
void PDS_set_attribute_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                           PDS_end_callback   end);
/**
 * Set pointer callbacks.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     begin     Pointer start callback.
 * @param [in]     end       Pointer end callback.
 */
void PDS_set_pointer_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin,
                                                         PDS_end_callback   end);
/**
 * Set group callbacks.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     begin     Group start callback.
 * @param [in]     end       Group end callback.
 */
void PDS_set_group_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                       PDS_end_callback   end);
/**
 * Set object callbacks.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     begin     Object start callback.
 * @param [in]     end       Object end callback.
 */
void PDS_set_object_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                        PDS_end_callback   end);
/**
 * Set 'set' callbacks.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     begin     Set start callback.
 * @param [in]     element   Set element callback.
 * @param [in]     end       Set end callback.
 */
void PDS_set_set_callbacks(PDS_callbacks *callbacks, PDS_collection_begin_callback begin,
                                                     PDS_scalar_callback           element,
                                                     PDS_collection_end_callback   end);
/**
 * Set sequence callbacks.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     begin     Sequence start callback.
 * @param [in]     element   Sequence element callback.
 * @param [in]     end       Sequence end callback.
 */
void PDS_set_sequence_callbacks(PDS_callbacks *callbacks, PDS_collection_begin_callback begin,
                                                          PDS_scalar_callback           element,
                                                          PDS_collection_end_callback   end);
/**
 * Set scalar callback.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     error     Scalar callback.
 */
void PDS_set_scalar_callback(PDS_callbacks *callbacks, PDS_scalar_callback scalar);
/**
 * Set error callback.
 * @param [in,out] callbacks Parser callbacks.
 * @param [in]     error     Error callback.
 */
void PDS_set_error_callback(PDS_callbacks *callbacks, PDS_error_callback error);
/**
 * Parse the PDS data contained in the input buffer.
 * @param [in] callbacks Parser callbacks.
 * @param [in] buffer    Input buffer.
 * @param [in] len       Length of the input buffer.
 * @param [in] user_data User data.
 * @return 1 if the PDS data contained in the input buffer was successfully parsed, or 0 if an error occured.
 */
int PDS_parse(PDS_callbacks *callbacks, const char *buffer, int len, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* TINY_PDS_H */

/*****************************************************************************/
#ifdef TINY_PDS_IMPL

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
#if 0
static int PDS_tolower(char c)
{
    return PDS_isupper(c) ? ('a' + ((c)-'A')) : (c);
}

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
    PDS_TOKEN_END,
};
/**
 * PDS token flag.
 */
enum PDS_TOKEN_FLAG
{
    PDS_TOKEN_FLAG_NONE = 0,
    PDS_TOKEN_FLAG_BEGIN,
    PDS_TOKEN_FLAG_END
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
    /** Token type @see PDS_TOKEN_TYPE **/
    int type;
    /** Flag (begin/end, ...). **/
    int flag;
} PDS_token;
/** PDS parser. **/
typedef struct
{
    /** Current status @see PDS_STATUS **/
    int status;
    /** Pointer to the first character of the input text. **/
    const char *first;
    /** Pointer to the last character of the input text. **/
    const char *last;
    /** Pointer to the current character. **/
    const char *current;
    /** Pointer to the beginning of the current line. **/
    const char *line;
    /** Number of the line being parsed.**/
    int line_num;
    /** User data. **/
    void *user_data;
    /** Current token. **/
    PDS_token token;
    /** Current scalar value. **/
    PDS_scalar scalar;
    /** Callbacks. **/
    PDS_callbacks callbacks;
} PDS_parser;

/**
 * Set error and run error display callback.
 * @param [in,out] parser Parser.
 * @param [in]     error Error status @see PDS_STATUS
 * @param [in]     message Error message.
 */
static void PDS_error(PDS_parser *parser, int error, const char *message)
{
    parser->status = error;
    if(0 != parser->callbacks.error)
    {
        parser->callbacks.error(parser->line_num, message, parser->user_data);
    }
}
/**
 * Skip whitespaces and comments from string.
 * A white space is either a space (' '), form-feed ('\\f'), newline ('\\n'),
 * carriage return ('\\r'), horizontal tab ('\\t') or vertical tab ('\\v').
 * Comments are C-like comments, but unlike them they must fit on a single line.
 * Nested comments are forbidden.
 * @param [in,out] parser PDS parser context.
 * @return 0 if an error occured, 1 upon success.
 */
static int PDS_skip_whitespaces(PDS_parser *parser) 
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
                ++parser->line_num;
                parser->line = parser->current+1;
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
static int64_t PDS_parse_int_base(const char *first, const char *last, const char **end, int base, int *status)
{
    const char *ptr;
    int64_t current;
    int64_t accumulator;
    int64_t cutoff;
    int64_t cutlimit;
    int any;
    int neg = 0;
    /* Sanity check. */
    if(PDS_OK != *status)
    {
        return 0;
    }
    if((base<2) || (base>16))
    {
        *status = PDS_INVALID_VALUE;
        return INT64_MAX;
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
    cutoff   = INT64_MAX; 
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
        return neg ? INT64_MIN : INT64_MAX; 
    }
    else
    {
        *status = any ? PDS_OK : PDS_INVALID_VALUE;
        return (int64_t)(neg ? -accumulator : accumulator);
    }
}
/**
 * Parse an integer number.
 * The number can be expressed in decimal or based notation.
 * A based integer is represented by a decimal number representing the base
 * followed by a number expressed in the specified base enclosed between '#'
 * characters.
 * @param [in,out] parser Parser.
 * @return 1 if the string contains a valid integer, 0 otherwise.
 */
static int PDS_parse_int(PDS_parser *parser)
{
    const char *ptr = 0;
    int64_t value;
    /* Sanity check. */
    if(PDS_OK != parser->status)
    {
        return 0;
    }
    /* Parse value. */
    value = PDS_parse_int_base(parser->current, parser->last, &ptr, 10, &parser->status);
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
        value = PDS_parse_int_base(current, parser->last, &ptr, (int)value, &parser->status);
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
 * @param [in,out] parser Parser.
 * @return 1 if a floating point value was successfully parser, or 0 if an error occured. 
 */
static int PDS_parse_real(PDS_parser *parser) 
{
    double value;
    int neg = 0;
    int div;
    int64_t integer;
    const char *ptr = 0;
    /* Integer part (can be negative). */   
    integer = PDS_parse_int_base(parser->current, parser->last, &ptr, 10, &parser->status);
    /* The integer part can be empty (ex: .03). */
    if((0 == integer) || (ptr == parser->current))
    {
        integer = 0;
        parser->status = PDS_OK;
        if('-' == *parser->current)
        {
            neg = 1;
            if(ptr == parser->current)
            { ++ptr; }
        }
        else if(('+' == *parser->current) && (ptr == parser->current))
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
        double decimal = 0.0;
        ++ptr;
        for(div=10; (ptr<=parser->last) && PDS_isdigit(*ptr); ptr++, div*=10)
        {
            integer = *ptr - '0';
            if(integer)
            {
                decimal += integer/(double)div;
            }
        }
        value += neg ? -decimal : decimal;
    }
    /* Check for exponent. */
    if(('e' == *ptr) || ('E' == *ptr))
    {
        int64_t i;
        int64_t n;
        n = PDS_parse_int_base(ptr+1, parser->last, &ptr, 10, &parser->status);
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
            int exponent;
            for(i=0, exponent=1; i<n; ++i, exponent*=10)
            {}
            value *= (double)exponent;
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
 * @param [in,out] status Status variable set to PDS_OK if the string contains
 *                        a valid identifier or PDS_INVALID_VALUE.
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
 * @param [in,out] parser Parser.
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
                    (void)PDS_parse_int_base(first+1, parser->last, &next, 10, &parser->status);
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
    parser->scalar.integer.unit.last = first-1;
    parser->current = first+1;
    return 1;
}
/**
 * Parse a literal symbol.
 * If the parsing was succesfull, the content of the literal value can be retreived from first+1 to 
 * end-1(excluded). 
 * @param [in,out] parser Parser.
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
    parser->scalar.symbolic.last = first-1;
    parser->scalar.type = PDS_SYMBOLIC_VALUE;
    
    parser->current = first+1;
    
    return 1;
}
/**
 * Parse a quoted string.
 * If the parsing was succesfull, the content of the quoted text can be retreived from first+1 to 
 * end-1(excluded). 
 * @param [in,out] parser Parser.
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
        if('\n' == *first)
        {
            ++parser->line_num;
            parser->line = first+1;
        } 
    }
    
    if((first>last) || ('"' != *first))
    {
        PDS_error(parser, PDS_INVALID_VALUE, "missing string delimiter");
        return 0;
    }
    
    parser->scalar.type = PDS_TEXT_STRING_VALUE;
    parser->scalar.text.last  = first-1;
 
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
 * @param [in,out] status Status variable set to PDS_OK if the string contains
 *                        a valid date or PDS_INVALID_VALUE.
 * @return 1 if the string contains a valid date, 0 if the string is invalid.
 */
static int PDS_parse_date(const char *first, const char *last, const char **end, PDS_datetime *date, int *status)
{
    int64_t value;
    const char *next = 0;
    *end = first;
    if(PDS_OK != *status)
    {
        return 0;
    }
    /* Year */
    value = PDS_parse_int_base(first, last, &next, 10, status);
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
    value = PDS_parse_int_base(first, last, &next, 10, status);
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
            value = PDS_parse_int_base(next+1, last, &next, 10, status);
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
 * @param [in,out] status Status variable set to PDS_OK if the string contains
 *                        a valid date or PDS_INVALID_VALUE.
 * @return 1 if the string contains a valid date, 0 if the string is invalid.
 */
static int PDS_parse_time(const char *first, const char *last, const char **end, PDS_datetime *date, int *status)
{
    int64_t value = 0;
    const char *next = 0;

    *end = first;

    if(PDS_OK != *status)
    {
        return 0;
    }

    /* Hour. */
    value = PDS_parse_int_base(first, last, &next, 10, status);
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
    value = PDS_parse_int_base(next+1, last, &next, 10, status);
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
        value = PDS_parse_int_base(first, last, &next, 10, status);
        if('.' == *next)
        {       
            int64_t tmp, i;
            first = next+1;
            *status = PDS_OK;
            tmp = PDS_parse_int_base(first, last, &next, 10, status);
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
        value = PDS_parse_int_base(next, last, &next, 10, status);
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
            value = PDS_parse_int_base(next+1, last, &next, 10, status);
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
 * A date is either a date, a time or a combination of date and time string.
 * @param [in,out] parser Parser.
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
    
    ret = PDS_parse_date(parser->current, parser->last, &next, date, &parser->status);  
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
    ret = PDS_parse_time(next, parser->last, &next, date, &parser->status);
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
 * Parse attribute name.
 * A attribute name is an identifier or the concatenation of a namespace and an identifier separated by ':'.
 * @param [in] first First character of the input string.
 * @param [in] last Last character of the input string.
 * @param [out] end If not null stores the pointer to the first invalid 
 *                  character of the input string.
 * @param [in,out] status Status variable set to PDS_OK if an integer was
 *                        successfully parsed. If an error occured it is set to 
 *                        PDS_INVALID_VALUE or PDS_INVALID_RANGE.
 * @return Pointer to the beginning of the attribute name, 0 otherwise.
 */
static const char* PDS_parse_lhs_attribute(const char *first, const char *last, const char **end, int *status)
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
 * lhs_token    := attribute_id | group | object | pointer
 * attribute_id := identifier | namespace:identifier
 * namespace    := identifier
 * group        := 'begin_group' | 'end_group'
 * object       := 'begin_object' | 'end_object'
 * pointer      := ^identifier
 * @param [in,out] parser Parser.
 * @return 1 if the string contains a valid token, 0 otherwise.
 */
static int PDS_parse_lhs(PDS_parser *parser)
{
    static const char name_buffer[] = "end_groupend_object";

    static const struct
    {
        off_t first;
        off_t last;
        int   type;
        int   flag;
    } lhs_parser[] =
    {
        { 4, 8, PDS_TOKEN_GROUP,  PDS_TOKEN_FLAG_BEGIN }, /* group      */
        { 0, 8, PDS_TOKEN_GROUP,  PDS_TOKEN_FLAG_END   }, /* end_group  */
        {13,18, PDS_TOKEN_OBJECT, PDS_TOKEN_FLAG_BEGIN }, /* object     */
        { 9,18, PDS_TOKEN_OBJECT, PDS_TOKEN_FLAG_END   }, /* end_object */
        { 0, 2, PDS_TOKEN_END,    PDS_TOKEN_FLAG_NONE  }, /* end        */
    };
    size_t i;
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
        lhs->flag = PDS_TOKEN_FLAG_NONE;
        return 1;
    }
    /* 2. group/object/end */
    for(lhs->last=lhs->first; (lhs->last<=parser->last) && !(PDS_isspace(*lhs->last) || ('/'==*lhs->last) || ('='==*lhs->last)); lhs->last++)
    {}
    lhs->last--;
    for(i=0; i<PDS_ARRAY_SIZE(lhs_parser); i++)
    {
        if(PDS_string_case_compare(lhs->first, lhs->last, name_buffer+lhs_parser[i].first, name_buffer+lhs_parser[i].last))
        {
            parser->current = lhs->last+1;
            lhs->type = lhs_parser[i].type; 
            lhs->flag = lhs_parser[i].flag;
            return 1;
        }
    }
    /* 3. attribute */
    lhs->first = PDS_parse_lhs_attribute(parser->current, parser->last, &parser->current, &parser->status);
    if(PDS_OK == parser->status)
    {
        lhs->last = parser->current-1;
        lhs->type = PDS_TOKEN_ATTRIBUTE;
        lhs->flag = PDS_TOKEN_FLAG_NONE;
        return 1;
    }

    /* No valid token found. */ 
    PDS_error(parser, PDS_INVALID_VALUE, "no valid token found");
    return 0;
}
/**
 * Parse a scalar value.
 * A scalar value can be a string, a literal symbol, a date/time, an integer or a floating point value.
 * @param [in,out] parser Parser.
 * @return 1 if the string contains a valid scalar value, 0 otherwise.
 */
static int PDS_parse_scalar_value(PDS_parser *parser)
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
                for(; (ptr <= parser->last) && PDS_isdigit(*ptr); ptr++)
                {}
                if(PDS_isdigit(c) && (('-' == *ptr) || (':' == *ptr)))
                {
                    ret = PDS_parse_datetime(parser);
                }
                else
                {
                    if(  (('.' == c   ) || ('e' == c   )) 
                       ||(('.' == *ptr) || ('e' == *ptr)) )
                    {
                        ret = PDS_parse_real(parser);
                    }
                    else
                    {
                        ret = PDS_parse_int(parser);
                    }
                    if(ret)
                    {
                        ret = PDS_skip_whitespaces(parser);
                        if(ret && ('<' == *parser->current))
                        {
                            ret = PDS_parse_unit(parser);
                        }
                    }
                }
            }
            else
            {
                PDS_error(parser, PDS_INVALID_VALUE, "invalid scalar value");

            }
            break;
    }
    return ret;
}
/**
 * Parse a value set.
 * set.begin, set.end callbacks are called at the beginning and end of each sets.
 * set.element is called for each set element.
 * @param [in,out] parser Parser.s
 * @return 1 if a set was successfully parsed, 0 otherwise.
 */
static int PDS_parse_set(PDS_parser *parser)
{
    int ret;
    if(PDS_OK != parser->status)
    {
        return 0;
    }

    if('{' != *parser->current++)
    {
        PDS_error(parser, PDS_INVALID_VALUE, "missing set separator");
        return 0;
    }
    if(0 != parser->callbacks.set.begin)
    {
        if(0 == parser->callbacks.set.begin(parser->user_data))
        {
            return 0;
        }
    }
    
    ret = PDS_skip_whitespaces(parser);
    if('}' == *parser->current)
    {
        parser->current++;
        PDS_error(parser, PDS_INVALID_VALUE, "empty set");
        return 0;
    }

    while(ret && (parser->current<=parser->last))
    {
        ret = PDS_skip_whitespaces(parser);
        if(ret)
        {
            ret = PDS_parse_scalar_value(parser);
            if(ret)
            {
                if(0 != parser->callbacks.set.element)
                {
                    if(0 == parser->callbacks.set.element(&parser->scalar, parser->user_data))
                    {
                        return 0;
                    }
                }
                ret = PDS_skip_whitespaces(parser);
                if(ret)
                {
                    char c = *parser->current++;
                    if('}' == c)
                    {
                        if(0 != parser->callbacks.set.end)
                        {
                            if(0 == parser->callbacks.set.end(parser->user_data))
                            {
                                return 0;
                            }
                        }
                        return 1;
                    }
                    else if(',' != c)
                    {
                        PDS_error(parser, PDS_INVALID_VALUE, "invalid element separator");
                        return 0;
                    }
                }
            }
        }
    }
    if(parser->current > parser->last)
    {
        PDS_error(parser, PDS_INVALID_VALUE, "missing separator");
        ret = 0;
    }
    return ret;
}
/**
 * Parse a multidimensional sequence.
 * sequence.begin, sequence.end callbacks are called at the beginning and end of each sequences.
 * sequence.element is called for each sequence element.
 * @param [in,out] parser Parser.
 * @return 1 if a multidimensional sequence was succesfully parsed, 0 otherwise.
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
    if(0 != parser->callbacks.sequence.begin)
    {
        if(0 == parser->callbacks.sequence.begin(parser->user_data))
        {
            return 0;
        }   
    }
    while(ret && (depth > 0) && (parser->current <= parser->last))
    {
        ret = PDS_skip_whitespaces(parser);
        if(ret)
        {
            if('(' == *parser->current)
            {
                ++parser->current;
                ++depth;
                if(0 != parser->callbacks.sequence.begin)
                {
                    if(0 == parser->callbacks.sequence.begin(parser->user_data))
                    {
                        return 0;
                    }
                }
            }
            else
            {
                ret = PDS_parse_scalar_value(parser);
                if(ret)
                {
                    if(0 != parser->callbacks.sequence.element)
                    {
                        if(0 == parser->callbacks.sequence.element(&parser->scalar, parser->user_data))
                        {
                            return 0;
                        }
                    }
                    ret = PDS_skip_whitespaces(parser);
                    if(ret)
                    {
                        char c;
                        while(((c = *parser->current++) == ')') && (depth > 0))
                        {
                            --depth;
                            if(0 != parser->callbacks.sequence.end)
                            {
                                if(0 == parser->callbacks.sequence.end(parser->user_data))
                                {
                                    return 0;
                                }
                            }
                            if(depth)
                            {
                                ret = PDS_skip_whitespaces(parser);
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
 * Parse right hand side (rhs) value.
 * A rhs value can be a sequence, a set or a single scalar value.
 * @param [in,out] parser Parser.
 * @return 1 upon success, 0 if an error occured.
 */
static int PDS_parse_rhs(PDS_parser *parser)
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
            ret = PDS_parse_scalar_value(parser);
            if(ret)
            {
                if(0 != parser->callbacks.scalar)
                {
                    ret = parser->callbacks.scalar(&parser->scalar, parser->user_data);
                }
            }
            break;
    }
    return ret;
}
/**
 * Parse a single statement.
 * @param [in,out] parser Parser.
 * @return 1 if a valid statement was found, 0 otherwise.
 */
static int PDS_parse_statement(PDS_parser *parser)
{
    int ret;
    int line;
    if(PDS_OK != parser->status)
    {
        return 0;
    }

    ret = PDS_skip_whitespaces(parser);
    if(!ret)
    {
        return 0;
    }

    ret = PDS_parse_lhs(parser);
    if(!ret)
    {
        PDS_error(parser, parser->status, "invalid left-hand value");
        return 0;
    }   
    if(PDS_TOKEN_END == parser->token.type)
    {
        line = parser->line_num;
        if(!PDS_skip_whitespaces(parser))
        {
            return 0;
        }
        /* Check for new line. */
        if(parser->line_num <= line)
        {
            PDS_error(parser, PDS_INVALID_VALUE, "no newline at the end of statement");
            return 0;   
        }
        parser->status = PDS_END;
        return 1;
    }
    
    ret = PDS_skip_whitespaces(parser);
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

    ret = PDS_skip_whitespaces(parser);
    if(!ret)
    {
        return 0;
    }
    
    line = parser->line_num;
    
    switch(parser->token.type)
    {
        case PDS_TOKEN_ATTRIBUTE:
        case PDS_TOKEN_POINTER:
            {
                PDS_begin_callback begin;
                PDS_end_callback   end;
                if(PDS_TOKEN_ATTRIBUTE == parser->token.type)
                {
                    begin = parser->callbacks.attribute.begin;
                    end   = parser->callbacks.attribute.end;
                }
                else
                {
                    begin = parser->callbacks.pointer.begin;
                    end   = parser->callbacks.pointer.end;
                }
                if(0 != begin)
                {
                    if(!begin(parser->token.first, parser->token.last, parser->user_data))
                    {
                        return 0;
                    }
                }
                if(0 == PDS_parse_rhs(parser))
                {
                    PDS_error(parser, parser->status, "invalid right value");
                    return 0;
                }
                if(0 != end)
                {
                    if( !end(parser->token.first, parser->token.last, parser->user_data) )
                    {
                        return 0;
                    }
                }
            }
            break;
        case PDS_TOKEN_GROUP:
        case PDS_TOKEN_OBJECT:
            {
                int (*callback)(const char*, const char*, void*);   
                parser->scalar.identifier.first = PDS_parse_identifier(parser->current, parser->last, &parser->current, &parser->status);
                if(PDS_OK != parser->status)
                {
                    PDS_error(parser, PDS_INVALID_VALUE, "invalid group identifier");
                    return 0;
                }
                parser->scalar.identifier.last = parser->current-1;
                parser->scalar.type = PDS_IDENTIFIER_VALUE;
                if(PDS_TOKEN_GROUP == parser->token.type)
                {
                    callback = (PDS_TOKEN_FLAG_BEGIN == parser->token.flag) ? parser->callbacks.group.begin : parser->callbacks.group.end;
                }
                else
                {
                    callback = (PDS_TOKEN_FLAG_BEGIN == parser->token.flag) ? parser->callbacks.object.begin : parser->callbacks.object.end;
                }
                if(0 != callback)
                {
                    ret = callback(parser->scalar.identifier.first, parser->scalar.identifier.last, parser->user_data);
                    if(!ret)
                    {
                        return 0;
                    }
                }
            }
            break;
        default:
            PDS_error(parser, PDS_INVALID_VALUE, "unknown token type");
            return 0;
    }

    if(!PDS_skip_whitespaces(parser))
    {
        return 0;
    }
    /* Check for new line. */
    if(parser->line_num <= line)
    {
        PDS_error(parser, PDS_INVALID_VALUE, "no newline at the end of statement");
        return 0;   
    }
    return 1;
}
/*
 * Parse the PDS data contained in the input buffer.
 */
int PDS_parse(PDS_callbacks *callbacks, const char *buffer, int len, void *user_data)
{
    const char* pds_version_name_first = "PDS_VERSION_ID";
    const char* pds_version_name_last  = &pds_version_name_first[13];
    const char* pds_version_id_first = "PDS3";
    const char* pds_version_id_last  = &pds_version_id_first[3];

    PDS_parser parser;

    int ret;
    
    /* Sanity check. */
    if((0 == buffer) || (0 == len))
    {
        return 0;
    }
    memcpy(&parser.callbacks, callbacks, sizeof(PDS_callbacks));
    parser.line = parser.current = parser.first = buffer;
    parser.last = buffer + len;
    parser.line_num = 1;
    parser.user_data = user_data;
    parser.status = PDS_OK;

    /* The first statement must be the PDS_VERSION_ID attribute. */
    if(!PDS_parse_statement(&parser))
    {
        return 0;
    }
    if(!PDS_string_case_compare(parser.token.first, parser.token.last, pds_version_name_first, pds_version_name_last))
    {
        PDS_error(&parser, PDS_INVALID_VALUE, "a PDS file must start with PDS_VERSION_ID");
        return 0;
    }
    /* Check version. */
    if(PDS_IDENTIFIER_VALUE != parser.scalar.type)
    {
        PDS_error(&parser, PDS_INVALID_VALUE, "invalid value type for PDS_VERSION_ID");
        return 0;
    }
    if(!PDS_string_case_compare(parser.scalar.identifier.first, parser.scalar.identifier.last, pds_version_id_first, pds_version_id_last))
    {
        PDS_error(&parser, PDS_INVALID_VALUE, "invalid PDS version id");
        return 0;
    }   
    /* Parse the remaining. */  
    for(ret=1; ret && (PDS_OK == parser.status); )
    {
        ret = PDS_parse_statement(&parser);
    }   
    return ret;
}
/* Set attribute callback. */
void PDS_set_attribute_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                           PDS_end_callback   end)
{
    callbacks->attribute.begin = begin;
    callbacks->attribute.end   = end;
}
/* Set pointer callback. */
void PDS_set_pointer_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                         PDS_end_callback   end)
{
    callbacks->pointer.begin = begin;
    callbacks->pointer.end   = end;
}
/* Set group callbacks. */
void PDS_set_group_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                       PDS_end_callback   end)
{
    callbacks->group.begin = begin;
    callbacks->group.end = end;
}
/* Set object callbacks. */
void PDS_set_object_callbacks(PDS_callbacks *callbacks, PDS_begin_callback begin, 
                                                        PDS_end_callback   end)
{
    callbacks->object.begin = begin;
    callbacks->object.end = end;
}
/* Set 'set' callbacks. */
void PDS_set_set_callbacks(PDS_callbacks *callbacks, PDS_collection_begin_callback begin, 
                                                     PDS_scalar_callback           element,
                                                     PDS_collection_end_callback   end)
{
    callbacks->set.begin   = begin;
    callbacks->set.element = element;
    callbacks->set.end     = end;
}
/* Set sequence callbacks. */
void PDS_set_sequence_callbacks(PDS_callbacks *callbacks, PDS_collection_begin_callback begin, 
                                                          PDS_scalar_callback           element,
                                                          PDS_collection_end_callback   end)
{
    callbacks->sequence.begin   = begin;
    callbacks->sequence.element = element;
    callbacks->sequence.end     = end;
}
/* Set scalar callback. */
void PDS_set_scalar_callback(PDS_callbacks *callbacks, PDS_scalar_callback scalar)
{
    callbacks->scalar = scalar;
}
/* Set error callback. */
void PDS_set_error_callback(PDS_callbacks *callbacks, PDS_error_callback error)
{
    callbacks->error = error;
}
#endif /* TINY_PDS_IMPL */
