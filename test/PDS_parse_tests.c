#include <munit.h>

#define TINY_PDS_IMPL
#include <tinypds.h>

void dummy_error(const PDS_error_description *desc, void *unused) {
    (void)unused;
    munit_logf(MUNIT_LOG_INFO, "line %d:%d: %s", desc->number, desc->position, desc->msg);
}

int compare_scalar(const PDS_scalar *s0, const PDS_scalar *s1) {
    if(s0->type != s1->type) {
        return 0;
    }
    switch(s0->type) {
        case PDS_INTEGER_VALUE:
            if(s0->integer.value == s1->integer.value) {
                if(    (s0->integer.unit.first && s1->integer.unit.first)
                    && (s0->integer.unit.last  && s1->integer.unit.last ) ) {
                    return PDS_string_compare(s0->integer.unit.first, s0->integer.unit.last, 
                                              s1->integer.unit.first, s1->integer.unit.last);
                }
                else if(    (0 == s0->integer.unit.first) && (0 == s1->integer.unit.first)
                         && (0 == s1->integer.unit.first) && (0 == s1->integer.unit.last) ) {
                    return 1;
                } 
            }
            return 0;
        case PDS_REAL_VALUE:
            if(s0->real.value == s1->real.value) {
                if(    (s0->real.unit.first && s1->real.unit.first)
                    && (s0->real.unit.last  && s1->real.unit.last ) ) {
                    return PDS_string_compare(s0->real.unit.first, s0->real.unit.last, 
                                              s1->real.unit.first, s1->real.unit.last);
                }
                else if(    (0 == s0->real.unit.first) && (0 == s1->real.unit.first)
                         && (0 == s1->real.unit.first) && (0 == s1->real.unit.last) ) {
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

typedef struct {
    int depth;
    int count;
    const char *name;
    const PDS_scalar *scalar;
} expected_t;

typedef struct {
    int depth;
    int index;
    const expected_t *expected;
} state_t;

int sequence_begin_callback(void *user_data)
{
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;   
    state->depth++;
    if(state->depth > expected->depth) {
        munit_log(MUNIT_LOG_INFO, "invalid sequence dimension");
        return 0;
    }
    return 1;
}

int sequence_element_callback(const PDS_scalar *scalar, void *user_data)
{
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;
    if(state->index >= expected->count)  {
        munit_log(MUNIT_LOG_INFO, "too many scalars");
        return 0;
    }
    if(!compare_scalar(scalar, &(expected->scalar[state->index])))  {
        munit_log(MUNIT_LOG_INFO, "scalar value mismatch");
        return 0;
    }
    state->index++;
    return 1;
}

int sequence_end_callback(void *user_data) {
    state_t *state = (state_t*)user_data;
    state->depth--;
    if(state->depth < 0) {
        munit_log(MUNIT_LOG_INFO, "invalid sequence dimension");
        return 0;
    }
    return 1;
}

int set_begin_callback(void *user_data) {
    (void)user_data;
    return 1;
}

int set_element_callback(const PDS_scalar *scalar, void *user_data) {
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;
    if(state->index >= expected->count) {
        munit_log(MUNIT_LOG_INFO, "too many scalars");
        return 0;
    }
    if(!compare_scalar(scalar, &(expected->scalar[state->index]))) {
        munit_log(MUNIT_LOG_INFO, "scalar value mismatch");
        return 0;
    }
    state->index++;
    return 1;
}

int set_end_callback(void *user_data) {
    (void)user_data;
    return 1;
}

int attribute_begin_callback(const char *first, const char *last, void *user_data) {
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;
    if(!PDS_string_compare(first, last, expected->name, expected->name+strlen(expected->name)-1)) {
        munit_log(MUNIT_LOG_INFO, "attribute name mismatch");
        return 0;
    }
    return 1;
}

int attribute_end_callback(const char *first, const char *last, void *user_data) {
    (void)first;
    (void)last;
    (void)user_data;
    return 1;
}

int pointer_begin_callback(const char *first, const char *last, void *user_data) {
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;
    if(!PDS_string_compare(first, last, expected->name, expected->name+strlen(expected->name)-1))
    {
        munit_log(MUNIT_LOG_INFO, "pointer name mismatch");
        return 0;
    }
    return 1;
}

int pointer_end_callback(const char *first, const char *last, void *user_data) {
    (void)first;
    (void)last;
    (void)user_data;
    return 1;
}

int scalar_callback(const PDS_scalar *scalar, void *user_data) {
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;
    if(!compare_scalar(scalar, expected->scalar)) {
        munit_log(MUNIT_LOG_INFO, "scalar value mismatch");
        return 0;
    }
    return 1;
}

int declaration_check_name(const char *what, const char *first, const char *last, void *user_data) {
    state_t *state = (state_t*)user_data;
    const expected_t *expected = state->expected;
    const PDS_scalar *scalar;   
    int i = state->index++;
    if(i >= expected->count)
    {
        munit_log(MUNIT_LOG_INFO, "too many elements");
        return 0;
    }
    scalar = &expected->scalar[i];
    if(!PDS_string_compare(scalar->identifier.first, scalar->identifier.last, first, last))
    {
        munit_logf(MUNIT_LOG_INFO,  "%s name mismatch", what);
        return 0;
    }
    return 1;
}

static int declaration_begin(const char *what, const char *first, const char *last, void *user_data) {
    if(!declaration_check_name(what, first, last, user_data)) {
        return 0;
    }
    return 1;
}

static int declaration_end(const char *what, const char *first, const char *last, void *user_data) {
    if(!declaration_check_name(what, first, last, user_data)) {
        return 0;
    }
    return 1;
}

int group_begin_callback(const char *first, const char *last, void *user_data) {
    return declaration_begin("group", first, last, user_data);
}

int group_end_callback(const char *first, const char *last, void *user_data) {
    return declaration_end("group", first, last, user_data);
}

int object_begin_callback(const char *first, const char *last, void *user_data) {
    return declaration_begin("object", first, last, user_data);
}

int object_end_callback(const char *first, const char *last, void *user_data) {
    return declaration_end("object", first, last, user_data);
}

MunitResult parse_statement_test(const MunitParameter params[], void* fixture) {
    const char *degrees = "DEGREES";
    const char *symbolic = "dummy";
    const char *group = "dummy_group_0";
    const char *object = "IMAGE";

    const PDS_scalar data[] = {
        {   .date_time.type = PDS_DATE_TIME_VALUE,
            .date_time.year  = 2016,
            .date_time.day   = 3,
            .date_time.month = 1,
            .date_time.hour   = 18,
            .date_time.minute = 21,
            .date_time.second = 49,
            .date_time.microsecond = 129900,
            .date_time.hour_offset   =  2,
            .date_time.minute_offset = 10,
            .date_time.time_type = PDS_ZONED_TIME
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.unit.first = degrees,
            .real.unit.last  = degrees + strlen(degrees)-1, 
            .real.value = 12.0
        },
        {   .symbolic.type  = PDS_SYMBOLIC_VALUE,
            .symbolic.first = symbolic,
            .symbolic.last  = symbolic + strlen(symbolic)-1
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.unit.first = 0,
            .integer.unit.last  = 0,
            .integer.value      = 327128
        },
        {   .identifier.type  = PDS_IDENTIFIER_VALUE,
            .identifier.first = group,
            .identifier.last  = group + strlen(group)-1
        },
        {   .identifier.type  = PDS_IDENTIFIER_VALUE,
            .identifier.first = object,
            .identifier.last  = object + strlen(object)-1
        }
    };
    const expected_t expected[] = {
        {   .name   = "LOCAL_NAMESPACE:DATE_TIME_ATTR",
            .scalar = &data[0],
            .count  = 1
        },
        {   .name   = "ELEVATION_FOV", 
            .scalar = &data[1],
            .count  = 1
        },
        {   .name   = "DUMMY:SYMBOLIC_LITERAL", 
            .scalar = &data[2],
            .count  = 1
        },
        {   .name   = "IMAGE",
            .scalar = &data[3],
            .count  = 1
        },
        {
            .name   = "group",
            .scalar = &data[4],
            .count  = 1
        },
        {
            .name   = "object",
            .scalar = &data[5],
            .count  = 1
        }
    };

    struct {
        const char *str;
        int end;
        const expected_t *expected;
        int status;
    } tv[] = {
        { "LOCAL_NAMESPACE:DATE_TIME_ATTR = 2016-01-3T18:21:49.1299+2:10 \r\n", 64, &expected[0], PDS_OK },
        { "ELEVATION_FOV                  = 12.000 <DEGREES>\r\n", 51, &expected[1], PDS_OK },
        { "DUMMY:SYMBOLIC_LITERAL = 'dummy' \r\n", 35, &expected[2], PDS_OK },
        { "/* comment line #0 */\r\n/* comment line #1 */\r\n^IMAGE = \t 327128    \r\n", 69, &expected[3], PDS_OK },
        { " \r\n        group\t = dummy_group_0\r\n", 35, &expected[4], PDS_OK },
        { "end_group=dummy_group_0\t    \r\n", 30, &expected[4], PDS_OK },
        { "OBJECT = IMAGE\r\n", 16, &expected[5], PDS_OK },
        { "END_OBJECT = IMAGE\r\n", 20, &expected[5], PDS_OK }
    };

    PDS_parser parser;
    state_t state;
    
    (void)params;
    (void)fixture;
    
    memset(&state, 0, sizeof(state_t));

    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);
    PDS_set_scalar_callback(&parser.callbacks, scalar_callback);
    PDS_set_attribute_callbacks(&parser.callbacks, attribute_begin_callback, attribute_end_callback);
    PDS_set_pointer_callbacks(&parser.callbacks, pointer_begin_callback, pointer_end_callback);
    PDS_set_group_callbacks(&parser.callbacks, group_begin_callback, group_end_callback);
    PDS_set_object_callbacks(&parser.callbacks, object_begin_callback, object_end_callback);

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        state.index = 0;
        state.expected = tv[i].expected;

        parser.line_num  = 1;
        parser.first     = tv[i].str;
        parser.last      = parser.first + strlen(parser.first) - 1;
        parser.current   = parser.first;
        parser.status    = PDS_OK;
        parser.user_data = &state;

        int ret = PDS_parse_statement(&parser);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        }
    }
    return MUNIT_OK;
}

MunitResult parse_set_test(const MunitParameter params[], void* fixture) {
    const PDS_scalar data[] =  {
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 123.8,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 90.0,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 0.125,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 0.02,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 1,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 2,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 3,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 4,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        }
    };

    const expected_t set[] =  {
        {   .count  = 4,
            .name   = "dummy",
            .scalar = &data[0]
        },
        {   .count  = 6,
            .name   = "failure",
            .scalar = &data[4]
        }
    };
    
    struct {
        const char *str;
        int end;
        const expected_t *expected;
        int status;
    } tv[] = {
        { "dummy = { 123.8, 90.0, 0.125, 0.02 }\r\n", 38, &set[0], PDS_OK },
        { "failure = {\r\n", 0, &set[1], PDS_INVALID_VALUE },
        { "failure = {}\r\n", 0, &set[1], PDS_INVALID_VALUE },
        { "failure = { \t, }\r\n", 0, &set[1], PDS_INVALID_VALUE },
        { "failure = {{}}\r\n", 0, &set[1], PDS_INVALID_VALUE },
        { "failure = {1,,3}\r\n", 0, &set[1], PDS_INVALID_VALUE },
        { "failure = {{1,2,3,4,5,6}}\r\n", 0, &set[1], PDS_INVALID_VALUE }
    };
    
    (void)params;
    (void)fixture;
    
    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);
    PDS_set_set_callbacks(&parser.callbacks, set_begin_callback, set_end_callback);
    PDS_set_scalar_callback(&parser.callbacks, set_element_callback);
    
    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        state_t state = { .index = 0, .expected = tv[i].expected };

        parser.line_num  = 1;
        parser.first     = tv[i].str;
        parser.last      = parser.first + strlen(parser.first) - 1;
        parser.current   = parser.first;
        parser.status    = PDS_OK;
        parser.user_data = &state;
        
        int ret = PDS_parse_statement(&parser);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        }
    }
    return MUNIT_OK;
}

