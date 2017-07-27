/*
 * sort.c
 * Version 20170714
 * Written by Harry Wong (RedAndBlueEraser)
 */

#include "sort.h"

#define FALSE 0
#define TRUE !FALSE
#define SORT_COMBSORT_SHRINKFACTOR 1.3

extern unsigned long int swapcount;

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
    char *ptrstart = (char *)arr,              /* Pointer to start of array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of unsorted portion of array. */
        *ptr1,                                 /* Pointer to first element to be compared. */
        *ptr2,                                 /* Pointer to second element to be compared. */
        *ptrlastswap;                          /* Pointer to furthest element that was last swapped. */

    /* Iterate until the entire array is sorted. After the n'th iteration, at
     * least the n largest elements are correctly positioned at the end of the
     * array. Only the unsorted portion of the array is traversed through.
     */
    while (ptrstart < ptrend) {
        /* Store the furthest element that is swapped. */
        ptrlastswap = ptrstart;

        /* Step through each pair of elements from the start of the array to the
         * end of the unsorted portion of the array, compare them, and swap them
         * if out of order.
         */
        for (ptr1 = ptrstart, ptr2 = ptr1 + elesize; ptr2 < ptrend; ptr1 = ptr2, ptr2 += elesize) {
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                ptrlastswap = ptr2;
            }
        }

        /* Mark the furthest element that was last swapped as the end of the
         * unsorted portion of the array.
         */
        ptrend = ptrlastswap;
    }
}

void sort_cocktailshakersort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of unsorted portion of array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of unsorted portion of array. */
        *ptr1,                                 /* Pointer to first element to be compared. */
        *ptr2,                                 /* Pointer to second element to be compared. */
        *ptrlastswap;                          /* Pointer to furthest element that was last swapped. */

    /* Iterate until the entire array is sorted. After the n'th iteration, at
     * least the n smallest and n largest elements are correctly positioned at
     * the start and end of the array, respectively. Only the unsorted portion
     * of the array is traversed through.
     */
    while (ptrstart < ptrend) {
        /* Store the furthest element that is swapped. */
        ptrlastswap = ptrstart;

        /* Step through each pair of elements from the start of the unsorted
         * portion of the array to the end of the unsorted portion of the array,
         * compare them, and swap them if out of order.
         */
        for (ptr1 = ptrstart, ptr2 = ptr1 + elesize; ptr2 < ptrend; ptr1 = ptr2, ptr2 += elesize) {
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                ptrlastswap = ptr2;
            }
        }

        /* Mark the furthest element that was last swapped as the end of the
         * unsorted portion of the array.
         */
        ptrend = ptrlastswap;

        if (!(ptrstart < ptrend)) {
            break;
        }

        /* Step through each pair of elements from the end of the unsorted
         * portion of the array to the start of the unsorted portion of the
         * array, compare them, and swap them if out of order.
         */
        for (ptr2 = ptrend - elesize, ptr1 = ptr2 - elesize; ptr2 > ptrstart; ptr2 = ptr1, ptr1 -= elesize) {
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                ptrlastswap = ptr2;
            }
        }

        /* Mark the furthest element that was last swapped as the start of the
         * unsorted portion of the array.
         */
        ptrstart = ptrlastswap;
    }
}

void sort_oddevensort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of array. */
        *ptr1,                                 /* Pointer to first element to be compared. */
        *ptr2,                                 /* Pointer to second element to be compared. */
        issorted = !count;                     /* Boolean flag whether array is sorted. */

    /* Iterate until the entire array is sorted. */
    while (!issorted) {
        /* Flag whether the array is sorted. If a swap occurs, the array is not
         * sorted.
         */
        issorted = TRUE;

        /* Step through each pair of elements with an even indexed first element
         * from the start of the array to the end of the array, compare them,
         * and swap them if out of order.
         */
        for (ptr1 = ptrstart, ptr2 = ptr1 + elesize; ptr1 < ptr2 && ptr2 < ptrend; ptr1 = ptr2 + elesize, ptr2 = ptr1 + elesize) {
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }
        }

        /* Step through each pair of elements with an odd indexed first element
         * from the start of the array to the end of the array, compare them,
         * and swap them if out of order.
         */
        for (ptr1 = ptrstart + elesize, ptr2 = ptr1 + elesize; ptr1 < ptr2 && ptr2 < ptrend; ptr1 = ptr2 + elesize, ptr2 = ptr1 + elesize) {
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }
        }
    }
}

