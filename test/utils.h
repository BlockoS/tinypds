#ifndef UTILS_H
#define UTILS_H

#include <tinypds.h>

void print_string(const char *first, const char *last);
void print_scalar(const PDS_scalar *scalar);

int compare_scalar(const PDS_scalar *s0, const PDS_scalar *s1);

#endif // UTILS_H
