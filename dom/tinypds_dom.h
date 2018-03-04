/* 
 * Tiny PDS3 parser
 *
 * This file provides both the interface and the implementation.
 * To instantiate the implementation add the following in *ONE* source file before including this file 
 *     #define TINY_PDS_DOM_IMPL
 *
 * Licensed under the MIT License
 * (c) 2016-2018 Vincent Cruz
 */
#ifndef TINY_PDS_DOM_H
#define TINY_PDS_DOM_H

#include <tinypds.h>

#ifdef __cplusplus
extern "C" {
#endif

/** PDS item type **/
typedef enum {
    /** 
     * PDS attribute. 
     * An attribute can contain a scalar, a set, a one dimensional or
     * 2 dimensional sequence.
     */
    PDS_ATTRIBUTE = 0,
    /**
     * PDS pointer.
     * An attribute can contain a scalar, a set, a one dimensional or
     * 2 dimensional sequence.
     */
    PDS_POINTER,
    /**
     * PDS object.
     * An object can contains attributes, pointers or objects.
     */
    PDS_OBJECT,
    /**
     * PDS group.
     * A group contains only attributes or pointers.
     */
    PDS_GROUP
} PDS_type;

/** PDS scalar type **/
typedef enum {
    /** 
     * No scalar is associated to current item. This is generally the 
     * case for objects and groups.
     */
    PDS_NONE,
    /** Single scalar. **/
    PDS_SINGLE,
    /** Scalar set. **/
    PDS_SET,
    /** One dimensional scalar sequence. **/
    PDS_SEQUENCE_1D,
    /** Two dimensional scalar sequence. **/
    PDS_SEQUENCE_2D
} PDS_scalar_type;

/** PDS item **/
typedef struct {
    /** Name **/
    PDS_string name;
    /** Item type. **/
    PDS_type   type;
} PDS_item;

#define PDS_DOM_typeof(pds)       ((pds)->type)
#define PDS_DOM_is_attribute(pds) ((NULL != pds) && (PDS_ATTRIBUTE == (pds)->type))
#define PDS_DOM_is_pointer(pds)   ((NULL != pds) && (PDS_POINTER == (pds)->type))
#define PDS_DOM_is_object(pds)    ((NULL != pds) && (PDS_OBJECT == (pds)->type))
#define PDS_DOM_is_group(pds)     ((NULL != pds) && (PDS_GROUP == (pds)->type))
#define PDS_DOM_has_scalar(pds)   ((NULL != pds) && ((PDS_ATTRIBUTE == (pds)->type) || (PDS_POINTER == (pds)->type)))

/**
 * Returns the type of the scalar attribute of current item.
 * The item must be an attribute or a pointer.
 * @param [in] pds PDS item.
 * @return scalar type or PDS_NONE if the item is neither an attribute
 *         nor a pointer.
 */
PDS_scalar_type PDS_DOM_scalar_typeof(PDS_item *pds);
/**
 * Returns the total number of scalars.
 * @param [in] pds PDS item.
 * @return total number of scalars or 0 if the item is neither an attribute
 *         nor a pointer.
 */
int PDS_DOM_scalar_count(PDS_item *pds);
/**
 * Gets the scalar associated to current item.
 * The item must be an attribute or a pointer containing a single scalar
 * value.
 * @param [in]  pds    PDS item.
 * @param [out] scalar Scalar value.
 * @return 1 if the item contains a single scalar, 0 otherwise.
 */
int PDS_DOM_scalar_get(PDS_item *pds, PDS_scalar *scalar);
/**
 * Gets the scalar at specified position in the set associated to the
 * current item, with bounds checking.
 * The item must be an attribute or a pointer containing a set of scalar
 * values.
 * @param [in]  pds    PDS item.
 * @param [out] scalar Scalar value.
 * @param [in]  i      Position of the scalar in the set.
 * @return 1 upon success, 0 otherwise.
 */
int PDS_DOM_set_get(PDS_item *pds, PDS_scalar *scalar, int i);
/**
 * Gets the scalar at specified position in the 1 dimensional sequence
 * associated to the current item, with bounds checking.
 * The item must be an attribute or a pointer containing a 1 dimensional
 * sequence of scalar values.
 * @param [in]  pds    PDS item.
 * @param [out] scalar Scalar value.
 * @param [in]  i      Position of the scalar in the 1D sequence.
 * @return 1 upon success, 0 otherwise.
 */
int PDS_DOM_sequence1d_get(PDS_item *pds, PDS_scalar *scalar, int i);
/**
 * Gets the scalar at specified position in the 2 dimensional sequence
 * associated to the current item, with bounds checking.
 * The item must be an attribute or a pointer containing a 2 dimensional
 * sequence of scalar values.
 * @param [in]  pds    PDS item.
 * @param [out] scalar Scalar value.
 * @param [in]  i      Row index.
 * @param [in]  j      Column index.
 * @return 1 upon success, 0 otherwise.
 */
int PDS_DOM_sequence2d_get(PDS_item *pds, PDS_scalar *scalar, int i, int j);
/**
 * Returns the number of rows.
 * The item must be an attribute or a pointer, and the associated scalar
 * must be a 2d sequence.
 * @param [in] pds PDS item.
 * @return number of rows for current 2d sequence or -1 if the item is not
 *         an attribute or a pointer and if the associated scalar is not
 *         a 2d sequence.
 */
int PDS_DOM_sequence2d_rows(PDS_item *pds);
/**
 * Returns the number of columns for the specified row.
 * The item must be an attribute or a pointer, and the associated scalar
 * must be a 2d sequence.
 * @param [in] pds PDS item.
 * @param [in] i   Row index.
 * @return number of columns for the ith row of current 2d sequence or -1 if the item
 *         is not an attribute or a pointer and if the associated scalar
 *         is not a 2d sequence.
 */
int PDS_DOM_sequence2d_cols(PDS_item *pds, int i);

/**
 * Get the first sibbling of current PDS item.
 * @param [in] pds PDS item.
 * @return pointer to sibling entity or NULL if the current PDS item
 *         does not have any sibling.
 */
PDS_item* PDS_DOM_sibling(PDS_item* pds);
/**
 * Get the parent of current PDS item.
 * @param [in] pds PDS item.
 * @return pointer to parent PDS item or NULL if the current PDS item
 *         does not have any parent.
 */
PDS_item* PDS_DOM_parent(PDS_item* pds);
/**
 * Returns a pointer to the first item of current object.
 * @param [in] pds PDS item
 * @return pointer to the first item of the current object.
 */
PDS_item* PDS_DOM_object_begin(PDS_item* pds);
/**
 * Returns a pointer to one past the last item of current object. 
 * @param [in] pds PDS item
 * @return pointer to one past the last item of the current object.
 */
PDS_item* PDS_DOM_object_end(PDS_item* pds);

#define PDS_DOM_object_foreach(obj, element) \
for(element=PDS_DOM_object_begin(obj); NULL!=element;  element=PDS_DOM_sibling(element)) 

/**
 * Returns a pointer to the first item of current group.
 * @param [in] pds PDS item
 * @return pointer to the first item of the current group.
 */
PDS_item* PDS_DOM_group_begin(PDS_item* pds);
/**
 * Returns a pointer to one past the last item of current group. 
 * @param [in] pds PDS item
 * @return pointer to one past the last item of the current group.
 */
PDS_item* PDS_DOM_group_end(PDS_item* pds);

#define PDS_DOM_group_foreach(grp, element) \
for(element=PDS_DOM_group_begin(grp); NULL!=element;  element=PDS_DOM_sibling(element)) 

/**
 * Parse string buffer. 
 * @param [in]  buffer  Input string buffer.
 * @param [in]  len     Length of the input string buffer.
 * @param [out] pds     Pointer to the first element of the PDS tree.
 * @param [out] error   Description of the last encountered error.
 * @return 1 if the PDS file was succesfully parser, 0 if an error occured.
 */
int PDS_DOM_parse(const char *buffer, size_t len, PDS_item **pds, PDS_error_description *error);
/**
 * Release object resources.
 * Recursively release any resources used by the current item and its
 * siblings and children.
 * @param [in] pds PDS item.
 */
void PDS_DOM_delete(PDS_item *pds);
/**
 * Defines the way the PDS tree is traversed.
 */
typedef enum {
    /** Only the siblings of the current item are checked. **/
    PDS_ONLY_SIBLINGS = 0,
    /** Only the children of the current item are checked. 
     *  The traversal is not recursive. **/
    PDS_ONLY_CHILDREN,
    /** Recursively checks all children **/
    PDS_CHILDREN_RECURSIVE,
    /** Recursively checks current item and its siblings. **/
    PDS_SIBLINGS_RECURSIVE
} PDS_search_type;
/**
 * Finds the first item which name matches.
 * @param [in] name Name we are looking for.
 * @param [in] current Current item.
 * @param [in] search Search type.
 * @param [in] check_current Check current item if not 0.
 * @return A pointer to the first matching item or NULL if no match was found.
 */
PDS_item* PDS_DOM_find(const char *name, PDS_item *current, PDS_search_type search, int check_current);

#ifdef __cplusplus
}
#endif