MunitResult parse_sequence_test(const MunitParameter params[], void* fixture) {
    const char *unit_first = "Km";
    const char *unit_last  = unit_first + strlen(unit_first) - 1;

    const PDS_scalar sequence_1D[] = 
    {
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 51,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 8,
            .integer.unit.first = unit_first,
            .integer.unit.last  = unit_last
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 117,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 9.2,
            .real.unit.first = unit_first,
            .real.unit.last  = unit_last
        },
    };

    const PDS_scalar sequence_2D[] =
    {
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 4,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {
            .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 2048,
            .integer.unit.first = 0,
            .integer.unit.last  = 0 
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 0.6,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 14,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {
            .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 1024,
            .integer.unit.first = 0,
            .integer.unit.last  = 0 
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 1.2,
            .real.unit.first = 0,
            .real.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 24,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {
            .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 256,
            .integer.unit.first = 0,
            .integer.unit.last  = 0 
        },
        {   .real.type  = PDS_REAL_VALUE,
            .real.value = 2.4,
            .real.unit.first = 0,
            .real.unit.last  = 0
        }
    };

    const PDS_scalar failure[] = 
    {
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 1,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 2,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 3,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 4,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 5,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        },
        {   .integer.type  = PDS_INTEGER_VALUE,
            .integer.value = 6,
            .integer.unit.first = 0,
            .integer.unit.last  = 0
        }
    };

    const expected_t sequences[] = 
    {
        {   .depth  = 1,
            .count  = 4,
            .name   = "dummy",
            .scalar = sequence_1D
        },
        {
            .depth  = 2,
            .count  = 9,
            .name   = "matrix",
            .scalar = sequence_2D
        },
        {
            .depth  = 2,
            .count  = 6,
            .name   = "failure",
            .scalar = failure
        }
    };
    
    struct {
        const char *str;
        int end;
        const expected_t *expected;
        int status;
    } tv[] = {
        { "   dummy  = \t( 51, 8 <Km>, \t117, 9.2 <Km>)\r\n", 44, &sequences[0], PDS_OK },
        { "matrix = (\r\n\t(4,2048,.6),\r\n\t(14,1024,1.2),\r\n\t(24,256,2.4) )\r\n", 61, &sequences[1], PDS_OK },
        { "failure = (\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = ()\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = (())\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = ((1,2)\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = (1,,3),\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = ((1,2,3,),(4,5,6))\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = ((1,2,3),(4,5,6)\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = (1,2,3,(4,5,6))\r\n", 0, &sequences[2], PDS_INVALID_VALUE },
        { "failure = ((1,2),3,4,5,6)\r\n", 0, &sequences[2], PDS_INVALID_VALUE }
    };

    (void)params;
    (void)fixture;

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);
    PDS_set_sequence_callbacks(&parser.callbacks, sequence_begin_callback, sequence_end_callback);
    PDS_set_scalar_callback(&parser.callbacks, sequence_element_callback);
  
    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        state_t state = { .depth = 0, .index = 0, .expected = tv[i].expected };

        parser.line_num  = 1;
        parser.first     = tv[i].str;
        parser.last      = parser.first + strlen(parser.first) - 1;
        parser.current   = parser.first;
        parser.status    = PDS_OK;
        parser.user_data = &state;
  
        int ret = PDS_parse_statement(&parser);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        }
    }
    return MUNIT_OK;
}

