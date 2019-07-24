/* Random utility functions.
 * Licensed under the MIT License
 * (c) 2018 Vincent Cruz
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <fitsio.h>

#include <tinypds.h>

#include "utils.h"

/* Returns a pointer to the last character of a null terminated string. */
const char* str_last(const char *str)
{
    return str + strlen(str) - 1;    
}

/* Store file content to memory. */
int buffer_from_file(const char *filename, char **buffer, size_t *len)
{
    FILE *stream;
    char *data;
    size_t size;

    *buffer = NULL;
    *len = 0;

    stream = fopen(filename, "rb");
    if(NULL == stream)
    {
        fprintf(stderr, "failed to open %s: %s\n", filename, strerror(errno));
        return 0;
    }

    fseek(stream, 0, SEEK_END);
    size = (size_t)ftell(stream);
    fseek(stream, 0, SEEK_SET);
    size -= (size_t)ftell(stream);

    data = (char*)malloc(size);
    if(data)
    {
        if(size == fread(data, 1, size, stream))
        {
            *buffer = data;
            *len = size;
        }
        else
        {
            fprintf(stderr, "read error from %s : %s\n", filename, strerror(errno));
            free(data);
        }
    }
    else
    {
        fprintf(stderr, "allocation error : %s\n", strerror(errno));
    }
    fclose(stream);
    return *buffer ? 1 : 0;
}

/* Convert sample type name to PDS_SAMPLE_TYPE. */
uint8_t convert_type_name(const char *first, const char *last)
{
    static const struct
    {
        const char *name;
        uint8_t type;
    } type_names[] =
    {
        { "MSB_INTEGER", PDS_SAMPLE_INT_MSB },
        { "LSB_INTEGER", PDS_SAMPLE_INT_LSB },
        { "MSB_UNSIGNED_INTEGER", PDS_SAMPLE_UINT_MSB },
        { "LSB_UNSIGNED_INTEGER", PDS_SAMPLE_UINT_LSB },
        { "PC_REAL", PDS_SAMPLE_FLOAT },
        { "IEEE_REAL", PDS_SAMPLE_FLOAT }
    };
    size_t i;
    for(i=0; i<ARRAY_SIZE(type_names); i++)
    {
        if(PDS_string_case_compare(type_names[i].name, str_last(type_names[i].name), first, last))
        {
            return type_names[i].type;
        }
    }
    return PDS_SAMPLE_UNKNOWN_TYPE;
}

/* Convert embedded image to FITS. */
int write_image(const char *filename, char *buffer, size_t size, size_t offset, PDS_image *image)
{
    size_t bytes_per_sample = image->sample_bits / 8;
    size_t element_count = image->lines * image->samples_per_line;
    size_t byte_count = element_count * bytes_per_sample;
    if((offset + byte_count) > size)
    {
        return 0;
    }
   
    fitsfile *fptr; 
    int status = 0;
    long axis_count[] = { image->samples_per_line, image->lines };
   
    int bitpix, datatype;
    switch(image->sample_type)
    {
        case PDS_SAMPLE_INT_MSB:
        case PDS_SAMPLE_INT_LSB:
            switch(image->sample_bits)
            {
                case 8:
                    bitpix   = SBYTE_IMG;
                    datatype = TSBYTE;
                    break;
                case 16:
                    bitpix   = SHORT_IMG;
                    datatype = TSHORT;
                    break;
                case 32:
                    bitpix   = LONG_IMG;
                    datatype = TINT;
                    break;
                case 64:
                    bitpix   = LONGLONG_IMG;
                    datatype = TLONGLONG;
                    break;
                default:
                    fprintf(stderr, "Unsupported type (uint)\n");
                    return 0;
            }
            break;
        case PDS_SAMPLE_UINT_MSB:
        case PDS_SAMPLE_UINT_LSB:
            switch(image->sample_bits)
            {
                case 8:
                    bitpix   = BYTE_IMG;
                    datatype = TBYTE;
                    break;
                case 16:
                    bitpix   = USHORT_IMG;
                    datatype = TUSHORT;
                    break;
                case 32:
                    bitpix   = ULONG_IMG;
                    datatype = TUINT;
                    break;
                default:
                    fprintf(stderr, "Unsupported type (uint)\n");
                    return 0;
            }
            break;
        case PDS_SAMPLE_FLOAT:       
            switch(image->sample_bits)
            {
                case 32:
                    bitpix   = FLOAT_IMG;
                    datatype = TFLOAT;
                    break;
                case 64:
                    bitpix   = DOUBLE_IMG;
                    datatype = TDOUBLE;
                    break;
                default:
                    fprintf(stderr, "Unsupported type (float)\n");
                    return 0;
            }
            break;
        default:
            fprintf(stderr, "Unsupported type\n");
            return 0;
    }
    
    // Converts MSB to LSB
    if(((image->sample_type == PDS_SAMPLE_INT_MSB) || (image->sample_type == PDS_SAMPLE_UINT_MSB)) && (image->sample_bits != 8)) {
        uint8_t *ptr = (uint8_t*)(buffer+offset);
        for(size_t i=0; i<element_count; i++) {
            uint8_t tmp;
            switch(image->sample_bits) {
                case 16:
                    tmp = ptr[0];
                    ptr[0] = ptr[1];
                    ptr[1] = tmp;
                    ptr += 2;
                    break;
                case 32:
                    tmp = ptr[0];
                    ptr[0] = ptr[3];
                    ptr[3] = tmp;
                    tmp = ptr[1];
                    ptr[1] = ptr[2];
                    ptr[2] = tmp;                    
                    ptr += 4;
                    break; 
                case 64:
                    tmp = ptr[0];
                    ptr[0] = ptr[7];
                    ptr[7] = tmp;
                    tmp = ptr[1];
                    ptr[1] = ptr[6];
                    ptr[6] = tmp;                    
                    tmp = ptr[2];
                    ptr[2] = ptr[5];
                    ptr[5] = tmp;                    
                    tmp = ptr[3];
                    ptr[3] = ptr[4];
                    ptr[4] = tmp;                    
                    ptr += 8;
                    break; 
            }
        }
    }
    
    fits_create_file(&fptr, filename, &status); 
    fits_create_img(fptr, bitpix, 2, axis_count, &status);
    fits_write_img(fptr, datatype, image->bands, element_count, buffer+offset, &status);
    fits_close_file(fptr, &status);
    fits_report_error(stderr, status);
    
    return status ? 0 : 1;
}
