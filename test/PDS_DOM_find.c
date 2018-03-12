#include <munit.h>

#include <tinypds.h>
#include <tinypds_dom.h>

static const char *pds_string = 
"PDS_VERSION_ID = PDS3\r\n"
"OBJECT = object_0\r\n"
"    ATTRIBUTE_0 = 0\r\n"
"    ATTRIBUTE_1 = 1\r\n"
"    ATTRIBUTE_2 = 2\r\n"
"    OBJECT = object_1\r\n"
"        ATTRIBUTE_3 = 3\r\n"
"        OBJECT = object_2\r\n"
"            ATTRIBUTE_4 = 4\r\n"
"            ATTRIBUTE_5 = 5\r\n"
"        END_OBJECT = object_2\r\n"
"        ATTRIBUTE_6 = 6\r\n"
"        OBJECT = object_3\r\n"
"            ATTRIBUTE_7 = 7\r\n"
"            OBJECT = object_4\r\n"
"                ATTRIBUTE_8 = 8\r\n"
"            END_OBJECT = object_4\r\n"
"        END_OBJECT = object_3\r\n"
"    END_OBJECT = object_1\r\n"
"    OBJECT = object_5\r\n"
"        ATTRIBUTE_9 = 9\r\n"
"        ATTRIBUTE_A = 10\r\n"
"    END_OBJECT = object_5\r\n"
"END_OBJECT = object_0\r\n"
"ATTRIBUTE_B = 11\r\n"
"ATTRIBUTE_0 = 13\r\n"
"OBJECT = object_6\r\n"
"    GROUP = group_0\r\n"
"        ATTRIBUTE_D = 15\r\n"
"        ATTRIBUTE_E = 16\r\n"
"        ATTRIBUTE_F = 17\r\n"
"    END_GROUP = group_0\r\n"
"END_OBJECT = object_6\r\n"
"ATTRIBUTE_C = 12\r\n"
"ATTRIBUTE_8 = 14\r\n"
"END\r\n";

static void* pds_dom_setup(const MunitParameter params[], void* user_data) {
	(void)params;
	(void)user_data;
	return malloc(sizeof(PDS_item**));
}

static void pds_dom_tear_down(void* fixture) {
	if(NULL == fixture) {
		return;
	}
	PDS_item **item = (PDS_item**)fixture;
	if(*item) {
        PDS_DOM_delete(*item);
	}
	free(item);
}

MunitResult pds_dom_find_test(const MunitParameter params[], void* fixture) {
    (void)params;
	PDS_item **item = (PDS_item**)fixture;
	PDS_item *found;
	PDS_scalar scalar;
	PDS_error_description err;
	int ret;

	ret = PDS_DOM_parse(pds_string, strlen(pds_string), item, &err);
	if(!ret) {
		munit_errorf("parse error line %d:%d: %s\n", err.number, err.position, err.msg);
		return MUNIT_FAIL;
	}

	found = PDS_DOM_find("ATTRIBUTE_0", *item, PDS_ONLY_SIBLINGS, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_attribute(found));
	munit_assert_int(PDS_SINGLE, ==, PDS_DOM_scalar_typeof(found));
	
	ret = PDS_DOM_scalar_get(found, &scalar);
	munit_assert_true(ret);
	munit_assert_int(PDS_INTEGER_VALUE, ==, scalar.type);
	munit_assert_int(13, ==, scalar.integer.value);

	found = PDS_DOM_find("ATTRIBUTE_8", found, PDS_ONLY_SIBLINGS, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_attribute(found));
	munit_assert_int(PDS_SINGLE, ==, PDS_DOM_scalar_typeof(found));
	
	ret = PDS_DOM_scalar_get(found, &scalar);
	munit_assert_true(ret);
	munit_assert_int(PDS_INTEGER_VALUE, ==, scalar.type);
	munit_assert_int(14, ==, scalar.integer.value);

	found = PDS_DOM_find("ATTRIBUTE_1", *item, PDS_ONLY_SIBLINGS, 1);
	munit_assert_ptr_equal(NULL, found);

	found = PDS_DOM_find("PDS_VERSION_ID", *item, PDS_ONLY_SIBLINGS, 1);
	munit_assert_ptr_not_equal(NULL, found);

	found = PDS_DOM_find("ATTRIBUTE_8", *item, PDS_ONLY_CHILDREN, 0);
	munit_assert_ptr_equal(NULL, found);

	found = PDS_DOM_find("object_0", *item, PDS_ONLY_SIBLINGS, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_object(found));

	munit_assert_ptr_equal(NULL, PDS_DOM_find("ATTRIBUTE_8", found, PDS_ONLY_CHILDREN, 0));

	found = PDS_DOM_find("object_1", found, PDS_ONLY_CHILDREN, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_object(found));

	found = PDS_DOM_find("ATTRIBUTE_8", found, PDS_CHILDREN_RECURSIVE, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_attribute(found));
	munit_assert_int(PDS_SINGLE, ==, PDS_DOM_scalar_typeof(found));

	ret = PDS_DOM_scalar_get(found, &scalar);
	munit_assert_true(ret);
	munit_assert_int(PDS_INTEGER_VALUE, ==, scalar.type);
	munit_assert_int(8, ==, scalar.integer.value);

	found = PDS_DOM_find("ATTRIBUTE_E", *item, PDS_SIBLINGS_RECURSIVE, 1);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_attribute(found));
	munit_assert_int(PDS_SINGLE, ==, PDS_DOM_scalar_typeof(found));

	ret = PDS_DOM_scalar_get(found, &scalar);
	munit_assert_true(ret);
	munit_assert_int(PDS_INTEGER_VALUE, ==, scalar.type);
	munit_assert_int(16, ==, scalar.integer.value);

	found = PDS_DOM_find("ATTRIBUTE_2", *item, PDS_SIBLINGS_RECURSIVE, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_attribute(found));
	munit_assert_int(PDS_SINGLE, ==, PDS_DOM_scalar_typeof(found));

	ret = PDS_DOM_scalar_get(found, &scalar);
	munit_assert_true(ret);
	munit_assert_int(PDS_INTEGER_VALUE, ==, scalar.type);
	munit_assert_int(2, ==, scalar.integer.value);

	found = PDS_DOM_find("object_3", found, PDS_SIBLINGS_RECURSIVE, 0);
	munit_assert_ptr_not_equal(NULL, found);
	munit_assert_true(PDS_DOM_is_object(found));
	
    return MUNIT_OK;
}

static MunitTest pds_dom_tests[] = {
    { "find", pds_dom_find_test, pds_dom_setup, pds_dom_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite pds_dom_suite = {
    "PDS DOM test suite", pds_dom_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main (int argc, char* const* argv) {
    return munit_suite_main(&pds_dom_suite, NULL, argc, argv);
}