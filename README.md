# tinypds & tinypds_dom : small PDS 3 parsers
[![Travis build Status](https://travis-ci.org/BlockoS/tinypds.svg)](https://travis-ci.org/BlockoS/tinypds)    [![Coverity Scan Build Status](https://scan.coverity.com/projects/8523/badge.svg)](https://scan.coverity.com/projects/blockos-tinypds)

`tinypds` is a small and simple event-driven single header-only library PDS 3 parser.
`tinypds_dom` is a small and simple DOM-like single header-only library PDS 3 parser.

`tinypds` and `tinypds_dom` written in C with little or no dependencies.

In order to use `tinypds` in your project, just copy `tinypds.h` into your project.
Note that this file contain both the interface and the implementation.
To instantiate the implementation of `tinypds` add the following define in *ONE* source file before including `tinypds.h`:
```c
#define TINY_PDS_IMPL
```

`tinypds_dom` depends on `tinypds`. Just like `tinypds`, copy the `tinypds_dom.h` fil into your project.
This file contains both the interface and the implementation. This means that you must define the following variable
in *ONE* source file before including `tinypds_dom.h`:
```c
#define TINY_PDS_DOM_IMPL
```

## Build ##
A CMake configuration file is provided in order to build a static library, documentation or run a bundle of tests.
A typical usage of CMake may be:
```bash
mkdir build
cd build
cmake ..
make
```
On a Linux system, the Makefile will generate the static libraries `libtinypds.a`, `libtinypds_dom.a` and stand-alone unit test binaries. 

`make test` will run the unit tests and generate a report with `CTest`.
  
`make doc` will generate the documentation with [DoxyGen](http://www.stack.nl/~dimitri/doxygen/).

## Usage - tinypds ##
`tinypds` works on raw ASCII buffer. PDS file reading is left to the user.

The `tinypds` events and associated callbacks are : 
* **scalar attribute statement** 
  ```c
  int attribute_begin(const char *first, const char *last, void *user_data)
  ```
  ```c
  int attribute_end(const char *first, const char *last, void *user_data)
  ```
  * **first** and **last** are pointers to the first and last characters of the **attribute name**.
  * **user_data** is a pointer to the user data.

* **pointer statement**
  ```c
  int pointer_begin(const char *first, const char *last, void *user_data)
  ```
  ```c
  int pointer_end(const char *first, const char *last, void *user_data)
  ```
  * **first** and **last** are pointers to the first and last characters of the **pointer name**.
  * **user_data** is a pointer to the user data.

* **single scalar value**
  ```c
  int scalar(const PDS_scalar *value, void *user_data)
  ```
  * **value** is a pointer to the current scalar value.
  * **user_data** is a pointer to the user data.

* **sequence attribute start**
  ```c
  int sequence_begin(void *user_data)
  ```
  * **user_data** is a pointer to the user data.

* **sequence attribute element**
  ```c
  int sequence_element(const PDS_scalar *scalar, void *user_data)
  ```
  * **scalar** is the current **sequence element** value.
  * **user_data** is a pointer to the user data.

* **sequence attribute end**
  ```c
  int sequence_end(void *user_data)
  ```
  * **user_data** is a pointer to the user data.

* **set attribute start**
  ```c
  int set_begin(void *user_data)
  ```
  * **user_data** is a pointer to the user data.

* **set attribute element**
  ```c
  int set_element(const PDS_scalar *scalar, void *user_data)
  ```
  * **scalar** is the current **set element** value.
  * **user_data** is a pointer to the user data.

* **set attribute end**
  ```c
  int set_end(void *user_data)
  ```
  * **user_data** is a pointer to the user data.

* **object declaration start**
  ```c
  int object_begin(const char *first, const char *last, void *user_data)
  ```
  * **first** and **last** are pointers to the first and last characters of the **object name**.
  * **user_data** is a pointer to the user data.

* **object declaration end**
  ```c
  int object_end(const char *first, const char *last, void *user_data)
  ```
  * **first** and **last** are pointers to the first and last characters of the **object name**.
  * **user_data** is a pointer to the user data.

* **group declaration start**
  ```c
  int group_begin(const char *first, const char *last, void *user_data)
  ```
  * **first** and **last** are pointers to the first and last characters of the **group name**.
  * **user_data** is a pointer to the user data.

* **group declaration end**
  ```c
  int group_end(const char *first, const char *last, void *user_data)
  ```
  * **first** and **last** are pointers to the first and last characters of the **group name**.
  * **user_data** is a pointer to the user data.

* **parsing error**
  ```c
  void error(int line, const char *msg, void *user_data)
  ```
  * **line** is the number of the line at which the error occured.
  * **msg** is the error message.
  * **user_data** is a pointer to the user data.

Callbacks returning an **int** must return 0 if an error occured.

The callbacks are set using the following functions :
```c
PDS_set_attribute_callbacks(&callbacks, attribute_begin, attribute_end)
```
```c
PDS_set_pointer_callbacks(&callbacks, pointer_begin, pointer_end)
```
```c
PDS_set_group_callbacks(&callbacks, group_begin, group_end)
```
```c
PDS_set_object_callbacks(&callbacks, object_begin, object_end)
```
```c
PDS_set_scalar_callback(&callbacks, scalar)
```
```c
PDS_set_sequence_callbacks(&callbacks, sequence_begin, sequence_element, sequence_end)
```
```c
PDS_set_set_callbacks(&callbacks, set_begin, set_element, set_end)
```
```c
PDS_set_error_callback(&callbacks, error)
```
    
Finally the parsing is started by calling **PDS_parse**. An implementation example can be found in **test/PDS_parse.c**

## Usage - tinypds_dom ##
`tinypds_dom` works on raw ASCII buffer just like `tinypds`.
* **parsing**
  ```c
  int PDS_DOM_parse(const char *buffer, size_t len, PDS_item **pds, PDS_parse_error *error)
  ```
  * **buffer** is a string of **len** characters containing the PDS document to be parsed.
  * **pds** will contain the pointer to first item of the PDS tree.
  * If something went wrong, **error** will contain the line where the error occured and a description of that error.
  * This function returns 1 upon success and 0 otherwise.
  
* **cleanup**
  ```c
  void PDS_DOM_delete(PDS_item *pds)
  ```
  This function releases the memory used by the PDS tree pointed by **pds**.
  
* **query**
  ```c
  PDS_item* PDS_DOM_find(const char *name, PDS_item *current, PDS_search_type search, int check_current)
  ```
  This function will search the PDS tree for the first item (object, group or attribute) which name matches the one given as argument.
  The **PDS_item** argument gives the starting point of the search. 
  The **PDS_search_type** parameter specifies the way the tree should be traversed. 4 modes are available.
    * PDS_ONLY_SIBLINGS only the siblings of the current item will be checked.
    * PDS_ONLY_CHILDREN only the children of the current item will be checked. 
    * PDS_CHILDREN_RECURSIVE all the children of the current item will be recursively checked.
    * PDS_SIBLINGS_RECURSIVE all siblings and their associated children will be recursively checked.
  
  The last parameter indicates if the starting point must be checked or ignored.
  
* **traversal**
  The following functions and macros give a way to walk along the PDS tree.
  ```c
    PDS_item* PDS_DOM_sibling(PDS_item* pds)
  ```
  Returns a pointer the first sibling item.
  ```c
    PDS_item* PDS_DOM_parent(PDS_item* pds)
  ```
  Returns a pointer to the parent item.
  ```c
    PDS_item* PDS_DOM_object_begin(PDS_item* pds)
  ```
  Returns a pointer to the first item of the current object.
  ```c
    PDS_item* PDS_DOM_object_end(PDS_item* pds)
  ```
  Returns a pointer to one past the last item of current object. 
  ```c
    PDS_DOM_object_foreach(obj, element)
  ```
  Iterates over object items. The iteration is only performed on the first level.
  ```c
    PDS_item* PDS_DOM_group_begin(PDS_item* pds)
  ```
  Returns a pointer to the first item of the current group.
  ```c
    PDS_item* PDS_DOM_group_end(PDS_item* pds)
  ```
  Returns a pointer to one past the last item of current group. 
  ```c
    PDS_DOM_group_foreach(grp, element)
  ```
  Iterates over group items
  
* **data access**
  ```c
  PDS_DOM_typeof(pds)
  ```
  ```c
  PDS_DOM_is_attribute(pds)
  ```
  ```c
  PDS_DOM_is_pointer(pds)
  ```
  ```c
  PDS_DOM_is_object(pds)
  ```
  ```c
  PDS_DOM_is_group(pds)
  ```
  ```c
  PDS_DOM_has_scalar(pds)
  ```
  ```c
  PDS_scalar_type PDS_DOM_scalar_typeof(PDS_item *pds)
  ```
  ```c
  int PDS_DOM_scalar_count(PDS_item *pds)
  ```
  ```c
  int PDS_DOM_scalar_get(PDS_item *pds, PDS_scalar *scalar)
  ```
  ```c
  int PDS_DOM_set_get(PDS_item *pds, PDS_scalar *scalar, int i)
  ```
  ```c
  int PDS_DOM_sequence1d_get(PDS_item *pds, PDS_scalar *scalar, int i)
  ```
  ```c
  int PDS_DOM_sequence2d_get(PDS_item *pds, PDS_scalar *scalar, int i, int j)
  ```
  ```c
  int PDS_DOM_sequence2d_rows(PDS_item *pds)
  ```
  ```c
  int PDS_DOM_sequence2d_cols(PDS_item *pds, int i)
  ```

## License ##
The MIT License
Copyright (c) 2016 Vincent Cruz
