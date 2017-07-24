/*
 * sort.h
 * Version 20170714
 * Written by Harry Wong (RedAndBlueEraser)
 */

#include <stddef.h>

/* Sort the elements in the array with bubble sort. */
void sort_bubblesort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with cocktail shaker sort. */
void sort_cocktailshakersort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with odd-even sort. */
void sort_oddevensort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with comb sort. */
void sort_combsort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with gnome sort. */
void sort_gnomesort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with quicksort. */
void sort_quicksort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with slowsort. */
void sort_slowsort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));

/* Sort the elements in the array with stooge sort. */
void sort_stoogesort(void *arr, size_t count, size_t elesize,
    int (*cmp)(const void *, const void *));
