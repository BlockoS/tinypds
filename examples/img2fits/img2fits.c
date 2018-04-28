/* Convert images embedded in PDS3 files to FITS image files.
 * Licensed under the MIT License
 * (c) 2018 Vincent Cruz
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TINY_PDS_IMPL
#include <tinypds.h>

#include "utils.h"

const char *g_image_string = "IMAGE";

void usage()
{
    fprintf(stderr, "img2fits name input.pds out.fits\n"
                    "    name can be either IMAGE, SIGMA_MAP_IMAGE or QUALITY_MAP_IMAGE\n");
}
/** Element type **/
enum PDS_ELEMENT_TYPE
{
    PDS_NONE = 0,
    PDS_ATTRIBUTE,
    PDS_POINTER
};
/** PDS parser payload. **/
typedef struct PDS_payload_t
{
    /** Image. **/
    PDS_image image;
    /** Record size in bytes. **/
    size_t record_size;
    /** Image record number. **/
    size_t image_record;
    /** Image data offset. **/
    size_t offset;
    /** Ignore elements if non zero. **/
    int ignore;
    /** Element depth. **/
    int depth;
    /** Are we parsing the IMAGE OBJECT. **/
    int image_object;
    /** Current element type. **/
    int element_type;
    /** Current parsing operation. **/
    int (*op)(const PDS_scalar *scalar, struct PDS_payload_t *payload);
} PDS_payload;
/** Attribute info. **/
typedef struct
{
    /** Attribute name. */
    const char *name;
    /** Attribute parser. */
    int (*parser)(const PDS_scalar*, PDS_payload*);
} PDS_attributes_infos;
/**
 * Check if the PDS scalar holds an unsigned integer.
 * @param [in] scalar PDS scalar.
 * @return 1 if the PDS scalar is an unsigned integer, 0 otherwise.
 */
int is_uint(const PDS_scalar *scalar)
{
   return ((PDS_INTEGER_VALUE == scalar->type) && (scalar->integer.value >= 0));
}
/**
 * Get record size from scalar.
 * @param [in]  scalar  PDS scalar.
 * @param [out] payload PDS parser payload.
 * @return 0 if the PDS scalar is not an integer, 1 otherwise.
 */
int record_size(const PDS_scalar *scalar, PDS_payload *payload)
{
    if(!is_uint(scalar))
    {
        return 0;
    }
    payload->record_size = (size_t)scalar->integer.value;
    return 1;
}
/**
 * Get image record from PDS scalar.
 * @param [in] scalar  PDS scalar.
 * @param [in] payload PDS parser payload containing the image structure to initialize.  
 * @return 1 the scalar contains an unsigned integer, 0 otherwise. 
 */
int image_record(const PDS_scalar *scalar, PDS_payload *payload)
{
    if (PDS_INTEGER_VALUE != scalar->type)
    {
        return 0;
    }
    payload->image_record = (size_t) scalar->integer.value;
    payload->offset = 0;
    if(scalar->integer.unit.first)
    {
        const char *bytes_unit = "BYTES";
        if(PDS_string_case_compare(scalar->integer.unit.first, scalar->integer.unit.last, &bytes_unit[0], &bytes_unit[4]))
        {
            payload->offset = payload->image_record - 1;
        }
    }
    return 1;
}
/**
 * Get image line count from PDS scalar.
 * @param [in] scalar  PDS scalar.
 * @param [in] payload PDS parser payload containing the image structure to initialize.  
 * @return 1 if the LINES attributes contains an unsigned integer, 0 otherwise. 
 */
int image_lines(const PDS_scalar *scalar, PDS_payload *payload)
{
    if(!is_uint(scalar))
    {
        return 0;
    }
    payload->image.lines = (size_t)scalar->integer.value;
    return 1;
}
/**
 * Get image samples per line from PDS scalar.
 * @param [in] scalar  PDS scalar.
 * @param [in] payload PDS parser payload containing the image structure to initialize.  
 * @return 1 if the LINE_SAMPLES attributes contains an unsigned integer, 0 otherwise. 
 */
int image_samples_per_line(const PDS_scalar *scalar, PDS_payload *payload)
{
    if(!is_uint(scalar))
    {
        return 0;
    }
    payload->image.samples_per_line = (size_t)scalar->integer.value;
    return 1;
}
/**
 * Get image sample bit count from PDS scalar.
 * @param [in] scalar  PDS scalar.
 * @param [in] payload PDS parser payload containing the image structure to initialize.  
 * @return 1 if SAMPLE_BITS attributes contains an unsigned integer, 0 otherwise. 
 */
