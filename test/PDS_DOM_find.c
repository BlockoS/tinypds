#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tinypds.h>
#include <tinypds_dom.h>

#include "test.h"

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

PDS_item *item = NULL;

void cleanup()
{
    if(item)
    {
        PDS_DOM_delete(item);
    }
}

int main()
{
    PDS_item *found;
    PDS_scalar scalar;
    PDS_error_description err;
    int ret;

    atexit(cleanup);

    ret = PDS_DOM_parse(pds_string, strlen(pds_string), &item, &err);
    if(!ret)
    {
        fprintf(stderr, "parse error line %d:%d: %s\n", err.number, err.position, err.msg);
        return EXIT_FAILURE;
    }

    found = PDS_DOM_find("ATTRIBUTE_0", item, PDS_ONLY_SIBLINGS, 0);
    check(NULL != found);
    check(PDS_DOM_is_attribute(found));
    check(PDS_SINGLE == PDS_DOM_scalar_typeof(found));
    
    ret = PDS_DOM_scalar_get(found, &scalar);
    check(ret);
    check(PDS_INTEGER_VALUE == scalar.type);
    check(13 == scalar.integer.value);

    found = PDS_DOM_find("ATTRIBUTE_8", found, PDS_ONLY_SIBLINGS, 0);
    check(NULL != found);
    check(PDS_DOM_is_attribute(found));
    check(PDS_SINGLE == PDS_DOM_scalar_typeof(found));
    
    ret = PDS_DOM_scalar_get(found, &scalar);
    check(ret);
    check(PDS_INTEGER_VALUE == scalar.type);
    check(14 == scalar.integer.value);

    found = PDS_DOM_find("ATTRIBUTE_1", item, PDS_ONLY_SIBLINGS, 1);
    check(NULL == found);

    found = PDS_DOM_find("PDS_VERSION_ID", item, PDS_ONLY_SIBLINGS, 1);
    check(NULL != found);

    found = PDS_DOM_find("ATTRIBUTE_8", item, PDS_ONLY_CHILDREN, 0);
    check(NULL == found);

    found = PDS_DOM_find("object_0", item, PDS_ONLY_SIBLINGS, 0);
    check(NULL != found);
    check(PDS_DOM_is_object(found));

    check(NULL == PDS_DOM_find("ATTRIBUTE_8", found, PDS_ONLY_CHILDREN, 0));

    found = PDS_DOM_find("object_1", found, PDS_ONLY_CHILDREN, 0);
    check(NULL != found);
    check(PDS_DOM_is_object(found));

    found = PDS_DOM_find("ATTRIBUTE_8", found, PDS_CHILDREN_RECURSIVE, 0);
    check(NULL != found);
    check(PDS_DOM_is_attribute(found));
    check(PDS_SINGLE == PDS_DOM_scalar_typeof(found));

    ret = PDS_DOM_scalar_get(found, &scalar);
    check(ret);
    check(PDS_INTEGER_VALUE == scalar.type);
    check(8 == scalar.integer.value);

    found = PDS_DOM_find("ATTRIBUTE_E", item, PDS_SIBLINGS_RECURSIVE, 1);
    check(NULL != found);
    check(PDS_DOM_is_attribute(found));
    check(PDS_SINGLE == PDS_DOM_scalar_typeof(found));

    ret = PDS_DOM_scalar_get(found, &scalar);
    check(ret);
    check(PDS_INTEGER_VALUE == scalar.type);
    check(16 == scalar.integer.value);

    found = PDS_DOM_find("ATTRIBUTE_2", item, PDS_SIBLINGS_RECURSIVE, 0);
    check(NULL != found);
    check(PDS_DOM_is_attribute(found));
    check(PDS_SINGLE == PDS_DOM_scalar_typeof(found));

    ret = PDS_DOM_scalar_get(found, &scalar);
    check(ret);
    check(PDS_INTEGER_VALUE == scalar.type);
    check(2 == scalar.integer.value);

    found = PDS_DOM_find("object_3", found, PDS_SIBLINGS_RECURSIVE, 0);
    check(NULL != found);
    check(PDS_DOM_is_object(found));

    return EXIT_SUCCESS;
}
