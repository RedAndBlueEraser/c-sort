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
    char *a = (char *)ptr1, *aend = a + len, *b = (char *)ptr2, tmp;
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

static char *sort_quicksort_partition_med3(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptr1 = (char *)arr,  /* Pointer to first element in array. */
        *ptr2,                 /* Pointer to middle element in array. */
        *ptr3;                 /* Pointer to last element in array. */

    /* Median of array with one or two elements is the first element. */
    if (count <= 2) {
        return ptr1;
    }

    /* Compare first, middle and last elements in array and return median
     * element.
     */
    ptr2 = ptr1 + (count / 2) * elesize;
    ptr3 = ptr1 + (count - 1) * elesize;
    if (cmp(ptr1, ptr2) < 0) {
        if (cmp(ptr2, ptr3) < 0) {
            return ptr2;
        } else if (cmp(ptr1, ptr3) < 0) {
            return ptr3;
        } else {
            return ptr1;
        }
    } else {
        if (cmp(ptr1, ptr3) < 0) {
            return ptr1;
        } else if (cmp(ptr2, ptr3) < 0) {
            return ptr3;
        } else {
            return ptr2;
        }
    }
}

static void sort_quicksort_partition(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *), char **ptrfirstgt, size_t *ifirsteq, size_t *ifirstgt) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrcurr,                  /* Pointer to element to be compared. */
        *ptrfirsteq,               /* Pointer to first element in equal to partition of array. */
        *ptrpivot;                 /* Pointer to pivot element to be compared. */
    size_t icurr = 0;              /* Index of element to be compared. */
    int cmpresult;                 /* Result of comparison between elements. */

    /* Determine pivot element with median-of-three. */
    ptrpivot = sort_quicksort_partition_med3(arr, count, elesize, cmp);

    /* Iterate until array partitioned. */
    ptrcurr = ptrfirsteq = ptrfirst;
    *ptrfirstgt = ptrfirst + count * elesize;
    *ifirstgt = count;
    while (icurr < *ifirstgt) {
        /* Compare element with pivot element. */
        cmpresult = cmp(ptrcurr, ptrpivot);

        /* If element is less than pivot element, swap element to after end of
         * less than partition of array (which is also the start of equal to
         * partition). This will move the first element of the equal to
         * partition (if any) to after end of equal to partition. Keep track of
         * pivot element when it is swapped.
         */
        if (cmpresult < 0) {
            memswap(ptrcurr, ptrfirsteq, elesize);
            if (ptrpivot == ptrfirsteq) {
                ptrpivot = ptrcurr;
            }
            ptrcurr += elesize;
            icurr++;
            ptrfirsteq += elesize;
        /* If element is greater than pivot element, swap element to before
         * start of greater than partition of array. Keep track of pivot element
         * when it is swapped.
         */
        } else if (cmpresult > 0) {
            *ptrfirstgt -= elesize;
            (*ifirstgt)--;
            memswap(ptrcurr, *ptrfirstgt, elesize);
            if (ptrpivot == *ptrfirstgt) {
                ptrpivot = ptrcurr;
            }
        /* If element is equal to pivot element, compare next element. */
        } else {
            ptrcurr += elesize;
            icurr++;
        }
    }

    /* Store pointer to first element in equal to partition of array, index of
     * first element in equal to partition of array, and index of first element
     * in greater than partition of array.
     */
    *ifirsteq = (ptrfirsteq - ptrfirst) / elesize;
}

void sort_quicksort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirstgt;  /* Pointer to first element in greater than partition of array. */
    size_t ifirsteq,   /* Index of first element in equal to partition of array. */
        ifirstgt;      /* Index of first element in greater than partition of array. */

    /* Don't need to sort arrays with one or less elements. */
    if (count <= 1) {
        return;
    }

    /* Partition array into three partitions - less than, equal to and greater
     * than partitions. The equal to partition would be correctly positioned in
     * the array.
     */
    sort_quicksort_partition(arr, count, elesize, cmp, &ptrfirstgt, &ifirsteq, &ifirstgt);

    /* Recursively sort less than and greater than paritions of the array. */
    sort_quicksort(arr, ifirsteq, elesize, cmp);
    sort_quicksort((void *)ptrfirstgt, count - ifirstgt, elesize, cmp);
}

void sort_slowsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrmiddle,                /* Pointer to middle element in array. */
        *ptrmiddleprev,            /* Pointer to element before middle element in array. */
        *ptrlast;                  /* Pointer to last element in array. */
    size_t ndivide2,               /* Count divided by two. */
        nlessndivide2,             /* Count minus count divided by two. */
        nless1;                    /* Count minus one. */

    /* Don't need to sort arrays with one or less elements. */
    if (count <= 1) {
        return;
    }

    /* Recursively sort first and second halves of the array. */
    ndivide2 = count / 2;
    ptrmiddle = ptrfirst + ndivide2 * elesize;
    nlessndivide2 = count - ndivide2;
    sort_slowsort(arr, ndivide2, elesize, cmp);
    sort_slowsort((void *)ptrmiddle, nlessndivide2, elesize, cmp);

    /* Compare last element of first half and last element of second half of the
     * array and swap larger element to the end of the array.
     */
    ptrmiddleprev = ptrmiddle - elesize;
    nless1 = count - 1;
    ptrlast = ptrfirst + nless1 * elesize;
    if (cmp(ptrmiddleprev, ptrlast) > 0) {
        memswap(ptrmiddleprev, ptrlast, elesize);
    }

    /* Recursively sort array without last, largest element. */
    sort_slowsort(arr, nless1, elesize, cmp);
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

void sort_selectionsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrlast,                  /* Pointer to last element in array. */
        *ptrmin,                   /* Pointer to smallest element in array. */
        *ptr;                      /* Pointer to element to be compared. */
    size_t n = count,              /* Length of unsorted portion of array. */
        i;

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Iterate until array sorted. After the n'th iteration, at least the n
     * smallest elements is correctly sorted at the start of the array.
     */
    ptrlast = ptrfirst + (count - 1) * elesize;
    while (ptrfirst < ptrlast) {
        /* Find smallest element in array. */
        ptrmin = ptrfirst;
        ptr = ptrfirst + elesize;
        for (i = 1; i < n; i++) {
            if (cmp(ptrmin, ptr) > 0) {
                ptrmin = ptr;
            }
            ptr += elesize;
        }

        /* Swap smallest element towards the start of the array (to the start of
         * unsorted portion of array).
         */
        memswap(ptrfirst, ptrmin, elesize);

        ptrfirst += elesize;
        n--;
    }
}