int image_sample_bits(const PDS_scalar *scalar, PDS_payload *payload)
{
    if(!is_uint(scalar))
    {
        return 0;
    }
    payload->image.sample_bits = (size_t)scalar->integer.value;
    return 1;
}
/**
 * Get image band count from PDS scalar.
 * @param [in] scalar  PDS scalar.
 * @param [in] payload PDS parser payload containing the image structure to initialize.  
 * @return 1 if BANDS attributes contains an unsigned integer, 0 otherwise. 
 */
int image_bands(const PDS_scalar *scalar, PDS_payload *payload)
{
    if(!is_uint(scalar))
    {
        return 0;
    }
    payload->image.bands = (size_t)scalar->integer.value;
    return 1;
}
/**
 * Get image sample type from PDS scalar.
 * @param [in] scalar  PDS scalar.
 * @param [in] payload PDS parser payload containing the image structure to initialize.  
 * @return 1 if SAMPLE_TYPE attributes contains a valid type, 0 otherwise. 
 */
int image_sample_type(const PDS_scalar *scalar, PDS_payload *payload)
{
    const char *first;
    const char *last;
    switch(scalar->type)
    {
        case PDS_TEXT_STRING_VALUE:
            first = scalar->text.first;
            last  = scalar->text.last;
            break;
        case PDS_SYMBOLIC_VALUE:
            first = scalar->symbolic.first;
            last  = scalar->symbolic.last;
            break;
        case PDS_IDENTIFIER_VALUE:
            first = scalar->identifier.first;
            last  = scalar->identifier.last;
            break;
        default:
            return 0;
    }
    payload->image.sample_type = convert_type_name(first, last);
    if(PDS_SAMPLE_UNKNOWN_TYPE == payload->image.sample_type)
    {
        return 0;
    }
    return 1;
}
/**
 * Print parsing error message to the standard error output.
 * @param [in] desc   Error description.
 * @param [in] unused PDS parser user data.
 */
void print_error(const PDS_error_description* desc, void *unused)
{
    (void)unused;
    fprintf(stderr, "line %d:%d: %s\n", desc->number, desc->position, desc->msg);
}
/**
 * Global attribute parser.
 * @param [in]     first     Pointer to the first character of the attribute name.
 * @param [in]     last      Pointer to the last character of the attribute name.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success or 0 if an error occured.
 */
int parse_attribute_begin(const char *first, const char *last, void *user_data)
{
    static PDS_attributes_infos base_attributes[] =
    {
        { "RECORD_BYTES", record_size }

    };
    static const size_t base_attributes_count = ARRAY_SIZE(base_attributes);

    static PDS_attributes_infos image_attributes[] =
    {
        {"LINES",        image_lines            },
        {"LINE_SAMPLES", image_samples_per_line },
        {"SAMPLE_BITS",  image_sample_bits      },
        {"SAMPLE_TYPE",  image_sample_type      },
        {"BANDS",        image_bands            }
    };
    static const size_t image_attributes_count = ARRAY_SIZE(image_attributes);

    PDS_attributes_infos *attributes;

    size_t count;
    size_t i;

    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    if(payload->ignore)
    {
        return 1;
    }
    payload->element_type = PDS_ATTRIBUTE;
    if(payload->image_object)
    {
        attributes = image_attributes;
        count = image_attributes_count;
    }
    else if(0 == payload->depth)
    {
        attributes = base_attributes;
        count = base_attributes_count;
    }
    else
    {
        return 1;
    }

    payload->op = NULL;
    for(i=0; i<count; i++)
    {
        if (PDS_string_case_compare(attributes[i].name, str_last(attributes[i].name), first, last))
        {
            payload->op = attributes[i].parser;
            return 1; 
        }
    }
    return 1;
}
/**
 * End of attribute declaration callback.
 * @param [in]     first     Pointer to the first character of the attribute name.
 * @param [in]     last      Pointer to the last character of the attribute name.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success or 0 if an error occured.
 */
int parse_attribute_end(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    (void)first;
    (void)last;
    if(NULL == payload)
    {
        return 0;
    }
    payload->element_type = PDS_NONE;
    return 1;
}
/**
 * Scalar callback.
 * @param [in]     scalar    Scalar value.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success or 0 if an error occured.
 */
int parse_scalar(const PDS_scalar *scalar, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    if(payload->op)
    {
        int ret = payload->op(scalar, payload);
        payload->op = NULL;
        if(!ret)
        {
            return 0;
        }
    }
    return 1;
}
/**
 * Global pointer parser.
 * We are only interested in the pointer associated to the image. 
 * The rest is ignored.
 * @param [in]     first     Pointer to the first character of the pointer name.
 * @param [in]     last      Pointer to the last character of the pointer name.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success or 0 if an error occured.
 */
