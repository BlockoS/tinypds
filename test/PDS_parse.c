#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TINY_PDS_IMPL
#include <tinypds.h>

typedef struct {
    int depth;
} user_data_t;

void dummy_error(const PDS_error_description *desc, void *unused) {
    (void)unused;
    fprintf(stderr, "line %d:%d: %s\n", desc->number, desc->position, desc->msg);
}

void print_tab(int depth) {
    int i;
    for(i=0; i<depth; i++) {
        printf("    ");
    }
}

void print_string(const char *first, const char *last) {
    for(; first<=last; first++) {                                                                                                         
        fputc(*first, stdout);                                                                                
    }                                                                                                         
}                                                                                                             
                                                                                                              
void print_scalar(const PDS_scalar *scalar) {
    switch(scalar->type) {
        case PDS_INTEGER_VALUE:                                                                               
            printf("%ld ", scalar->integer.value);
            if(scalar->integer.unit.first && scalar->integer.unit.last) {
                print_string(scalar->integer.unit.first, scalar->integer.unit.last);
            }                                                          
            break;                                                                                            
        case PDS_REAL_VALUE:                                                                                  
            printf("%f ", scalar->real.value);                                                                
            if(scalar->real.unit.first && scalar->real.unit.last) {
                print_string(scalar->real.unit.first, scalar->real.unit.last);
            }                                                          
            break;                                                                                            
        case PDS_DATE_TIME_VALUE:                                                                             
            printf("%04d-%02d-%02d ", scalar->date_time.year, scalar->date_time.month, scalar->date_time.day);
            printf("%02d:%02d:%02d:%02d ", scalar->date_time.hour, scalar->date_time.minute,
                                           scalar->date_time.second, scalar->date_time.microsecond);
            if(PDS_ZONED_TIME == scalar->date_time.time_type) {
                printf("%02d:%02d ", scalar->date_time.hour_offset, scalar->date_time.minute_offset);
            }
            else if(PDS_UTC_TIME == scalar->date_time.time_type) {
                printf("UTC");
            }
            break;                                                                                            
        case PDS_TEXT_STRING_VALUE:                                                                           
            printf("\"");                                                                                 
            print_string(scalar->text.first, scalar->text.last);                                              
            printf("\" ");                                                                                 
            break;                                                                                            
        case PDS_SYMBOLIC_VALUE:                                                                              
            printf("\'");                                                                                 
            print_string(scalar->text.first, scalar->text.last);                                              
            printf("\' ");                                                                                 
            break;                                                                                            
        case PDS_IDENTIFIER_VALUE:                                                                            
            print_string(scalar->identifier.first, scalar->identifier.last);                                  
            break;                                                                                            
    }                                                                                                         
    printf("\n");
}

int dummy_scalar(const PDS_scalar *scalar, void *user_data) {
    (void)user_data;
    print_scalar(scalar);
    return 1;
}

int dummy_attribute_begin(const char *first, const char *last, void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[attribute] ");
    print_string(first, last);
    printf(" = ");
    return 1;
}

int dummy_attribute_end(const char *first, const char *last, void *user_data) {
    (void)first;
    (void)last;
    (void)user_data;
    return 1;
}

int dummy_pointer_begin(const char *first, const char *last, void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[pointer  ] ");
    print_string(first, last);
    printf(" = ");
    return 1;
}

int dummy_pointer_end(const char *first, const char *last, void *user_data) {
    (void)first;
    (void)last;
    (void)user_data;
    return 1;
}

int dummy_set_begin(void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    data->depth++;
    printf(" {\n");
    return 1;
}

int dummy_set_end(void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf("}\n");
    return 1;
}

int dummy_sequence_begin(void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    data->depth++;
    printf("(\n");
    return 1;
}

int dummy_sequence_end(void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf(")\n");
    return 1;
}

int dummy_object_begin(const char *first, const char *last, void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[object   ] ");
    print_string(first, last);
    printf(" = [\n");
    data->depth++;
    return 1;
}

int dummy_object_end(const char *first, const char *last, void *user_data) {
    (void)first;
    (void)last;
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf("]\n");
    return 1;
}

int dummy_group_begin(const char *first, const char *last, void *user_data) {
    user_data_t *data = (user_data_t*)user_data;
    print_tab(data->depth);
    printf("[group    ] ");
    print_string(first, last);
    printf(" = [\n");
    data->depth++;
    return 1;
}

int dummy_group_end(const char *first, const char *last, void *user_data) {
    (void)first;
    (void)last;
    user_data_t *data = (user_data_t*)user_data;
    data->depth--;
    print_tab(data->depth);
    printf("]\n");
    return 1;
}

int main(int argc, const char* argv[]) {
    PDS_callbacks callbacks;

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
    
    PDS_set_error_callback(&callbacks, dummy_error);
    PDS_set_scalar_callback(&callbacks, dummy_scalar);
    PDS_set_attribute_callbacks(&callbacks, dummy_attribute_begin, dummy_attribute_end);
    PDS_set_pointer_callbacks(&callbacks, dummy_pointer_begin, dummy_pointer_end);
    PDS_set_set_callbacks(&callbacks, dummy_set_begin, dummy_set_end);
    PDS_set_sequence_callbacks(&callbacks, dummy_sequence_begin, dummy_sequence_end);
    PDS_set_group_callbacks(&callbacks, dummy_group_begin, dummy_group_end);
    PDS_set_object_callbacks(&callbacks, dummy_object_begin, dummy_object_end);

    ret = PDS_parse(&callbacks, buffer, len, &user_data);

    free(buffer);
    return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
