# tinypds : a small PDS 3 parser
[![Travis build Status](https://travis-ci.org/BlockoS/tinypds.svg)](https://travis-ci.org/BlockoS/tinypds)    [![Coverity Scan Build Status](https://scan.coverity.com/projects/8523/badge.svg)](https://scan.coverity.com/projects/blockos-tinypds)

`tinypds` is a small and simple event-driven single header-only library PDS 3 parser.

`tinypds` is written in C with little or no dependencies.

In order to use `tinypds` in your project, just copy `tinypds.h` into your project.
Note that this file contains both the interface and the implementation.
To instantiate the implementation add the following define in *ONE* source file before including `tinypds.h`:
```c
#define TINY_PDS_IMPL
```

## Usage ##
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

## Build ##
A CMake configuration file is provided in order to build a static library, documentation or run a bundle of tests.

## License ##
The MIT License
Copyright (c) 2015 Vincent Cruz