int parse_pointer_begin(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    if(payload->ignore)
    {
        return 1;
    }
    payload->element_type = PDS_POINTER;
    if(0 == payload->depth)
    {
        if (PDS_string_case_compare(g_image_string, str_last(g_image_string), first, last))
        {
            payload->op = image_record;
        }
    }
    return 1;
}
/**
 * End of pointer declaration callback.
 * @param [in]     first     Pointer to the first character of the pointer name.
 * @param [in]     last      Pointer to the last character of the pointer name.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success or 0 if an error occured.
 */
int parse_pointer_end(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    (void)first;
    (void)last;
    if(NULL == payload)
    {
        return 0;
    }
    payload->element_type = PDS_NONE;
    return 1;
}
/**
 * Dummy callback for the start and end of set and sequence.
 * @param [in,out] user_data PDS parser user data. (unused)
 * @return always 1. 
 */
int dummy_begin_end(void *user_data)
{
    (void)user_data;
    return 1;
}
/**
 * Object start callback.
 * Only the specified image object is being parsed.
 * All other objects are ignored.
 * @param [in]     first     Pointer to the first character of the object name.
 * @param [in]     last      Pointer to the last character of the object name.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success, 0 otherwise.
 */
int object_begin(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    payload->depth++;
    if(  payload->ignore || (payload->depth > 1) ||
        (!PDS_string_case_compare(g_image_string, str_last(g_image_string), first, last)) )
    {
        payload->ignore++;
    }
    else
    {
        payload->image_object = 1;
    }
    return 1;
}
/**
 * Object end callback.
 * Only the specified image object is being parsed.
 * All other objects are ignored.
 * @param [in]     first     Pointer to the first character of the object name.
 * @param [in]     last      Pointer to the last character of the object name.
 * @param [in,out] user_data PDS parser user data.
 * @return 1 upon success, 0 otherwise.
 */
int object_end(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    if(payload->ignore)
    {
        payload->ignore--;
    }
    else if((1 == payload->depth) && PDS_string_case_compare(g_image_string, str_last(g_image_string + strlen(g_image_string)), first, last))
    {
        payload->image_object = 0;
    }
    payload->depth--;
    return 1;
}
/**
 * Group start callback.
 * @note Groups are ignored.
 * @param [in]     first     Pointer to the first character of the group name. (unused)
 * @param [in]     last      Pointer to the last character of the group name. (unused)
 * @param [in,out] user_data PDS parser user data. (unused)
 * @return always 1.
 */ 
int group_begin(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    payload->depth++;
    payload->ignore++;
    (void)first;
    (void)last;
    return 1;
}
/**
 * Group end callback.
 * @note Groups are ignored.
 * @param [in]     first     Pointer to the first character of the group name. (unused)
 * @param [in]     last      Pointer to the last character of the group name. (unused)
 * @param [in,out] user_data PDS parser user data. (unused)
 * @return always 1.
 */ 
int group_end(const char *first, const char *last, void *user_data)
{
    PDS_payload *payload = (PDS_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    payload->depth--;
    payload->ignore--;
    (void)first;
    (void)last;
    return 1;
}
/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
    PDS_callbacks callbacks;
    PDS_payload payload;

    char *buffer;
    size_t size;

    int ret;

    if(4 != argc)
    {
        usage();
        return EXIT_FAILURE;
    }

	g_image_string = argv[1];

    if(!buffer_from_file(argv[2], &buffer, &size))
    {
        return EXIT_FAILURE;
    }

    memset(&payload, 0, sizeof(PDS_payload));
    payload.image.bands = 1;

    PDS_set_error_callback(&callbacks, print_error);
    PDS_set_scalar_callback(&callbacks, parse_scalar);
    PDS_set_attribute_callbacks(&callbacks, parse_attribute_begin, parse_attribute_end);
    PDS_set_pointer_callbacks(&callbacks, parse_pointer_begin, parse_pointer_end);
    PDS_set_set_callbacks(&callbacks, dummy_begin_end, dummy_begin_end);
    PDS_set_sequence_callbacks(&callbacks, dummy_begin_end, dummy_begin_end);
    PDS_set_group_callbacks(&callbacks, group_begin, group_end);
    PDS_set_object_callbacks(&callbacks, object_begin, object_end);

    ret = PDS_parse(&callbacks, buffer, (int)size, &payload);
    if(ret)
    {
        if(!payload.offset) {
            payload.offset = (payload.image_record-1) * payload.record_size;
        }
        ret = write_image(argv[3], buffer, size, payload.offset, &payload.image);
    }

    free(buffer);
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