MunitResult parse_real_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        double expected;
        int status;
    } tv[] = {
        { "0.6170492162", 12,  0.6170492162, PDS_OK },
        {"-0.3187327298", 13, -0.3187327298, PDS_OK },
        {"-0.5045034227", 13, -0.5045034227, PDS_OK },
        {"-0.5129678433", 13, -0.5129678433, PDS_OK },
        { "   511-", 6,     511.0, PDS_OK },
        {"-71.600 ", 7,     -71.6, PDS_OK },
        { "+0.0004", 7,    0.0004, PDS_OK },  
        {   "1.e3@", 4,    1000.0, PDS_OK },  
        {"211.32e4", 8, 2113200.0, PDS_OK },  
        {  "-1.e-5", 6,    -1.e-5, PDS_OK },  
        {  "-.2e-1", 6,     -0.02, PDS_OK },  
        {  "-.01e3", 6,     -10.0, PDS_OK },  
        {   "af.01", 0,       0.0, PDS_OK },
        {     "-.e", 0,       0.0, PDS_INVALID_VALUE }
    };

    (void)params;
    (void)fixture;

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        parser.line_num = 1;
        parser.first    = tv[i].str; 
        parser.last     = parser.first + strlen(parser.first) - 1; 
        parser.current  = parser.first; 
        parser.status   = PDS_OK; 

        int ret = PDS_parse_real(&parser);
        munit_assert_int(tv[i].status, ==, parser.status);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        if(ret) {
            munit_assert_int(PDS_REAL_VALUE, ==, parser.scalar.type);
            munit_assert_double_equal(tv[i].expected, parser.scalar.real.value, 6);
        }
    }
    return EXIT_SUCCESS;
}