void sort_combsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of array. */
        *ptr1,                                 /* Pointer to first element to be compared. */
        *ptr2,                                 /* Pointer to second element to be compared. */
        issorted = !count;                     /* Boolean flag whether array is sorted. */
    size_t gap = count;                        /* Distance between elements to be compared. */

    /* Iterate until the entire array is sorted. */
    while (!issorted) {
        /* Calculate gap with previous gap and shrink factor. */
        /* Flag whether the array is sorted. If the gap is greater than 1, or a
         * swap occurs, the array is not sorted.
         */
        gap /= SORT_COMBSORT_SHRINKFACTOR;
        if (gap <= 1) {
            issorted = TRUE;
            gap = 1;
        }

        /* Step through each pair of spaced elements from the start of the array
         * to the end of the array, compare them, and swap them if out of order.
         */
        for (ptr1 = ptrstart, ptr2 = ptr1 + gap * elesize; ptr2 < ptrend; ptr1 += elesize, ptr2 += elesize) {
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }
        }
    }
}

void sort_gnomesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of array. */
        *ptr1,                                 /* Pointer to first element to be compared. */
        *ptr2,                                 /* Pointer to second element to be compared. */
        *ptrlastcmp;                           /* Pointer to furthest element that was last compared. */

    /* Avoid comparing against overflowed pointer. */
    if (count == 0) {
        return;
    }

    /* Iterate until the entire array is sorted. After the n'th iteration, at
     * least the n elements are sorted (but may not be in the correct position)
     * at the start of the array.
     */
    for (ptr1 = ptrstart, ptrlastcmp = ptr2 = ptr1 + elesize; ptr2 < ptrend; ptr1 = ptrlastcmp, ptrlastcmp = ptr2 = ptr1 + elesize) {
        /* Step through each pair of elements from the n'th element in the array
         * to the start of the array, compare them, and swap them if out of
         * order.
         */
        while (ptr2 > ptrstart && cmp(ptr1, ptr2) > 0) {
            memswap(ptr1, ptr2, elesize);
            ptr2 = ptr1;
            ptr1 -= elesize;
        }
    }
}

static char *sort_quicksort_partition_med3(char *ptr1, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptr2,  /* Pointer to middle element in array. */
        *ptr3;   /* Pointer to last element in array. */

    /* The median element of an array with one or two elements is always the
     * first element.
     */
    if (count <= 2) {
        return ptr1;
    }

    /* Compare the first, middle and last elements in the array and return the
     * median element.
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

static void sort_quicksort_partition(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *), char **ptrptrfirstgt, size_t *ptrifirsteq, size_t *ptrifirstgt) {
    char *ptrstart = (char *)arr,                  /* Pointer to start of array. */
        *ptrcurr = ptrstart,                       /* Pointer to element to be compared. */
        *ptrfirsteq = ptrstart,                    /* Pointer to first element in equal to partition of array. */
        *ptrfirstgt = ptrstart + count * elesize,  /* Pointer to first element in greater than partition of array. */
        *ptrpivot;                                 /* Pointer to pivot element to be compared. */
    int cmpresult;                                 /* Result of comparison between elements. */

    /* Get pivot element with median-of-three. */
    ptrpivot = sort_quicksort_partition_med3(ptrstart, count, elesize, cmp);

    /* Iterate until the entire array is partitioned. */
    while (ptrcurr < ptrfirstgt) {
        /* Compare the element with the pivot element. */
        cmpresult = cmp(ptrcurr, ptrpivot);

        /* If the element is smaller than the pivot element, swap the element to
         * past the end of the less than partition of the array (also the start
         * of the equal to partition). This moves the first element of the equal
         * to partition (if present) to past the end of the equal to partition.
         * Keep track of the pivot element if it is swapped.
         */
        if (cmpresult < 0) {
            memswap(ptrcurr, ptrfirsteq, elesize);
            if (ptrpivot == ptrfirsteq) {
                ptrpivot = ptrcurr;
            }
            ptrcurr += elesize;
            ptrfirsteq += elesize;
        /* If the element is greater than the pivot element, swap the element to
         * before the start of the greater than partition of the array. Keep
         * track of pivot element if it is swapped.
         */
        } else if (cmpresult > 0) {
            ptrfirstgt -= elesize;
            memswap(ptrcurr, ptrfirstgt, elesize);
            if (ptrpivot == ptrfirstgt) {
                ptrpivot = ptrcurr;
            }
        /* If the element is equal to the pivot element, compare the next
         * element.
         */
        } else {
            ptrcurr += elesize;
        }
    }

    /* Store the pointer to the first element in the greater than partition of
     * the array, the index of the first element in the equal to partition of
     * the array, and the index of first element in the greater than partition
     * of the array.
     */
    *ptrptrfirstgt = ptrfirstgt;
    *ptrifirsteq = (ptrfirsteq - ptrstart) / elesize;
    *ptrifirstgt = (ptrfirstgt - ptrstart) / elesize;
}

