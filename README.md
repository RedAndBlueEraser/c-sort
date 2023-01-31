# C Sort

Implementation of several sorting algorithms in pure C.

## How to use?

You can either create an object file from the provided source files and link it to your program, or simply copy the functions you need from `sort.c` into your program.

### Setup

Place `sort.h` and `sort.c` next to your C program source files.

Add `#include "sort.h"` with your program's `#include` directives.

You can now use various sorting algorithms in your C program.

### Compiling

Create the `sort.o` object file by running:

```Shell
gcc -c sort.c
```

Then compile and link your C program by running:

```Shell
gcc sort.o yourprogram.c -o yourprogram
```

### Copying

You can directly copy functions you need from `sort.h` and `sort.c` into your program instead.

`sort.h` contains function prototypes for all implemented sorting algorithms. These declare the functions' names, parameters and return types.

`sort.c` contains function definitions for all implemented sorting algorithms as well as several utility functions.

You must copy the `memswap` function in order to copy and use any implemented sorting algorithm into your program.

In order to copy and use the implemented odd-even sort algorithm `sort_oddevensort`, you must have `#define FALSE 0` and `#define TRUE !FALSE` in your program's `#define` directives.

In order to copy and use the implemented comb sort algorithm `sort_combsort`, you must have `#define FALSE 0`, `#define TRUE !FALSE` and `#define SORT_COMBSORT_SHRINKFACTOR 1.3` in your program's `#define` directives.

In order to copy and use the implemented quicksort algorithm `sort_quicksort`, you must have `#define SORT_QUICKSORT_INSSORTTHRES 8` in your program's `#define` directives. You must also copy the `sort_quicksort_partition_med3` and `sort_quicksort_partition` functions.

In order to copy and use the implemented heapsort algorithm `sort_heapsort`, you must also copy the `sort_heapsort_heapify_siftdown` and `sort_heapsort_heapify` functions.

In order to copy and use the implemented shellsort algorithm `sort_shellsort`, you must have `#define SORT_SHELLSORT_GAPSEQ { ... }` and `#define SORT_SHELLSORT_GAPSEQ_COUNT 13` in your program's `#define` directives.

In order to copy and use the implemented merge sort algorithm `sort_mergesort`, you must have `#include <stdio.h>`, `#include <stdlib.h>` and `#include <string.h>` in your program's `#include` directives. You must also copy the `sort_mergesort_merge` function.

## Functions and algorithms

All functions to call implemented sorting algorithms follow the same function signature structure:

```C
void sort_algorithmnamesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

Replace `algorithmnamesort` with a sorting algorithm name such as `bubblesort` or `quicksort`.

- `void *arr` - The array to be sorted. Cannot be `NULL`.
- `size_t count` - The number of elements in the array. Must be a positive integer greater than or equal to 0.
- `size_t elesize` - The size of one element in the array. Must be a positive integer greater than 0.
- `int (*cmp)(const void *, const void *))` - The comparison function used to sort the array in ascending order. It takes in two pointers to elements as arguments, and compares the elements, and returns a value greater than 0 if the first data item comes after the second data item, a value less than 0 if the first data item comes before the second data item or `0` if the two data items are equal.

The type and order of the functions' parameters is very much the same as C library function `qsort`:

```C
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
```

All implemented sorting algorithms can be used in place of `qsort` and vice versa without changing the function arguments.

### Bubble sort - `sort_bubblesort()`

```C
void sort_bubblesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm bubbles the largest element to the end of the array in each iteration.

This implementation is optimised by remembering the furthest (to the end) swapped position in each iteration to shrink the array to be sorted.

### Cocktail shaker sort - `sort_cocktailshakersort()`

```C
void sort_cocktailshakersort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm bubbles the smallest element to the start of the array and the largest element to the end of the array in each iteration.

This implementation is optimised by remembering the nearest (to the start) and the furthest (to the end) swapped positions in each iteration to shrink the array to be sorted.

### Odd-even sort - `sort_oddevensort()`

```C
void sort_oddevensort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm uses two separate iterations to bubble odd-even and even-odd indexed pairs of elements. The concept behind this algorithm is that it is a highly parallelisable version of bubble sort.

This implementation is single-threaded. A multi-threaded implementation of this algorithm may possibly be achieved with pthread.

### Comb sort - `sort_combsort()`