#endif /* TINY_PDS_DOM_H */

/*****************************************************************************/
#ifdef TINY_PDS_DOM_IMPL

#if defined(PDS_DOM_MALLOC) && defined(PDS_DOM_REALLOC) && defined(PDS_DOM_FREE)
// ok
#elif !defined(PDS_DOM_MALLOC) && !defined(PDS_DOM_REALLOC) && !defined(PDS_DOM_FREE)
// ok
#else
#error "Must define all or none of PDS_DOM_MALLOC, PDS_DOM_REALLOC and PDS_DOM_FREE"
#endif

#if !defined(PDS_DOM_MALLOC)
#include <stdlib.h>
#define PDS_DOM_MALLOC(sz)         malloc(sz)
#define PDS_DOM_REALLOC(p, new_sz) realloc(p, new_sz)
#define PDS_DOM_FREE(p)            free(p)
#endif

#if defined(PDS_DOM_MEMCPY) && defined(PDS_DOM_MEMSET)
// ok
#elif !defined(PDS_DOM_MEMCPY) && !defined(PDS_DOM_MEMSET)
// ok
#else
#error "Must define all or none of PDS_DOM_MEMCPY and PDS_DOM_MEMSET"
#endif

#if !defined(PDS_DOM_MEMCPY)
#include <string.h>
#define PDS_DOM_MEMCPY(dst, src, sz) memcpy(dst, src, sz)
#define PDS_DOM_MEMSET(dst, c, sz) memset(dst,c, sz)
#endif

