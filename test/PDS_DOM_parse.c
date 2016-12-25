#include <errno.h>
#include <tinypds.h>
#include <tinypds_dom.h>

#include "test.h"
#include "utils.h"

#include <tinypds.h>

#define TINY_PDS_DOM_IMPL
#include <tinypds_dom.h>

void tabs(int depth)
{
    int i;
    for(i=0; i<depth; i++)
    {
        printf("\t");
    }
}

void print_DOM_scalar(PDS_item *item, int depth)
{
    PDS_scalar scalar;
    int i, j;
    switch(PDS_DOM_scalar_typeof(item))
    {
        case PDS_NONE:
            break;
        case PDS_SINGLE:
            if(PDS_DOM_scalar_get(item, &scalar))
            {
                print_scalar(&scalar);
            }
            break;
        case PDS_SET:
            {
                printf("\n");
                int count = PDS_DOM_scalar_count(item);
                for(i=0; i<count; i++)
                {
                    tabs(depth+1);
                    if(PDS_DOM_set_get(item, &scalar, i))
                    {
                        print_scalar(&scalar);
                    }
                }
            }
            break;
        case PDS_SEQUENCE_1D:
            {
                printf("\n");
                int count = PDS_DOM_scalar_count(item);
                for(i=0; i<count; i++)
                {
                    tabs(depth+1);
                    if(PDS_DOM_sequence1d_get(item, &scalar, i))
                    {
                        print_scalar(&scalar);
                    }
                }
            }
            break;
        case PDS_SEQUENCE_2D:
            {
                int rows = PDS_DOM_sequence2d_rows(item);
                for(i=0; i<rows; i++)
                {
                    int cols = PDS_DOM_sequence2d_cols(item, i);
                    for(j=0; j<cols; j++)
                    {
                        tabs(depth+1);
                        if(PDS_DOM_sequence2d_get(item, &scalar, i, j))
                        {
                            print_scalar(&scalar);
                        }
                    }
                    printf("\n");
                }
            }
            break;
    }
}

void print(PDS_item *item, int depth)
{
    for( ; NULL != item; item=PDS_DOM_sibling(item))
    {
        tabs(depth);
        switch(item->type)
        {
            case PDS_ATTRIBUTE:
                printf("[attribute] ");
                print_string(item->name.first, item->name.last);
                printf(" = ");
                print_DOM_scalar(item, depth+1);
                break;
            case PDS_POINTER:
                printf("[pointer  ] ");
                print_string(item->name.first, item->name.last);
                printf(" = ");
                print_DOM_scalar(item, depth+1);
                break;
            case PDS_GROUP:
                printf("[group    ] ");
                print_string(item->name.first, item->name.last);
                printf("\n");
                print(PDS_DOM_group_begin(item), depth+1);
                break;
            case PDS_OBJECT:
                printf("[object   ] ");
                print_string(item->name.first, item->name.last);
                printf("\n");
                print(PDS_DOM_object_begin(item), depth+1);
                break;
        }
    }
}

int main(int argc, const char* argv[])
{
    FILE *in;
    size_t len;
    char *buffer;

    PDS_item *item;
    PDS_error_description err;
    int ret;

    const char *filename = argv[1];
    if(argc != 2)
    {
        fprintf(stderr, "missing PDS file\n");
        return EXIT_FAILURE;
    }
    in = fopen(filename, "rb");
    if(NULL == in)
    {
        fprintf(stderr, "failed to read %s : %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }
    fseek(in, 0, SEEK_END);
    len = ftell(in);
    fseek(in, 0, SEEK_SET);
    len -= ftell(in);
    
    buffer = (char*)malloc(len);
    if(NULL == buffer)
    {
        fprintf(stderr, "unable to allocate buffer : %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    if(fread(buffer, 1, len, in) != len)
    {
        fprintf(stderr, "failed to read buffer %s : %s\n", filename, strerror(errno));
        return EXIT_FAILURE; 
    } 
    
    fclose(in);
    
    ret = PDS_DOM_parse(buffer, len, &item, &err);
    
    print(item, 0);

    PDS_DOM_delete(item);
    
    free(buffer);
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