MunitResult parse_int_base_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        int base;
        int64_t value;
        int status;
    } tv[] = {
        {   " \t-714@_",  6, 10,   -714, PDS_OK },
        {        "bEeF",  4, 16, 0xbeef, PDS_OK },
        {    "10001111",  8,  2,   0x8f, PDS_OK },
        {    "+1339/6 ",  4,  8,     91, PDS_OK },
        {       "dummy",  0,  7,      0, PDS_INVALID_VALUE },
        {  "21874836501234567890", 20, 10, INT64_MAX, PDS_INVALID_RANGE },
        { "-30000000000000000000", 21, 10, INT64_MIN, PDS_INVALID_RANGE }
    };

    (void)params;
    (void)fixture;

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        const char *first = tv[i].str;
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
        int status = PDS_OK;
    
        int64_t value = PDS_parse_int_base(first, last, &end, tv[i].base, &status);
        munit_assert_int64(tv[i].value, ==, value);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, end);
        munit_assert_int(tv[i].status, ==, status);
    }
    return MUNIT_OK;
}

MunitResult parse_int_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        int32_t expected;
        int status;
    } tv[] = {
        {   "   511-", 6,       511, PDS_OK },
        {"2#+0101#@ ", 8,         5, PDS_OK },
        {    "8#foo#", 2,         0, PDS_INVALID_VALUE },
        {   "10#-9a#", 5, INT32_MAX, PDS_INVALID_VALUE }, 
        {    "16#ffc", 6, INT32_MAX, PDS_INVALID_VALUE }, 
        {       "4##", 2,         0, PDS_INVALID_VALUE }
    };
    
    (void)params;
    (void)fixture;
    
    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        parser.line_num = 1;
        parser.first    = tv[i].str;
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;
        
        int ret = PDS_parse_int(&parser);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            munit_assert_ptr_equal(tv[i].str+ tv[i].end, parser.current);
            munit_assert_int(PDS_INTEGER_VALUE, ==, parser.scalar.type);
            munit_assert_int32(tv[i].expected, ==, parser.scalar.integer.value);
        }
    }
    return EXIT_SUCCESS;
}