/**
 * ELF hash.
 * @param [in] key  Input string.
 * @param [in] len  Length of the input string.
 * @return 32 bits hash of the input key.
 */
static uint32_t PDS_hash(const char* key, size_t len) {
    const uint8_t *in = (const uint8_t*)key;
    
    size_t  i;
    uint32_t h;
    
    for(i=0, h=0; i<len; i++) {
        uint32_t g;
        h = (h << 4) + in[i];
        g = h & 0xf0000000;
        if(g) {
            h ^= g >> 24;
        }
        h &= ~g;
    }
    return h;
}
/** Hash table bucket. **/
typedef struct {
    /** Hash of the key. **/
    uint32_t hash;
    /** Key. **/
    const char *key;
    /** Key length. **/
    size_t len;
    /** Index in the data buffer of the associated PDS scalar. **/
    uint32_t index;
} PDS_bucket;

#define PDS_HTAB_INITIAL_BUCKET_COUNT 8

/**
 * Opaque hash table structure.
 */
typedef struct {
    /** Hash table buckets. **/
    PDS_bucket *buckets;
    /** Capacity (number of allocated entries). **/
    uint32_t capacity;
    /** Number of used entries. **/ 
    uint32_t used;
} PDS_htab;
/**
 * Compute probing distance.
 */
static int PDS_htab_distance(PDS_htab *tab, uint32_t pos) {
    if(NULL == tab->buckets[pos].key) {
        return -1;
    }
    uint32_t original = tab->buckets[pos].hash % tab->capacity;
    return (tab->capacity + pos - original) % tab->capacity;
}
/**
 * Creates an empty hash table.
 * @param [in][out] tab Pointer to the hash table to be destroyed.
 * @return 1 upon success, 0 if an error occured.
 */
static int PDS_htab_create(PDS_htab* tab) {
    if(NULL == tab) {
        return 0;
    }
    tab->capacity = PDS_HTAB_INITIAL_BUCKET_COUNT;
    tab->used = 0;
    tab->buckets = (PDS_bucket*)PDS_DOM_MALLOC(tab->capacity * sizeof(PDS_bucket));
    if(NULL == tab->buckets) {
        return 0;
    }
    PDS_DOM_MEMSET(tab->buckets, 0, tab->capacity * sizeof(PDS_bucket));
    return 1; 
}
/**
 * Releases memory used by a hash table.
 * @param [in] tab Pointer to the hash table to be destroyed.
 */
static void PDS_htab_destroy(PDS_htab* tab) {
    if(NULL == tab) {
        return;
    }
    if(tab->buckets) {
        PDS_DOM_FREE(tab->buckets);
        tab->buckets = NULL;
    }
    tab->capacity = 0;
    tab->used = 0;
}
/**
 * Retrieves the index of the bucket associated to a given key.
 * @param [in] tab Hash table.
 * @param [in] hash Entry hash.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return index of the matching bucket or UINT32_MAX if there is no matching entry.
 */
