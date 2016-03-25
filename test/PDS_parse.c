#include <errno.h>
#include <tinypds.h>

#include "test.h"
#include "utils.h"

#define TINY_PDS_IMPL
#include <tinypds.h>

typedef struct
{
    int depth;
} user_data_t;

void print_tab(int depth)
{
    int i;
    for(i=0; i<depth; i++)
    {
        printf("    ");
    }
}
int dummy_attribute(const char *first, const char *last, const PDS_scalar *scalar, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[attribute] ");
    print_string(first, last);
    printf(" = ");
    print_scalar(scalar);
    return 1;
}
int dummy_pointer(const char *first, const char *last, const PDS_scalar *scalar, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[pointer  ] ");
    print_string(first, last);
    printf(" = ");
    print_scalar(scalar);
    return 1;
}
int dummy_set_begin(const char *first, const char *last, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[set      ] ");
    print_string(first, last);
    printf(" = {\n");
    data->depth++;
    return 1;
}
int dummy_set_element(const PDS_scalar *scalar, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    print_scalar(scalar);
    return 1;
}
int dummy_set_end(const char *first, const char *last, void *user_data)
{
    (void)first;
    (void)last;
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf("}\n");
    return 1;
}
int dummy_sequence_begin(const char *first, const char *last, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[sequence ] ");
    print_string(first, last);
    printf(" = (\n");
    data->depth++;
    return 1;
}
int dummy_sequence_element(const PDS_scalar *scalar, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    print_scalar(scalar);
    return 1;
}
int dummy_sequence_end(const char *first, const char *last, void *user_data)
{
    (void)first;
    (void)last;
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf(")\n");
    return 1;
}
int dummy_object_begin(const char *first, const char *last, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[object   ] ");
    print_string(first, last);
    printf(" = [\n");
    data->depth++;
    return 1;
}
int dummy_object_end(const char *first, const char *last, void *user_data)
{
    (void)first;
    (void)last;
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf("]\n");
    return 1;
}
int dummy_group_begin(const char *first, const char *last, void *user_data)
{
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[group    ] ");
    print_string(first, last);
    printf(" = [\n");
    data->depth++;
    return 1;
}
int dummy_group_end(const char *first, const char *last, void *user_data)
{
    (void)first;
    (void)last;
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf("]\n");
    return 1;
}

int main(int argc, const char* argv[])
{
    PDS_parser parser;

    FILE *in;
    size_t len;
    char *buffer;

    int ret;

    user_data_t user_data;

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
    
    user_data.depth = 0;
    
    PDS_set_error_callback(&parser, dummy_error);
    PDS_set_attribute_callback(&parser, dummy_attribute);
    PDS_set_pointer_callback(&parser, dummy_pointer);
    PDS_set_set_callbacks(&parser, dummy_set_begin, dummy_set_element, dummy_set_end);
    PDS_set_sequence_callbacks(&parser, dummy_sequence_begin, dummy_sequence_element, dummy_sequence_end);
    PDS_set_group_callbacks(&parser, dummy_group_begin, dummy_group_end);
    PDS_set_object_callbacks(&parser, dummy_object_begin, dummy_object_end);

    ret = PDS_parse(&parser, buffer, len, &user_data);

    free(buffer);
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
