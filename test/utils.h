#ifndef UTILS_H
#define UTILS_H

#include <pds.h>

typedef struct
{
    int depth;
    int count;
    const char *name;
    const PDS_scalar *scalar;
} expected_t;

typedef struct
{
    int depth;
    int index;
    const expected_t *expected;
} state_t;

void print_string(const char *first, const char *last);
void print_scalar(const PDS_scalar *scalar);

int compare_scalar(const PDS_scalar *s0, const PDS_scalar *s1);

int attribute_callback(const char *first, const char *last, const PDS_scalar *scalar, void *user_data);

int set_begin_callback(const char *first, const char *last, void *user_data);
int set_element_callback(const PDS_scalar *scalar, void *user_data);
int set_end_callback(const char *first, const char *last, void *user_data);

int sequence_begin_callback(const char *first, const char *last, void *user_data);
int sequence_element_callback(const PDS_scalar *scalar, void *user_data);
int sequence_end_callback(const char *first, const char *last, void *user_data);

int group_begin_callback(const char *first, const char *last, void *user_data);
int group_end_callback(const char *first, const char *last, void *user_data);

int object_begin_callback(const char *first, const char *last, void *user_data);
int object_end_callback(const char *first, const char *last, void *user_data);

#endif // UTILS_H