static uint32_t PDS_htab_index(PDS_htab* tab, uint32_t hash, const char* key, int len) {
    uint32_t i;
    for(i=0; i<tab->capacity; i++) {
        uint32_t pos = (hash + i) % tab->capacity;
        int distance = PDS_htab_distance(tab, pos);
        if((NULL == tab->buckets[pos].key) || ((int)i > distance)) {
            return UINT32_MAX;
        }
        else if(PDS_string_compare(key, key+len, tab->buckets[pos].key, tab->buckets[pos].key+tab->buckets[pos].len)) {
            return pos;
        }
    }
    return UINT32_MAX;
}
/**
 * Retrieves the index of the PDS item associated to a given key.
 * @param [in] tab Hash table.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return index of the PDS item associated to the key or UINT32_MAX if the entry was not found.
 */ 
static uint32_t PDS_htab_get(PDS_htab* tab, const char* key, size_t len)
{
    uint32_t hash = PDS_hash(key, len);
    uint32_t pos = PDS_htab_index(tab, hash, key, len);
    if(UINT32_MAX == pos)
    {
        return UINT32_MAX;
    }
    return tab->buckets[pos].index;
}
/**
 * Insert bucket into the hash table.
 */
static void PDS_htab_insert(PDS_htab* tab, PDS_bucket* bucket) {
    int probe = 0;
    uint32_t i;
    
    uint32_t pos = bucket->hash % tab->capacity;
    for(i=0; i<tab->capacity; i++, probe++, pos=(pos+1)%tab->capacity) {
        if(tab->buckets[pos].key) {
            int distance = PDS_htab_distance(tab, pos);
            if(probe > distance) {
                PDS_bucket tmp = tab->buckets[pos];
                tab->buckets[pos] = *bucket;
                *bucket = tmp;
                probe = distance;
            }
        }
        else {
            tab->buckets[pos] = *bucket;
            break;
        }
    }
}
/**
 * Expand hash table.
 */
static int PDS_htab_grow(PDS_htab* tab) {
    PDS_bucket* buckets;
    uint32_t capacity;
    
    uint32_t old_capacity = tab->capacity;
    PDS_bucket* old_buckets = tab->buckets;

    capacity = tab->capacity * 2;
    buckets = (PDS_bucket*)PDS_DOM_MALLOC(capacity * sizeof(PDS_bucket));
    if(NULL == buckets) {
        return 0;
    }
    PDS_DOM_MEMSET(buckets, 0, capacity * sizeof(PDS_bucket));

    tab->capacity = capacity;
    tab->buckets = buckets;
    for(uint32_t i=0; i<old_capacity; i++) {
        if(old_buckets[i].key) {
            PDS_htab_insert(tab, &old_buckets[i]);
        }
    }
    free(old_buckets);
    return 1;
}
/**
 * Adds a new entry to the hash table.
 * @param [in] tab  Hash table.
 * @param [in] key  Entry key.
 * @param [in] len  Length of the entry key.
 * @param [in] item [todo]
 * @return [todo]
 */
static int PDS_htab_add(PDS_htab* tab, const char* key, size_t len, uint32_t item) {
    PDS_bucket bucket;
    bucket.key   = key;
    bucket.len   = len;
    bucket.hash  = PDS_hash(key, len);
    bucket.index = item;
    
    if(tab->used == tab->capacity) {
        if(0 == PDS_htab_grow(tab)) {
            return 0;
        }
    }
    tab->used++;
    PDS_htab_insert(tab, &bucket);
    return 1;
}

/**
 *
 */
static int PDS_htab_del(PDS_htab* tab, const char* key, size_t len) {
    uint32_t i;
    uint32_t hash = PDS_hash(key, len);
    uint32_t pos = PDS_htab_index(tab, hash, key, len);
    if(UINT32_MAX == pos) {
        return 0;
    }

    PDS_DOM_MEMSET(&tab->buckets[pos], 0, sizeof(PDS_bucket));
    
    tab->used--;
    for(i=0; i<tab->capacity; i++) {
        uint32_t prev = pos;
        pos = (pos+1) % tab->capacity;
        if(NULL == tab->buckets[pos].key) {
            break;
        }
        int distance = PDS_htab_distance(tab, pos);
        if(0 == distance) {
            break;
        }
        PDS_DOM_MEMCPY(&tab->buckets[prev], &tab->buckets[pos], sizeof(PDS_bucket));
        PDS_DOM_MEMSET(&tab->buckets[pos], 0, sizeof(PDS_bucket));
    }
    
    pos = PDS_htab_index(tab, hash, key, len);

    return 1;
}