void sort_quicksort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirstgt;  /* Pointer to first element in greater than partition of array. */
    size_t ifirsteq,   /* Index of first element in equal to partition of array. */
        ifirstgt;      /* Index of first element in greater than partition of array. */

    /* The array is sorted if there are one or fewer elements in the array. */
    if (count <= 1) {
        return;
    }

    /* Partition array into three partitions - less than, equal to, and greater
     * than partitions. After partitioning, the elements in the equal to
     * partition are correctly positioned in the array.
     */
    sort_quicksort_partition(arr, count, elesize, cmp, &ptrfirstgt, &ifirsteq, &ifirstgt);

    /* Recursively sort less than and greater than paritions of the array. */
    sort_quicksort(arr, ifirsteq, elesize, cmp);
    sort_quicksort(ptrfirstgt, count - ifirstgt, elesize, cmp);
}

void sort_slowsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrfirst = (char *)arr,  /* Pointer to first element in array. */
        *ptrmiddle,                /* Pointer to middle element in array. */
        *ptrmiddleprev,            /* Pointer to element before middle element in array. */
        *ptrlast;                  /* Pointer to last element in array. */
    size_t ndivide2,               /* Count divided by two. */
        nless1;                    /* Count minus one. */

    /* The array is sorted if there are one or fewer elements in the array. */
    if (count <= 1) {
        return;
    }

    /* Recursively sort first and second halves of the array. */
    ndivide2 = count / 2;
    ptrmiddle = ptrfirst + ndivide2 * elesize;
    sort_slowsort(arr, ndivide2, elesize, cmp);
    sort_slowsort(ptrmiddle, count - ndivide2, elesize, cmp);

    /* Compare the last element in the first half of the array with the last
     * element in the second half of the array, and swap them if the former is
     * larger than the latter. That is, move the largest element in the array
     * to the end of the array.
     */
    ptrmiddleprev = ptrmiddle - elesize;
    nless1 = count - 1;
    ptrlast = ptrfirst + nless1 * elesize;
    if (cmp(ptrmiddleprev, ptrlast) > 0) {
        memswap(ptrmiddleprev, ptrlast, elesize);
    }

    /* Recursively sort the array without last, largest element. */
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

    /* Compare the first element in the array with the last element in the
     * array, and swap them if the former is larger than the latter.
     */
    ptrlast = ptrfirst + (count - 1) * elesize;
    if (cmp(ptrfirst, ptrlast) > 0) {
        memswap(ptrfirst, ptrlast, elesize);
    }

    /* Recursively sort the first 2/3, the last 2/3, and then the first 2/3
     * again of the array if there are three or more elements.
     */
    if (count >= 3) {
        ndivide3 = count / 3;
        nlessndivide3 = count - ndivide3;
        sort_stoogesort(arr, nlessndivide3, elesize, cmp);
        sort_stoogesort(ptrfirst + ndivide3 * elesize, nlessndivide3, elesize, cmp);
        sort_stoogesort(arr, nlessndivide3, elesize, cmp);
    }
}

