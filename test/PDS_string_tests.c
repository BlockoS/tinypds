#include <munit.h>

#define TINY_PDS_IMPL
#include <tinypds.h>

void dummy_error(const PDS_error_description *desc, void *unused)
{
    (void)unused;
    munit_logf(MUNIT_LOG_INFO, "line %d:%d: %s\n", desc->number, desc->position, desc->msg);
}

MunitResult find_first_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        int end;
        char expected;
        int status;
    } tv[] = {
        {                      "  b1234\t`#_Q3#[S}",  9, '#', PDS_OK },
        { "namespace:VALUE_1    =  'literal'  \r\n", 21, '=', PDS_OK },
        {                          "I hate clowns!",  0, 'I', PDS_OK }, 
        {           "Klatu verata ni-mumble mumble", -1, '*', PDS_INVALID_VALUE }
    };
    
    (void)params;
    (void)fixture;
    
    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        const char *first = tv[i].str;
        const char *last  = first + strlen(first) - 1;

        const char *found = PDS_find_first(first, last, tv[i].expected);
        if(PDS_OK != tv[i].status) {
            munit_assert_ptr_equal(0, found);
        }
        else {
            munit_assert_ptr_equal(tv[i].str + tv[i].end, found);
        }
    }
    return MUNIT_OK;
}

MunitResult skip_whitespaces_test(const MunitParameter params[], void* fixture) {
    struct {
        const char *str;
        size_t end;
        int ret;
        int line;
        int status;
    } tv[] = {
        {                                        "nothing to skip!",  0, 1, 1, PDS_OK },
        {                                          "\t    \r\n end",  8, 1, 2, PDS_OK },
        {                                               " /***/end",  6, 1, 1, PDS_OK },
        {                                                "/**/ end",  5, 1, 1, PDS_OK },
        {                   "/* a\tlonger\tcomment */\t\r\n\r\nend", 27, 1, 3, PDS_OK },
        {"  \t/*line 1 */\n \t\t/*line 2*/\n \t\t\t/*line 3*/\nend", 44, 1, 4, PDS_OK },
        {                               "  /* /* nested comment */",  6, 0, 1, PDS_INVALID_VALUE },
        {                            "  /* muti-line \n comment */", 15, 0, 1, PDS_INVALID_VALUE },
        {                                                  "  /*/ ",  6, 0, 1, PDS_INCOMPLETE },
        {                                 "  /* incomplete comment", 23, 0, 1, PDS_INCOMPLETE },
        {                                                       "/",  1, 0, 1, PDS_INCOMPLETE },
        {                                                   " /\t*",  2, 0, 1, PDS_INVALID_VALUE },
        {                                                  "\t\t/*",  4, 0, 1, PDS_INCOMPLETE },
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
        parser.current  = parser.first;
        parser.last     = parser.first + strlen(parser.first) - 1;
        parser.status   = PDS_OK;
        int ret;
        ret = PDS_skip_whitespaces(&parser);
        munit_assert_int(tv[i].status, ==, parser.status);
        munit_assert_ptr_equal(tv[i].str + tv[i].end, parser.current);
        munit_assert_int(tv[i].ret, ==, ret);
        munit_assert_int(tv[i].line, ==, parser.line_num);
    }

    return MUNIT_OK;
}

MunitResult string_case_compare_test(const MunitParameter params[], void* fixture) {
    const char *buffer = "test string with lowercase test string, uppercase TEST STRING and mixed-case test sTrIng.";
    (void)params;
    (void)fixture;

    struct
    {
        const char *first[2];
        const char *second[2];
        int expected;
    } tv[] = {
        { { buffer   , buffer+10 }, { buffer+27, buffer+37 }, 1 }, 
        { { buffer   , buffer+10 }, { buffer+50, buffer+60 }, 1 }, 
        { { buffer+50, buffer+60 }, { buffer+77, buffer+87 }, 1 },
        { { buffer   , buffer+25 }, { buffer+27, buffer+42 }, 0 },
        { { buffer   , buffer+10 }, { buffer+27, buffer+50 }, 0 },
        { { buffer+27, buffer+47 }, { buffer   , buffer+10 }, 0 },
    };
    
    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++)
    {
        int ret = PDS_string_case_compare(tv[i].first[0], tv[i].first[1], tv[i].second[0], tv[i].second[1]);
        munit_assert_int(ret, ==, tv[i].expected);
    }   

    return MUNIT_OK;
}

MunitResult string_compare_test(const MunitParameter params[], void* fixture) {
    const char *buffer = "test string with lowercase test string, uppercase TEST STRING and mixed-case test sTrIng.";
    (void)params;
    (void)fixture;

    struct
    {
        const char *first[2];
        const char *second[2];
        int expected;
    } tv[] = {
        { { buffer   , buffer+10 }, { buffer+27, buffer+37 }, 1 }, 
        { { buffer+50, buffer+60 }, { buffer+77, buffer+87 }, 0 },
        { { buffer   , buffer+25 }, { buffer+27, buffer+42 }, 0 },
        { { buffer   , buffer+10 }, { buffer+27, buffer+50 }, 0 },
        { { buffer+27, buffer+47 }, { buffer   , buffer+10 }, 0 },
    };
    
    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++)
    {
        int ret = PDS_string_compare(tv[i].first[0], tv[i].first[1], tv[i].second[0], tv[i].second[1]);
        munit_assert_int(ret, ==, tv[i].expected);
    }

    return MUNIT_OK;
}

MunitResult trim_test(const MunitParameter params[], void* fixture) {
    (void)params;
    (void)fixture;
    
    struct {
        const char *str;
        off_t end; 
        off_t expected;
    } tv[] = {
        { "\t\n   \r test_\t  fin", 17, 7 },
        {     "\t  [ string ]\r\n", 12, 3 },
        {           "another test", 11, 0 },
        {           "_+=*\t\t\t\n",  3, 0 },
        {          "   \r\v\t \n ", -1,-1 }
    };
    
    size_t i;
    for(i=0; i<PDS_ARRAY_SIZE(tv); i++) {
        const char *first = tv[i].str;
        const char *last  = first + strlen(first) - 1;
        const char *begin = 0;
        const char *end   = 0;

        PDS_trim(first, last, &begin, &end);
        if(tv[i].expected >= 0)
            munit_assert_ptr_equal(first + tv[i].expected, begin);
        else
            munit_assert_ptr_equal(0, begin);

        if(tv[i].end >= 0)
            munit_assert_ptr_equal(first + tv[i].end, end);
        else
            munit_assert_ptr_equal(0, begin);
    }

    return MUNIT_OK;
}

static MunitTest string_tests[] = {
    { "find first", find_first_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "skip whitespaces", skip_whitespaces_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "case compare", string_case_compare_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "compare", string_compare_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "trim", trim_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite string_suite = {
    "string test suite/", string_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main (int argc, char* const* argv) {
    return munit_suite_main(&string_suite, NULL, argc, argv);
}