#if 0
typedef struct PDS_item_impl_t {
    PDS_item info;

    PDS_htab htab;

    PDS_scalar_type scalar_type;
    PDS_scalar *scalar;
    int *last;
    int count;
    
    struct PDS_item_impl_t *parent;
    struct PDS_item_impl_t *child;
    
    struct PDS_item_impl_t *sibling;
    
    struct PDS_item_impl_t *next;
} PDS_item_impl;

typedef struct {
    PDS_item_impl *root;
    PDS_item_impl *parent;
    PDS_item_impl *current;
    PDS_item_impl *previous;
    int dimension;
    PDS_error_description error;
} PDS_DOM_payload;

/* Create new PDS item */
static PDS_item_impl* PDS_DOM_create(PDS_type type, const char *first, const char *last) {
    PDS_item_impl *item;
    item = PDS_DOM_MALLOC(sizeof(PDS_item_impl));
    if(NULL == item) {
        return NULL;
    }
    
    item->info.name.type  = PDS_TEXT_STRING_VALUE;
    item->info.name.first = first;
    item->info.name.last  = last;
    
    item->info.type = type;

    item->scalar_type = PDS_NONE;
    item->scalar = NULL;
    item->last   = NULL;
    item->count  = 0;
    
    item->child   = NULL;
    item->sibling = NULL;
    item->next    = NULL;

    item->parent = NULL;
    return item;
}

/* Create a new PDS_item and add it to the hierarchy. */
static int PDS_DOM_add(PDS_DOM_payload *payload, PDS_type type, const char *first, const char *last)
{
    PDS_item_impl *item = PDS_DOM_create(type, first, last);
    if(NULL == item)
    {
        return PDS_INVALID_VALUE;
    }

    item->parent = payload->parent;
    if( (PDS_GROUP == type) || (PDS_OBJECT == type) )
    {
        payload->parent = item;
    }
    if(payload->current)
    {
        payload->current->sibling = item;
    }
    payload->current = item;

    if(NULL == payload->root)
    {
        payload->root = item;
    }
    
    if(payload->previous)
    {
        payload->previous->next = item;
    }
    payload->previous = item;
    
    return PDS_OK;
}
/* Scalar callback */
static int PDS_DOM_parse_scalar(const PDS_scalar *scalar, void *user_data)
{
    int index;
    int last;
    PDS_scalar *tmp;
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    /* Sanity check */
    if(NULL == payload)
    {
        return 0;
    }
    if(NULL == payload->current)
    {
        return 0;
    }
    if((PDS_ATTRIBUTE != payload->current->info.type) && (PDS_POINTER != payload->current->info.type))
    {
        return 0;
    }
    /* Copy scalar */
    index = payload->current->count-1;
    last  = payload->current->last[index];
    tmp = PDS_DOM_REALLOC(payload->current->scalar, (last+1)*sizeof(PDS_scalar));
    if(NULL == tmp)
    {
        return 0;
    }
    payload->current->scalar = tmp;
    payload->current->last[index]++;
    PDS_DOM_MEMCPY(&payload->current->scalar[last], scalar, sizeof(PDS_scalar));
    return 1;
}
/* Set start callback. */
static int PDS_DOM_parse_set_begin(void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    /* Sanity check */
    if(NULL == payload)
    {
        return 0;
    }
    if(NULL == payload->current)
    {
        return 0;
    }
    if((PDS_ATTRIBUTE != payload->current->info.type) && (PDS_POINTER != payload->current->info.type))
    {
        return 0;
    }
    if(payload->dimension)
    {
        return 0;
    }
    payload->current->scalar_type = PDS_SET;
    payload->dimension = 1;
    return 1;
}
/* Set end callback. */
static int PDS_DOM_parse_set_end(void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    /* Sanity check */
    if(NULL == payload)
    {
        return 0;
    }
    if(NULL == payload->current)
    {
        return 0;
    }
    payload->dimension = 0;
    return 1;
}
/* Sequence start callback. */
static int PDS_DOM_parse_sequence_begin(void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    /* Sanity check */
    if(NULL == payload)
    {
        return 0;
    }
    if(NULL == payload->current)
    {
        return 0;
    }
    if((PDS_ATTRIBUTE != payload->current->info.type) && (PDS_POINTER != payload->current->info.type))
    {
        return 0;
    }
    /* Check that the sequence is either 1D or 2D */
    if(payload->dimension >= 2)
    {
        return 0;
    }
    if(payload->dimension == 1)
    {
        int *tmp;
        int index = payload->current->count;
        tmp = PDS_DOM_REALLOC(payload->current->last, (index+1)*sizeof(int));
        if(NULL == tmp)
        {
            return 0;
        }
        payload->current->last[index] = index ? payload->current->last[index-1] : 0;
        payload->current->count++;
        payload->current->scalar_type = PDS_SEQUENCE_2D;
    }
    else if(PDS_SINGLE == payload->current->scalar_type)
    {
        payload->current->scalar_type = PDS_SEQUENCE_1D;
    }
    payload->dimension++;
    return 1;
}
/* Sequence end callback. */
static int PDS_DOM_parse_sequence_end(void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    /* Sanity check */
    if(NULL == payload)
    {
        return 0;
    }
    if(NULL == payload->current)
    {
        return 0;
    }
    payload->dimension--;
    return 1;
}
/* Attribute callback */
static int PDS_DOM_parse_attribute_begin(const char *first, const char *last, void *user_data)
{
    int ret;
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    ret = PDS_DOM_add(payload, PDS_ATTRIBUTE, first, last); 
    if(PDS_OK != ret)
    {
        return 0;
    }
    payload->current->count = 1;
    payload->current->last = PDS_DOM_MALLOC(sizeof(int));
    if(NULL == payload->current->last)
    {
        return 0;
    }
    payload->current->last[0] = 0;
    payload->current->scalar_type = PDS_SINGLE;
    return 1;
}
static int PDS_DOM_parse_attribute_end(const char *first, const char *last, void *user_data)
{
    (void)first;
    (void)last;
    (void)user_data;
    return 1;
}
/* Pointer callback */
static int PDS_DOM_parse_pointer_begin(const char *first, const char *last, void *user_data)
{
    int ret;
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    ret = PDS_DOM_add(payload, PDS_POINTER, first, last); 
    if(PDS_OK != ret)
    {
        return 0;
    }
    payload->current->count = 1;
    payload->current->last = PDS_DOM_MALLOC(sizeof(int));
    if(NULL == payload->current->last)
    {
        return 0;
    }
    payload->current->last[0] = 0;
    payload->current->scalar_type = PDS_SINGLE;
    return 1;
}
static int PDS_DOM_parse_pointer_end(const char *first, const char *last, void *user_data)
{
    (void)user_data;
    (void)first;
    (void)last;
    return 1;
}