```C
void sort_combsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm initially bubbles distant pairs of elements, and closing the gap with each iteration, ending with a normal bubble sort.

This implementation uses a shrink factor of 1.3 as defined by `SORT_COMBSORT_SHRINKFACTOR`. You may change this to other values (any real number greater than 1 is valid).

### Gnome sort - `sort_gnomesort()`

```C
void sort_gnomesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm bubbles elements to grow a sorted section of the array at the start of the array. Each iteration sorts an additional element into this sorted section of the array.

This implementation is optimised by remembering the furthest (to the end) compared position in each iteration to eliminate unnecessary comparisons. This optimisation makes this implementation function exactly the same as the implemented insertion sort.

### Quicksort - `sort_quicksort()`

```C
void sort_quicksort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm uses a recursive divide and conquer method to partition the array into "less than" and "greater than" sections, and then recursively partition those sections to achieve a sorted array.

This implementation uses the "median of three" method to choose the pivot element. It is optimised to partition the array with an additional "equals to" section to reduce the depth of recursions. It is also optimised to use insertion sort instead of further partitioning when there are 8 or less elements as defined by `SORT_QUICKSORT_INSSORTTHRES`. You may change this to other values (any integer greater than or equal to 0 is valid).

### Slowsort - `sort_slowsort()`

```C
void sort_slowsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm uses a poor recursive divide and conquer method to move the largest element to end of the array in each iteration.

Slowsort is a very slow algorithm, but does have a bounded time complexity. It is jokingly referred to as a "multiply and surrender" solution, the opposite of "divide and conquer".

### Stooge sort - `sort_stoogesort()`

```C
void sort_stoogesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **exchange** based sort algorithm. This algorithm uses a recursive method by sorting overlapping 2/3 sections of the array.

### Selection sort - `sort_selectionsort()`

```C
void sort_selectionsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **selection** based sort algorithm. This algorithm scans for the smallest element and moves it to the start of the array in each iteration.


### Heapsort - `sort_heapsort()`

```C
void sort_heapsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **selection** based sort algorithm. This algorithm constructs a heap in the array and uses it to put the largest element at the end of the array in each iteration.

This implementation uses sift down to construct and maintain the heap.

### Insertion sort - `sort_insertionsort()`

```C
void sort_insertionsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **insertion** based sort algorithm. This algorithm moves elements to grow a sorted section of the array at the start of the array. Each iteration sorts an additional element into this sorted section of the array.

This implementation functions exactly the same as the implemented gnome sort, despite with differently written lines of code.

### Shellsort - `sort_shellsort()`

```C
void sort_shellsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **insertion** based sort algorithm. This algorithm moves elements to grow sorted distant elements in the array.

This implementation uses Marcin Ciura's gap sequence followed by Naoyuki Tokuda's gap sequence as defined by `SORT_SHELLSORT_GAPSEQ` and `SORT_SHELLSORT_GAPSEQ_COUNT`. You may change this to other values (an array of decrementing integers and ending with 1).


### Merge sort - `sort_mergesort()`

```C
void sort_mergesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));
```

An **merge** based sort algorithm. This algorithm uses a recursive divide and conquer method to sort halves of the array and then merge them.

## Example

```C
#include <stdio.h>
#include "sort.h"

