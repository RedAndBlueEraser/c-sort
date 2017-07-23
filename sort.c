/*
 * sort.c
 * Version 20170714
 * Written by Harry Wong (RedAndBlueEraser)
 */

#include "sort.h"

#define FALSE 0
#define TRUE !FALSE
#define SORT_COMBSORT_SHRINKFACTOR 1.3

extern int swapcount;

/* Swap the specified amount of bytes between two memory areas. */
static void memswap(void *ptr1, void *ptr2, size_t len) {
    char *a = ptr1, *aend = a + len, *b = ptr2, tmp;
    swapcount++;
    while (a < aend) {
        tmp = *a;
        *a = *b;
        *b = tmp;
        a++;
        b++;
    }
}

void sort_bubblesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrlast,                  /* Pointer to last element in array. */
        *ptr1,                     /* Pointer to first element to be compared. */
        *ptr2,                     /* Pointer to second element to be compared. */
        *ptrlastswap;              /* Pointer to first element that was last swapped. */

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Iterate until array sorted. After the n'th iteration, at least the n
     * largest elements is correctly sorted at the end of the array.
     */
    ptrlast = ptrfirst + (count - 1) * elesize;
    while (ptrfirst < ptrlast) {
        ptrlastswap = ptrfirst;

        /* Iterate pairs until reach sorted end of array. */
        for (ptr1 = ptrfirst; ptr1 < ptrlast; ptr1 = ptr2) {
            ptr2 = ptr1 + elesize;

            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                ptrlastswap = ptr1;
            }
        }

        /* The element that was last swapped closest towards the end of the
         * array indicates this part of the array onwards is correctly sorted.
         */
        ptrlast = ptrlastswap;
    }
}

void sort_cocktailshakersort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrlast,                  /* Pointer to last element in array. */
        *ptr1,                     /* Pointer to first element to be compared. */
        *ptr2,                     /* Pointer to second element to be compared. */
        *ptrlastswap;              /* Pointer to element that was last swapped. */

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Iterate until array sorted. After the n'th iteration, the n smallest and
     * n largest elements would be correctly sorted at the end of the array.
     */
    ptrlast = ptrfirst + (count - 1) * elesize;
    while (ptrfirst < ptrlast) {
        ptrlastswap = ptrfirst;

        /* Iterate pairs until reach sorted end of array. */
        for (ptr1 = ptrfirst; ptr1 < ptrlast; ptr1 = ptr2) {
            ptr2 = ptr1 + elesize;

            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                ptrlastswap = ptr1;
            }
        }

        /* The element that was last swapped closest towards the end of the
         * array indicates this part of the array onwards is correctly sorted.
         */
        ptrlast = ptrlastswap;

        /* Iterate pairs until reach sorted start of array. */
        for (ptr2 = ptrlast; ptr2 > ptrfirst; ptr2 = ptr1) {
            ptr1 = ptr2 - elesize;

            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                ptrlastswap = ptr2;
            }
        }

        /* The element that was last swapped closest towards the end of the
         * array indicates this part of the array onwards is correctly sorted.
         */
        ptrfirst = ptrlastswap;
    }
}

void sort_oddevensort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrlast,                  /* Pointer to last element in array. */
        *ptr1,                     /* Pointer to first element to be compared. */
        *ptr2;                     /* Pointer to second element to be compared. */
    int issorted = FALSE;          /* Boolean flag whether array is sorted. */

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Iterate until array sorted. */
    ptrlast = ptrfirst + (count - 1) * elesize;
    while (!issorted) {
        issorted = TRUE;

        /* Iterate even index starting pairs. */
        for (ptr1 = ptrfirst; ptr1 < ptrlast; ptr1 = ptr2 + elesize) {
            ptr2 = ptr1 + elesize;

            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }

            ptr1 = ptr2 + elesize;
        }

        /* Iterate odd index starting pairs. */
        for (ptr1 = ptrfirst + elesize; ptr1 < ptrlast; ptr1 = ptr2 + elesize) {
            ptr2 = ptr1 + elesize;

            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }

            ptr1 = ptr2 + elesize;
        }
    }
}

void sort_combsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptr1,                     /* Pointer to first element to be compared. */
        *ptr2;                     /* Pointer to second element to be compared. */
    int issorted = FALSE;          /* Boolean flag whether array is sorted. */
    size_t gap = count,            /* Distance between elements to be compared. */
        i;

    /* Iterate until array sorted. */
    while (!issorted) {
        /* Determine gap with shrink factor. */
        gap /= SORT_COMBSORT_SHRINKFACTOR;
        if (gap > 1) {
            issorted = FALSE;
        } else {
            gap = 1;
            issorted = TRUE;
        }

        /* Iterate spaced pairs. */
        ptr1 = ptrfirst;
        ptr2 = ptr1 + gap * elesize;
        for (i = gap; i < count; i++) {
            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }

            ptr1 += elesize;
            ptr2 += elesize;
        }
    }
}

void sort_gnomesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrlast,                  /* Pointer to last element in array. */
        *ptr1,                     /* Pointer to first element to be compared. */
        *ptr2,                     /* Pointer to second element to be compared. */
        *ptr2max;                  /* Pointer to furthest element ever reached in array.*/

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Iterate until array sorted. When the last pair of elements at the end of
     * the array has been compared (and possibly swapped), then all elements
     * would be correctly sorted.
     */
    ptrlast = ptrfirst + (count - 1) * elesize;
    for (ptr1 = ptrfirst; ptr1 < ptrlast; ptr1 = ptr2) {
        /* Store pointer to furthest element ever reached in array. */
        ptr2max = ptr2 = ptr1 + elesize;

        /* Compare pair and continuously swap smaller element as far towards the
         * beginning of the array.
         */
        while (ptr2 > ptrfirst && cmp(ptr1, ptr2) > 0) {
            memswap(ptr1, ptr2, elesize);
            ptr2 = ptr1;
            ptr1 -= elesize;
        }

        /* Restore pointer from furthest element to resume working in last
         * working, important position.
         */
        ptr2 = ptr2max;
    }
}

void sort_stoogesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrlast;                  /* Pointer to last element in array. */
    size_t ndivide3,               /* Count divided by three. */
        nlessndivide3;             /* Count minus count divided by three. */

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Compare first and last elements and swap larger element to the end of the
     * array.
     */
    ptrlast = ptrfirst + (count - 1) * elesize;
    if (cmp(ptrfirst, ptrlast) > 0) {
        memswap(ptrfirst, ptrlast, elesize);
    }

    /* Recursively sort first 2/3, last 2/3, and then first 2/3 again of the
     * array when there are three or more elements.
     */
    if (count >= 3) {
        ndivide3 = count / 3;
        nlessndivide3 = count - ndivide3;
        sort_stoogesort(arr, nlessndivide3, elesize, cmp);
        sort_stoogesort((void *)(ptrfirst + ndivide3 * elesize), nlessndivide3, elesize, cmp);
        sort_stoogesort(arr, nlessndivide3, elesize, cmp);
    }
}