/* Group start callback. */
static int PDS_DOM_parse_group_begin(const char *first, const char *last, void *user_data)
{
    int ret;
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    ret = PDS_DOM_add(payload, PDS_GROUP, first, last); 
    return (PDS_OK == ret);
}
/* Group end callback. */
static int PDS_DOM_parse_group_end(const char *first, const char *last, void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }

    payload->current->sibling = NULL;
    payload->current = payload->parent;

    if(payload->parent)
    {
        payload->parent->child = payload->parent->sibling;
        payload->parent->sibling = NULL;
        payload->parent = payload->parent->parent;
    }

    (void)first;
    (void)last;

    return 1;
}

/* Object start callback. */
static int PDS_DOM_parse_object_begin(const char *first, const char *last, void *user_data)
{
    int ret;
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }
    ret = PDS_DOM_add(payload, PDS_OBJECT, first, last); 
    return (PDS_OK == ret);
}
/* Object end callback. */
static int PDS_DOM_parse_object_end(const char *first, const char *last, void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return 0;
    }

    payload->current->sibling = NULL;
    payload->current = payload->parent;

    if(payload->parent)
    {
        payload->parent->child = payload->parent->sibling;
        payload->parent->sibling = NULL;
        payload->parent = payload->parent->parent;
    }

    (void)first;
    (void)last;

    return 1;
}
/* Error callback. */
static void PDS_DOM_error(const PDS_error_description* desc, void *user_data)
{
    PDS_DOM_payload *payload = (PDS_DOM_payload*)user_data;
    if(NULL == payload)
    {
        return;
    }
    payload->error = *desc;
}