MunitResult parse_identifier_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        int expected;
        int status;
    } tv[] = {
        {                    "ROSETTA:",  7, 1, PDS_OK },
        { "SPACECRAFT_CLOCK_STOP_COUNT", 27, 1, PDS_OK },
        {             "INVALID_ENDING_",  0, 0, PDS_INVALID_VALUE },  
        {           "DOUBLE__SEPARATOR",  0, 0, PDS_INVALID_VALUE }, 
        {                 "INVALID_@ID",  0, 0, PDS_INVALID_VALUE },
        {                 "_INVALID_ID",  0, 0, PDS_INVALID_VALUE }
    };    

    (void)params;
    (void)fixture;

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        const char *first = tv[i].str;
        const char *last  = first + strlen(first) - 1;
        const char *end   = 0;
        int status = PDS_OK;
    
        const char *ptr = PDS_parse_identifier(first, last, &end, &status);
        munit_assert_int(tv[i].expected, ==, (ptr!=0));
        munit_assert_ptr_equal(tv[i].str + tv[i].end, end);
        munit_assert_int(tv[i].status, ==, status);
    }
    return MUNIT_OK;
}

MunitResult parse_datetime_test(const MunitParameter params[], void* fixture) {
    typedef struct
    {
        int ret;
        PDS_datetime t;
    } expected_datetime_t;

    static const expected_datetime_t values[] =
    {
        { 1, {0,1977, 24, 7, 0, 0, 0,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,1985,320, 0, 0, 0, 0,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,   0,  0, 0,12, 7,31,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,   0,  0, 0, 8,15, 1,  2450,-8,-15, PDS_ZONED_TIME } },
        { 1, {0,2015, 14, 7,11,49,57,     0, 0,  0, PDS_LOCAL_TIME } },
        { 1, {0,2016,  3, 1,18,21,49,129900, 2, 10, PDS_ZONED_TIME } },
        { 1, {0,1977,  5, 9,12,56, 0,     0, 0,  0, PDS_UTC_TIME   } },
        { 1, {0,1997, 15,10, 8,43, 0,     0, 1,  0, PDS_ZONED_TIME } },
        { 0, {0,   0,  0, 0, 0, 0, 0,     0, 0,  0, PDS_LOCAL_TIME } }, 
    };

    struct {
        const char *str;
        int end;
        const expected_datetime_t expected;
        int status;
    } tv[] = {
        { "1977-07-24 /* single date */", 10, values[0], PDS_OK },
        { "1985-320   /* day of year */",  8, values[1], PDS_OK },
        { "12:07:31                    ",  8, values[2], PDS_OK },
        { "08:15:1.00245-08:15         ", 19, values[3], PDS_OK },
        { "2015-07-14T11:49:57\r\n     ", 19, values[4], PDS_OK },
        { "2016-01-3T18:21:49.1299+2:10", 28, values[5], PDS_OK },
        { "1977-09-05T12:56:000.000Z\t ", 25, values[6], PDS_OK },
        { "1997-10-15T08:43:00+1       ", 21, values[7], PDS_OK },
        { "11-11-11          /* year */",  0, values[8], PDS_INVALID_RANGE },
        { "1989-53-11       /* month */",  0, values[8], PDS_INVALID_RANGE },
        { "1975-04-34         /* day */",  0, values[8], PDS_INVALID_RANGE },
        { "1976-11-11T /* empty time */",  0, values[8], PDS_INVALID_VALUE },
        { "1977-12-24T07 /*incomplete*/",  0, values[8], PDS_INVALID_VALUE },
        { "1977-12-24T07:15:00-44      ",  0, values[8], PDS_INVALID_RANGE },
        { "1977-12-24T07:15:00+12:75   ",  0, values[8], PDS_INVALID_RANGE }
    };
    
    (void)params;
    (void)fixture;

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        parser.line_num = 1;
        parser.first    = tv[i].str;
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;
        
        int ret = PDS_parse_datetime(&parser);
        munit_assert_int(tv[i].expected.ret, ==, ret);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            PDS_datetime *date_time = &parser.scalar.date_time;
            munit_assert_int(PDS_DATE_TIME_VALUE, ==, parser.scalar.type);
            munit_assert_int(tv[i].expected.t.year, ==, date_time->year);
            munit_assert_int(tv[i].expected.t.day, ==, date_time->day);
            munit_assert_int(tv[i].expected.t.month, ==, date_time->month);
            munit_assert_int(tv[i].expected.t.hour, ==, date_time->hour);
            munit_assert_int(tv[i].expected.t.minute, ==, date_time->minute);
            munit_assert_int(tv[i].expected.t.second, ==, date_time->second);
            munit_assert_int(tv[i].expected.t.microsecond, ==, date_time->microsecond);
            munit_assert_int(tv[i].expected.t.hour_offset, ==, date_time->hour_offset);
            munit_assert_int(tv[i].expected.t.minute_offset, ==, date_time->minute_offset);
            munit_assert_int(tv[i].expected.t.time_type, ==, date_time->time_type);
        }
    }
    return MUNIT_OK;
}