int cmp(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    int A[] = { 9, 5, 3, 1, 2, 8, 0, 4, 7, 6 };
    /* A is now [9, 5, 3, 1, 2, 8, 0, 4, 7, 6]. */
    sort_mergesort(A, 10, sizeof (int), cmp);
    /* A is now [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]. */
    return 0;
}
```

## Algorithm characteristics

The characteristics of the implemented sorting algorithms are listed below, mostly copied from [Wikipedia](https://en.wikipedia.org/wiki/Sorting_algorithm#Comparison_of_algorithms "Sorting algorithm").

| Name                  | Time complexity (Best)      | (Average)                    | (Worst)                     | Space complexity | Is stable? |
| --------------------- | --------------------------- | ---------------------------- | --------------------------- | ---------------- | ---------- |
| Bubble sort           | n                           | n<sup>2</sup>                | n<sup>2</sup>               | 1                | Yes        |
| Cocktail shaker sort  | n                           | n<sup>2</sup>                | n<sup>2</sup>               | 1                | Yes        |
| Odd-even sort         | n                           | n<sup>2</sup>                | n<sup>2</sup>               | 1                | Yes        |
| Comb sort             | n log(n)                    | ?                            | n<sup>2</sup>               | 1                | No         |
| Gnome sort            | n                           | n<sup>2</sup>                | n<sup>2</sup>               | 1                | Yes        |
| Quicksort             | n log(n)                    | n log(n)                     | n<sup>2</sup>               | log(n)           | No         |
| Slowsort              | ?                           | ?                            | ?                           | ?                | Yes        |
| Stooge sort           | n<sup>log(3)/log(3/2)</sup> | n<sup>log(3)/log(3/2)</sup>  | n<sup>log(3)/log(3/2)</sup> | n                | No         |
| Selection sort        | n<sup>2</sup>               | n<sup>2</sup>                | n<sup>2</sup>               | 1                | No         |
| Heapsort              | n log(n)                    | n log(n)                     | n log(n)                    | 1                | No         |
| Insertion sort        | n                           | n<sup>2</sup>                | n<sup>2</sup>               | 1                | Yes        |
| Shellsort             | n log(n)                    | ?                            | n log<sup>2</sup>(n)        | 1                | No         |
| Merge sort            | n log(n)                    | n log(n)                     | n log(n)                    | n                | Yes        |

_Time complexity_ refers to the time taken to run the sorting algorithm relative to the input size.
_(Best)_ is the best case scenario (possibly by entering particular input) where the sorting algorithm finishes the fastest.
_(Average)_ is the typical performance of the sorting algorithm given various random inputs.
_(Worst)_ is the worst case scenario (possibly by entering particular input) where the sorting algorithm finishes the slowest.

_Space complexity_ refers to the maximum amount of memory space used to run the sorting algorithm relative to the input size.

_Is stable?_ refers to whether the sorting algorithm maintains the relative order of equal elements.

## Performance tests

The performance tests results of the implemented sorting algorithms are tabled below, including the time taken to finish, the number of comparisons made (by counting the number of calls made to the comparison function), and the number of swaps made.

The test program was compiled with gcc version (5.4.0) (GCC) in x86_64-pc-cygwin. The tests were performed on CYGWIN_NT-6.3 2.8.2 x86_64 Cygwin with an Intel Core i7-4790 CPU (4 vCPUs only), 6 GB RAM and Windows 8.1 x64 OS machine.

| Name                       | Test 1                                                   | Test 2                                                | Test 3                                                    | Test 4                                               |
| -------------------------- | -------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------- | ---------------------------------------------------- |
| In-built qsort<sup>*</sup> |        21 ms<br>    2664799 compares                     |        0 ms<br>     450004 compares                   |         6 ms<br>     600015 compares                      |        3 ms<br>     150011 compares                  |
| Bubble sort                | 144 s 781 ms<br>11249129233 compares<br>5607749833 swaps |        0 ms<br>     149999 compares<br>       0 swaps | 186 s 678 ms<br>11249925000 compares<br>11249924995 swaps |        0 ms<br>     149999 compares<br>      0 swaps |
| Cocktail shaker sort       | 118 s 245 ms<br> 7477502504 compares<br>5607749833 swaps |        0 ms<br>     149999 compares<br>       0 swaps | 186 s 247 ms<br>11249925000 compares<br>11249924995 swaps |        0 ms<br>     149999 compares<br>      0 swaps |
| Odd-even sort              | 141 s 457 ms<br>11223375177 compares<br>5607749833 swaps |        0 ms<br>     149999 compares<br>       0 swaps | 192 s 956 ms<br>11250074999 compares<br>11249924995 swaps |        0 ms<br>     149999 compares<br>      0 swaps |
| Comb sort                  |        47 ms<br>    6400094 compares<br>   1291171 swaps |       21 ms<br>    5650099 compares<br>       0 swaps |        31 ms<br>    5800098 compares<br>     383756 swaps |       22 ms<br>    5650099 compares<br>      0 swaps |
| Gnome sort                 |  93 s 802 ms<br> 5607899815 compares<br>5607749833 swaps |        0 ms<br>     149999 compares<br>       0 swaps | 181 s 844 ms<br>11249925000 compares<br>11249924995 swaps |        0 ms<br>     149999 compares<br>      0 swaps |
| Quicksort                  |        63 ms<br>    2815824 compares<br>   2652888 swaps |      573 ms<br>   31488373 compares<br>31302447 swaps |       397 ms<br>   22669892 compares<br>   22483896 swaps |        0 ms<br>     150003 compares<br>      0 swaps |
| Slowsort                   |      >30 min<sup>**</sup>                                |     >30 min<sup>**</sup>                              |      >30 min<sup>**</sup>                                 |     >30 min<sup>**</sup>                             |
| Stooge sort                |      >30 min<sup>**</sup>                                |     >30 min<sup>**</sup>                              |      >30 min<sup>**</sup>                                 |     >30 min<sup>**</sup>                             |
| Selection sort             |  35 s 214 ms<br>11249925000 compares<br>    150000 swaps | 35 s 120 ms<br>11249925000 compares<br>  150000 swaps |  35 s 813 ms<br>11249925000 compares<br>     150000 swaps | 34 s 212 ms<br>11249925000 compares<br> 150000 swaps |
| Heapsort                   |        78 ms<br>    4700614 compares<br>   2448044 swaps |       68 ms<br>    4838882 compares<br> 2561460 swaps |        63 ms<br>    4571194 compares<br>    2336011 swaps |        6 ms<br>     449994 compares<br> 150000 swaps |
| Insertion sort             |  93 s 578 ms<br> 5607899815 compares<br>5607749833 swaps |        0 ms<br>     149999 compares<br>       0 swaps |  182 s 59 ms<br>11249925000 compares<br>11249924995 swaps |        3 ms<br>     149999 compares<br>      0 swaps |
| Shellsort                  |        68 ms<br>    3983707 compares<br>   2169398 swaps |        8 ms<br>    1877271 compares<br>       0 swaps |        25 ms<br>    2654412 compares<br>     903035 swaps |        6 ms<br>    1877271 compares<br>      0 swaps |
| Merge sort                 |        73 ms<br>    2392304 compares<br>   2410977 swaps |       37 ms<br>    1264224 compares<br> 1198688 swaps |        41 ms<br>    1323635 compares<br>    2522318 swaps |       37 ms<br>    1264224 compares<br>1198688 swaps |

<sup>*</sup> In-built qsort implementation used in gcc version (5.4.0) (GCC) in x86_64-pc-cygwin. The number of swaps made cannot be recorded.

<sup>**</sup> Test stopped because it took too long.

Four different tests were performed:

- Test 1 - Unsorted array with 150000 random integers between -1000000 and 1000000.
- Test 2 - Sorted array with 150000 random integers between -1000000 and 1000000.
- Test 3 - Reverse sorted array with 150000 random integers between -1000000 and 1000000.
- Test 4 - Array with 150000 integers of '5's.

Each test was ran five times for each implemented sorting algorithm and the in-built qsort, and the average time taken to finish was rounded to the nearest millisecond and recorded as the time taken to finish. The number of comparisons made and the number of swaps made were consistent over the five repetitions.

Due to limited patience, if the test took over 30 minutes, the test was terminated.

### Conclusions and highlights

The following conclusions and highlights are only based on this implementation only and that comparisons and swaps are cheap. It may not fully reflect the actual performance of the sorting algorithm.

The in-built qsort in gcc version (5.4.0) (GCC) in x86_64-pc-cygwin is really fast for all tests, requiring a small number of comparisons. It performs best on already sorted, reverse sorted and uniform input.

Somewhat slow sorting algorithms such as bubble sort, cocktail shaker sort, odd-even sort, gnome sort and insertion sort perform bad on unsorted and reverse sorted input, however, they are extremely fast for already sorted and uniform input, requiring minimal number of compares and swaps.

Comb sort, an improved version of bubble sort, and shellsort, an improved version of insertion sort, dramatically improved the performance of the sorting algorithm, significantly reducing the time taken (from ~3 minutes to less than a second) for some tests.

Selection sort is very consistent across tests requiring the same amount of time, number of comparisons and number of swaps regardless of the order of the array. It is, however, still a slow sorting algorithm, but generally faster than bubble sort and even insertion sort when the array is not already sorted.

Comb sort, quicksort, heapsort, shellsort and merge sort are really fast sorting algorithms, performing relatively fast in all tests.

Quicksort is noticeably slightly slow when the array is already sorted or reverse sorted. Changing the pivot selection significantly changed this, so improvements to pivot selection is useful.

## Author

RedAndBlueEraser