/* Parse string buffer. */
int PDS_DOM_parse(const char *buffer, size_t len, PDS_item **item, PDS_error_description *error)
{
    PDS_callbacks callbacks;
    PDS_DOM_payload payload;
    
    int ret;
    
    /* Sanity check */
    if( NULL == error )
    {
        return 0;
    }
    if( (NULL == buffer) || (0 == len) || (NULL == item) )
    {
        error->line = 0;
        error->msg = "invalid parameters";
        return 0;
    }
    
    /* Set tinypds callbacks */
    PDS_set_attribute_callbacks(&callbacks, PDS_DOM_parse_attribute_begin,
                                            PDS_DOM_parse_attribute_end);
    PDS_set_pointer_callbacks(&callbacks, PDS_DOM_parse_pointer_begin,
                                          PDS_DOM_parse_pointer_end);
    PDS_set_set_callbacks(&callbacks, PDS_DOM_parse_set_begin,
                                      PDS_DOM_parse_set_end);
    PDS_set_sequence_callbacks(&callbacks, PDS_DOM_parse_sequence_begin,
                                           PDS_DOM_parse_sequence_end);
    PDS_set_group_callbacks(&callbacks, PDS_DOM_parse_group_begin,
                                        PDS_DOM_parse_group_end);
    PDS_set_object_callbacks(&callbacks, PDS_DOM_parse_object_begin,
                                         PDS_DOM_parse_object_end);
    PDS_set_scalar_callback(&callbacks, PDS_DOM_parse_scalar);
    PDS_set_error_callback(&callbacks, PDS_DOM_error);
    
    payload.root = payload.parent = NULL;
    payload.current = payload.previous = NULL;
    
    payload.dimension = 0;
    
    *item = NULL;
 
    payload.error.line     = 0;
    payload.error.number   = 0;
    payload.error.position = 0;
    payload.error.status   = PDS_OK;
    payload.error.msg      = "ok";
 
    /* Parse buffer */
    ret = PDS_parse(&callbacks, buffer, len, &payload);
    *error = payload.error;
    if(!ret)
    {
        PDS_DOM_delete((PDS_item*)payload.root);
        return ret;
    }
    
    *item = (PDS_item*)payload.root;
    return ret;
}

/* Release object resources. */
void PDS_DOM_delete(PDS_item *pds)
{
    PDS_item_impl *it;
    PDS_item_impl *end;
    if(NULL == pds)
    {
        return;
    }
    
    it = (PDS_item_impl*)pds;
    if(it->parent)
    {
        end = it->parent->sibling;
        it->parent->child = NULL;
        it->parent->next  = end;
    }
    else
    {
        end = NULL;
    }
    
    while(it != end)
    {
        PDS_item_impl *obj = it;
        it = it->next;
        
        if(obj->scalar)
        {
            PDS_DOM_FREE(obj->scalar);
        }
        if(obj->last)
        {
            PDS_DOM_FREE(obj->last);
        }
        PDS_DOM_FREE(obj);
    }
}
/* Get the type of the scalar attribute of current item. */
PDS_scalar_type PDS_DOM_scalar_typeof(PDS_item *pds)
{
    return PDS_DOM_has_scalar(pds) ? ((PDS_item_impl*)pds)->scalar_type : PDS_NONE;
}
/* Returns the total number of scalars. */
int PDS_DOM_scalar_count(PDS_item *pds)
{
    if(PDS_DOM_has_scalar(pds))
    {
        PDS_item_impl *item = (PDS_item_impl*)pds;
        return (item->count) ? item->last[item->count-1] : 0;
    }
    return 0; 
}
/* Gets the scalar associated to current item at the given row and column. */
static int PDS_DOM_scalar_get_internal(PDS_item *pds, PDS_scalar *scalar, PDS_scalar_type type, int i, int j)
{
    PDS_item_impl *item = (PDS_item_impl*)pds;
    int count = PDS_DOM_scalar_count(pds);
    int offset;

    scalar->type = PDS_UNKNOWN_VALUE;
    if( (type != PDS_DOM_scalar_typeof(pds)) || (0 == count) )
    {
        return 0;
    }
    
    if(i >= item->count)
    {
        return 0;
    }
    
    offset = (i ? item->last[i-1] : 0) + j;
    if(offset > item->last[i])
    {
        return 0;
    }
    PDS_DOM_MEMCPY(scalar, item->scalar + offset, sizeof(PDS_scalar));
    return 1;
}
/* Gets the scalar associated to current item. */
int PDS_DOM_scalar_get(PDS_item *pds, PDS_scalar *scalar)
{
    return PDS_DOM_scalar_get_internal(pds, scalar, PDS_SINGLE, 0, 0);
}
/* Gets the scalar at specified position in the set associated to the
 * current item, with bounds checking. */
int PDS_DOM_set_get(PDS_item *pds, PDS_scalar *scalar, int i)
{
    return PDS_DOM_scalar_get_internal(pds, scalar, PDS_SET, 0, i);
}
/* Gets the scalar at specified position in the 1 dimensional sequence
 * associated to the current item, with bounds checking. */
int PDS_DOM_sequence1d_get(PDS_item *pds, PDS_scalar *scalar, int i)
{
    return PDS_DOM_scalar_get_internal(pds, scalar, PDS_SEQUENCE_1D, 0, i);
}
/* Gets the scalar at specified position in the 2 dimensional sequence
 * associated to the current item, with bounds checking. */