void sort_selectionsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of unsorted portion array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of array. */
        *ptr,                                  /* Pointer to element to be compared. */
        *ptrmin;                               /* Pointer to smallest element to be swapped. */

    /* Iterate until the entire array is sorted. After the n'th iteration, at
     * least the n smallest elements are correctly positioned at the start of
     * the array. Only the unsorted portion of the array is traversed through.
     */
    while (ptrstart < ptrend) {
        /* Find the smallest element in the unsorted portion of the array. */
        ptrmin = ptrstart;

        /* Step through each element from the start of the unsorted portion of
         * the array to the end of the array, compare it with the smallest
         * element, and update the smallest element if this one is smaller.
         */
        for (ptr = ptrstart + elesize; ptr < ptrend; ptr += elesize) {
            if (cmp(ptrmin, ptr) > 0) {
                ptrmin = ptr;
            }
        }

        /* Swap the smallest element with the first element in the unsorted
         * portion of the array.
         */
        memswap(ptrstart, ptrmin, elesize);

        ptrstart += elesize;
    }
}

static void sort_heapsort_heapify_siftdown(char *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *), size_t iparent) {
    char *ptrparent,                   /* Pointer to parent node element. */
        *ptrchild1,                    /* Pointer to first, and then, largest child node element. */
        *ptrchild2;                    /* Pointer to second child node element. */
    size_t ichild1 = 2 * iparent + 1,  /* Index of first, and then, largest child node element. */
        ichild2;                       /* Index of second child node element. */

    if (ichild1 < count && ichild1 > iparent) {
        /* Get the largest child node element. */
        ptrchild1 = arr + ichild1 * elesize;
        ichild2 = ichild1 + 1;
        if (ichild2 < count && ichild2 > iparent) {
            ptrchild2 = ptrchild1 + elesize;
            if (cmp(ptrchild1, ptrchild2) < 0) {
                ichild1 = ichild2;
                ptrchild1 = ptrchild2;
            }
        }

        /* Compare the parent node element with its largest child node element,
         * and swap them if the latter is larger than the former. If a swap
         * occurs, recursively sift down the child node.
         */
        ptrparent = arr + iparent * elesize;
        if (cmp(ptrparent, ptrchild1) < 0) {
            memswap(ptrparent, ptrchild1, elesize);
            sort_heapsort_heapify_siftdown(arr, count, elesize, cmp, ichild1);
        }
    }
}

static void sort_heapsort_heapify(char *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    size_t i = count / 2;
    /* Sift down from the bottom of the heap (close to the middle of the array;
     * the last element with child nodes) to the root of the heap (first element
     * in the array). Smaller elements are moved towards the bottom of the heap
     * (end of the array) and larger elements are moved towards the root of the
     * heap (start of the array).
     */
    while (i-- > 0) {
        sort_heapsort_heapify_siftdown(arr, count, elesize, cmp, i);
    }
}

void sort_heapsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of array. */
        *ptrend = ptrstart + count * elesize;  /* Pointer to end of array. */
    size_t i = count;                          /* Index of last element in array. */

    /* Build the heap in the array with the largest element at the root (as the
     * first element of the array).
     */
    sort_heapsort_heapify(ptrstart, count, elesize, cmp);

    /* Iterate until the entire array is sorted. After the n'th iteration, at
     * least the n largest elements are correctly positioned at the end of the
     * array. Only the unsorted portion of the array is processed on.
     */
    while (i-- > 0) {
        /* Swap the first, n'th largest element with the last element in the
         * unsorted portion of the array.
         */
        memswap(ptrstart, ptrend -= elesize, elesize);

        /* Rebuild the heap in the array without the last element in the
         * unsorted portion of the array.
         */
        sort_heapsort_heapify_siftdown(ptrstart, i, elesize, cmp, 0);
    }
}

void sort_insertionsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,              /* Pointer to start of array. */
        *ptrend = ptrstart + count * elesize,  /* Pointer to end of array. */
        *ptrcurr,                              /* Pointer to element to be compared. */
        *ptr1,                                 /* Pointer to first element to be compared. */
        *ptr2;                                 /* Pointer to second element to be compared. */

    /* Iterate until the entire array is traversed. After the n'th iteration, at
     * least the n elements are sorted (but may not be in the correct position)
     * at the start of the array.
     */
    for (ptrcurr = ptrstart; ptrcurr < ptrend; ptrcurr += elesize) {
        /* Step through each pair of elements from the n'th element in the array
         * to the start of the array, compare them, and swap them if out of
         * order.
         */
        for (ptr2 = ptrcurr, ptr1 = ptr2 - elesize; ptr2 > ptrstart && cmp(ptr1, ptr2) > 0; ptr2 = ptr1, ptr1 -= elesize) {
            memswap(ptr1, ptr2, elesize);
        }
    }
}
