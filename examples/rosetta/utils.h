/* Random utility functions.
 * Licensed under the MIT License
 * (c) 2016 Vincent Cruz
 */
#ifndef UTILS_H
#define UTILS_H

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

/** Sample type **/
enum PDS_SAMPLE_TYPE
{
    /** Unknown type. */
    PDS_SAMPLE_UNKNOWN_TYPE = 0,
    /** LSB unsigned integer. **/
    PDS_SAMPLE_UINT_LSB,
    /** MSB unsigned integer. **/
    PDS_SAMPLE_UINT_MSB,
    /** LSB integer. **/
    PDS_SAMPLE_INT_LSB,
    /** MSB integer. **/
    PDS_SAMPLE_INT_MSB,
    /** 32 bits IEEE float. **/
    PDS_SAMPLE_FLOAT
};

/** PDS image structure. **/
typedef struct
{
    /** Line count (height). **/
    size_t lines;
    /** Samples per line (width). **/
    size_t samples_per_line;
    /** Sample type (float, uint, etc...). **/
    uint8_t sample_type;
    /** Sample size in bits (bits per pixel). **/
    size_t sample_bits;
    /** Band count (component count). **/
    size_t bands;
} PDS_image;

/**
 * Returns a pointer to the last character of a null terminated string.
 */
const char* str_last(const char *str);

/**
 * Store file content to memory.
 * @param [in]  filename Source filename.
 * @param [out] buffer   Destination buffer.
 * @param [out] len      Bytes stored.
 * @return 1 if the whole file content was succesfully tranfered to the destination buffer.
 *         0 if an error occured.
 */
int buffer_from_file(const char *filename, char **buffer, size_t *len);

/**
 * Convert sample type name to PDS_SAMPLE_TYPE.
 * @param [in] first Pointer to the first character of the type name.
 * @param [in] last  Pointer to the last character of the type name.
 * @return PDS_SAMPLE_TYPE associated to the sample type name, or PDS_SAMPLE_UNKNOWN_TYPE if no type name matches.
 */
uint8_t convert_type_name(const char *first, const char *last);
/**
 * Convert embedded image to FITS.
 * @param [in] filename Output filename.
 * @param [in] buffer   PDS buffer.
 * @param [in] size     PDS buffer size.
 * @param [in] offset   Image data offset in PDS buffer.
 * @param [in] image    PDS image.
 * @return 1 upon success, 0 otherwise.
 */
int write_image(const char *filename, char *buffer, size_t size, size_t offset, PDS_image *image);

#endif /* UTILS_H */