int PDS_DOM_sequence2d_get(PDS_item *pds, PDS_scalar *scalar, int i, int j)
{
    return PDS_DOM_scalar_get_internal(pds, scalar, PDS_SEQUENCE_2D, i, j);
}
/* Returns the number of rows. */
int PDS_DOM_sequence2d_rows(PDS_item *pds)
{
    if(PDS_SEQUENCE_2D != PDS_DOM_scalar_typeof(pds))
    {
        return -1;
    }
    return ((PDS_item_impl*)pds)->count;
}
/* Returns the number of columns for the specified row. */
int PDS_DOM_sequence2d_cols(PDS_item *pds, int i)
{
    PDS_item_impl *item = (PDS_item_impl*)pds;
    if(PDS_SEQUENCE_2D != PDS_DOM_scalar_typeof(pds))
    {
        return -1;
    }
    if((i >= item->count) || (0 == item->count))
    {
        return 0;
    }
    return item->last[i] - ((i > 0) ? item->last[i-1] : 0);
}
/* Get the sibbling of current PDS object. */
PDS_item* PDS_DOM_sibling(PDS_item* pds)
{
    PDS_item_impl *current = (PDS_item_impl*)pds;
    return (NULL != current) ? (PDS_item*)(current->sibling) : NULL;
}
/* Get the parent of current PDS object. */
PDS_item* PDS_DOM_parent(PDS_item* pds)
{
    PDS_item_impl *current = (PDS_item_impl*)pds;
    return (NULL != current) ? (PDS_item*)(current->parent) : NULL;
}
/* Returns a pointer to the first item of current object. */
PDS_item* PDS_DOM_object_begin(PDS_item* pds)
{
    return (PDS_item*)(PDS_DOM_is_object(pds) ? ((PDS_item_impl*)pds)->child : NULL);
}
/* Returns a pointer to one past the last item of current object. */
PDS_item* PDS_DOM_object_end(PDS_item* pds)
{
    if(PDS_DOM_is_object(pds))
    {
        PDS_item_impl* item = (PDS_item_impl*)pds;
        if(NULL != item->child)
        {
            return (PDS_item*)item->sibling;
        }
    }
    return NULL;
}
/* Returns a pointer to the first item of current group. */
PDS_item* PDS_DOM_group_begin(PDS_item* pds)
{
    return (PDS_item*)(PDS_DOM_is_group(pds) ? ((PDS_item_impl*)pds)->child : NULL);
}
/* Returns a pointer to one past the last item of current group. */
PDS_item* PDS_DOM_group_end(PDS_item* pds)
{
    if(PDS_DOM_is_group(pds))
    {
        PDS_item_impl* item = (PDS_item_impl*)pds;
        if(NULL != item->child)
        {
            return (PDS_item*)item->sibling;
        }
    }
    return NULL;
}
/* Finds the first item which name matches. */
PDS_item* PDS_DOM_find(const char *name, PDS_item *current, PDS_search_type search, int check_current)
{
    const char *first;
    const char *last;
    PDS_item_impl *begin = (PDS_item_impl*)current;
    PDS_item_impl *it;
    PDS_item_impl *end;

    /* sanity check */
    if((NULL == name) || (NULL == current))
    {
        return NULL;
    }
    
    first = name;
    last  = name + strlen(name) - 1;

    if(check_current && PDS_string_compare(current->name.first, current->name.last, first, last))
    {
        return current;
    }
    switch(search)
    {
        case PDS_ONLY_SIBLINGS:
            for(it=begin->sibling, end=NULL; (it!=end) && !PDS_string_compare(it->info.name.first, it->info.name.last, first, last); it=it->sibling)
            {}
            break;
        case PDS_ONLY_CHILDREN:
            for(it=begin->child, end=NULL; (it!=end) && !PDS_string_compare(it->info.name.first, it->info.name.last, first, last); it=it->sibling)
            {}
            break;
        case PDS_CHILDREN_RECURSIVE:
            for(it=begin->child, end=begin->sibling; (it!=end) && !PDS_string_compare(it->info.name.first, it->info.name.last, first, last); it=it->next)
            {}
            break;
        case PDS_SIBLINGS_RECURSIVE:
            end = (begin->parent) ? (begin->parent->sibling) : NULL;
            for(it=begin->next; (it!=end) && !PDS_string_compare(it->info.name.first, it->info.name.last, first, last); it=it->next)
            {}
            break;
    }

    return (PDS_item*)((it != end) ? it : NULL);
}
#endif
#endif /* TINY_PDS_DOM_IMPL */