MunitResult parse_symbol_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        int expected;
        int status;
    } tv[] = {
        { "' \t _/<@>)?!#$.;'", 17, 1, PDS_OK },
        {   "'Itchy&Scratchy'", 16, 1, PDS_OK },
        {"'New York 1997'    ", 15, 1, PDS_OK },
        {            "'%'    ",  3, 1, PDS_OK },
        {    "'ANEW-LINE\r\n'",  0, 0, PDS_INVALID_VALUE }, 
        {"'' /*empty string*/",  0, 0, PDS_INVALID_VALUE } 
    };
    
    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    (void)params;
    (void)fixture;

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        parser.scalar.type = PDS_UNKNOWN_VALUE;

        parser.line_num = 1;
        parser.first    = tv[i].str;
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;
        
        int ret = PDS_parse_symbol(&parser);
        
        munit_assert_int(tv[i].expected, ==, ret);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            munit_assert_int(PDS_SYMBOLIC_VALUE, ==, parser.scalar.type);
            munit_assert_ptr_equal(tv[i].str + 1, parser.scalar.symbolic.first);
            munit_assert_ptr_equal(tv[i].str + tv[i].end - 2, parser.scalar.symbolic.last);
        }
    }
    return MUNIT_OK;
}

MunitResult parse_unit_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        int expected;
        int status;        
    } tv[] = {
        { "<measurement_unit>", 18, 1, PDS_OK },
        {         "<km/s>\r\n",  6, 1, PDS_OK },
        {         "<m**3/kg> ",  9, 1, PDS_OK }, 
        {    "<GM*CM*SEC**-2>", 15, 1, PDS_OK }, 
        {          "<km*s**t>",  0, 0, PDS_INVALID_VALUE }, 
        {         "<foor%bar>",  0, 0, PDS_INVALID_VALUE }, 
        {        "<unit_a//5>",  0, 0, PDS_INVALID_VALUE }, 
        {          "<MICRON/>",  0, 0, PDS_INVALID_VALUE }, 
        {               "<m*>",  0, 0, PDS_INVALID_VALUE }, 
        {       "<degree*808>",  0, 0, PDS_INVALID_VALUE } 
    };

    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    (void)params;
    (void)fixture;

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        parser.line_num = 1;
        parser.first    = tv[i].str;
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.status   = PDS_OK;

        parser.scalar.type = PDS_INTEGER_VALUE;
        parser.scalar.integer.unit.first = 0;
        parser.scalar.integer.unit.last  = 0;

        int ret = PDS_parse_unit(&parser);
        
        munit_assert_int(tv[i].expected, ==, ret);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        munit_assert_int(tv[i].status, ==, parser.status);
    }
    return MUNIT_OK;
}

