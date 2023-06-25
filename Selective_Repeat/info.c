/*
To initialize an array of integers to 0 in C, you can use the following approaches:

1. Static Initialization:
   If the array is declared with static storage duration or at file scope, you can initialize it with `{0}` or `{}`. This will set all elements of the array to 0.

*/
int array[10] = {0};

/*
2. Automatic Initialization:
   If the array is declared with automatic storage duration (inside a function), you can use the `memset` function from the `<string.h>` header to set all elements to 0.

*/
#include <string.h>

int array[10];
memset(array, 0, sizeof(array));
/*

   The `memset` function takes the destination array, the value to be set (`0` in this case), and the number of bytes to set (`sizeof(array)`).
*/

/*
3. Dynamic Initialization:
   If the array is allocated dynamically using `malloc` or `calloc`, you can use `calloc` to allocate and initialize the memory to 0 in a single step.

*/
int *array = calloc(10, sizeof(int));
/*

The `calloc` function allocates the specified number of elements(`10` in this case) and sets them to 0.

These methods ensure that all elements of the integer array are initialized to 0 when you declare
or allocate it.

* /

