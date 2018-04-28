/* Convert images embedded in PDS3 files to FITS image files.
 * Licensed under the MIT License
 * (c) 2018 Vincent Cruz
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fitsio.h>

#define TINY_PDS_IMPL
#define TINY_PDS_DOM_IMPL
#include <tinypds_dom.h>

#include "utils.h"

const char *g_image_label = "IMAGE";

static void usage()
{
    fprintf(stderr, "img2fits_dom name input.pds out.fits\n"
                    "    name can be either IMAGE, SIGMA_MAP_IMAGE or QUALITY_MAP_IMAGE\n");
}

static int find_scalar(PDS_item *root, const char *name, PDS_type type, int value_type, PDS_search_type search, PDS_scalar *scalar)
{
    PDS_item *current = root;
    do
    {
        current = PDS_DOM_find(name, current, search, 0);
        if( current && (type == PDS_DOM_typeof(current))
                    && PDS_DOM_scalar_get(current, scalar)
                    && (value_type == scalar->type))
        {
            return 1;
        }
    } while(current);
    fprintf(stderr, "Unable to find attribute %s\n", name);
    return 0;
}

static int image_sample_type(const PDS_scalar *scalar, uint8_t *sample_type)
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
    *sample_type = convert_type_name(first, last);
    if(PDS_SAMPLE_UNKNOWN_TYPE == *sample_type)
    {
        return 0;
    }
    return 1;
}

/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
    PDS_item *root;
    PDS_error_description err;
    char *buffer;
    size_t size;

    int ret;

    buffer = NULL;
    root = NULL;

    if(4 != argc)
    {
        usage();
        return EXIT_FAILURE;
    }

    g_image_label = argv[1];

    if(!buffer_from_file(argv[2], &buffer, &size))
    {
        return EXIT_FAILURE;
    }

    ret = PDS_DOM_parse(buffer, size, &root, &err);
    if(!ret)
    {
        fprintf(stderr, "%s: parse error line %d:%d : %s\n", argv[2], err.number, err.position, err.msg);
    }
    else
    {
        PDS_scalar scalar;
        size_t record_size  = 0;
        size_t image_record = 0;
        
        ret = find_scalar(root, "RECORD_BYTES", PDS_ATTRIBUTE, PDS_INTEGER_VALUE, PDS_ONLY_SIBLINGS, &scalar);
        if(ret) { record_size = scalar.integer.value; }
 
        ret = ret && find_scalar(root, g_image_label, PDS_POINTER, PDS_INTEGER_VALUE, PDS_ONLY_SIBLINGS, &scalar);
        if(ret)
        {
            PDS_item *current = root;
            image_record = scalar.integer.value;
            int compute_offset = 1;
            if(scalar.integer.unit.first) 
            {
                const char *bytes_unit = "BYTES";
                if(PDS_string_case_compare(scalar.integer.unit.first, scalar.integer.unit.last, &bytes_unit[0], &bytes_unit[4])) 
                {
                    compute_offset = 0;
                }
            }
            
            ret = 0;
            do
            {
                current = PDS_DOM_find(g_image_label, current, PDS_ONLY_SIBLINGS, 0);
            } while(current && !PDS_DOM_is_object(current));
            if(current)
            {
                PDS_image image;
                memset(&image, 0, sizeof(PDS_image));
                
                ret = find_scalar(current, "LINES", PDS_ATTRIBUTE, PDS_INTEGER_VALUE, PDS_ONLY_CHILDREN, &scalar);
                if(ret) { image.lines = scalar.integer.value; }

                ret = ret && find_scalar(current, "LINE_SAMPLES", PDS_ATTRIBUTE, PDS_INTEGER_VALUE, PDS_ONLY_CHILDREN, &scalar);
                if(ret) { image.samples_per_line = scalar.integer.value; }
                
                ret = ret && find_scalar(current, "SAMPLE_BITS", PDS_ATTRIBUTE, PDS_INTEGER_VALUE, PDS_ONLY_CHILDREN, &scalar);
                if(ret) { image.sample_bits = scalar.integer.value; }
                
                ret = ret && find_scalar(current, "BANDS", PDS_ATTRIBUTE, PDS_INTEGER_VALUE, PDS_ONLY_CHILDREN, &scalar);
                if(ret) { image.bands = scalar.integer.value; }
                else { 
                    fprintf(stderr, "assuming BANDS = 1\n");
                    ret = 1;
                    image.bands = 1;
                }
                
                ret = ret && find_scalar(current, "SAMPLE_TYPE", PDS_ATTRIBUTE, PDS_IDENTIFIER_VALUE, PDS_ONLY_CHILDREN, &scalar);
                if(ret)
                {
                    size_t offset = compute_offset ? ((image_record-1) * record_size) : (image_record-1);
                    ret = image_sample_type(&scalar, &image.sample_type);
                    printf("%ld\n", offset);
                    ret = ret && write_image(argv[3], buffer, size, offset, &image);
                }
            }
        }
    }
    PDS_DOM_delete(root);
    free(buffer);
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