MunitResult parse_string_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        int expected;
        int status;
    } tv[] = {
        {          "\"\" /*empty string*/",  2, 1, PDS_OK },
        {             "\"simple string\" ", 15, 1, PDS_OK },
        { "\"multi\r\nline\r\n\tstring\" ", 22, 1, PDS_OK },
        {    "\"Some\\\\escaped\\nchars\"", 22, 1, PDS_OK },
        {        "\" missing  delimiter  ",  0, 0, PDS_INVALID_VALUE } 
    };
    
    (void)params;
    (void)fixture;
    
    PDS_parser parser;
    memset(&parser, 0, sizeof(PDS_parser));
    PDS_set_error_callback(&parser.callbacks, dummy_error);

    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        parser.status   = PDS_OK;
        parser.first    = tv[i].str;
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.current  = parser.first;
        parser.line_num = 1;
 
        int ret = PDS_parse_string(&parser);      
        munit_assert_int(tv[i].expected, ==, ret);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        munit_assert_int(tv[i].status, ==, parser.status);
        if(ret) {
            munit_assert_int(PDS_TEXT_STRING_VALUE, ==, parser.scalar.type);
            munit_assert_ptr_equal(parser.first+1, parser.scalar.text.first);
            munit_assert_ptr_equal(parser.current-2, parser.scalar.text.last);
        }
    }
    return MUNIT_OK;
}

static MunitTest parse_tests[] = {
    { "statement", parse_statement_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "set", parse_set_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "sequence", parse_sequence_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "real", parse_real_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "int base", parse_int_base_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "int", parse_int_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "identifier", parse_identifier_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "datetime", parse_datetime_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "symbol", parse_symbol_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "unit", parse_unit_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "string", parse_string_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite parse_suite = {
    "parsing test suite/", parse_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main (int argc, char* const* argv) {
    return munit_suite_main(&parse_suite, NULL, argc, argv);
}