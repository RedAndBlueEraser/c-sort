/*
 * sort.c
 * Version 20170714
 * Written by Harry Wong (RedAndBlueEraser)
 */

#include <stdio.h>
#include <stdlib.h>

#define ARRLEN 10
#define FALSE 0
#define TRUE !FALSE
#define SORT_COMBSORT_SHRINKFACTOR 1.3

/* Sort the elements in the array with bubble sort. */
void sort_bubblesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));

/* Sort the elements in the array with cocktail shaker sort. */
void sort_cocktailshakersort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));

/* Sort the elements in the array with odd-even sort. */
void sort_oddevensort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));

/* Sort the elements in the array with comb sort. */
void sort_combsort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *));

/* Print integer array. */
void printintarray(int arr[], size_t len) {
    size_t i;

    printf("[");

    /* Print first element if it exists. */
    if (len > 0) {
        printf("%d", arr[0]);
    }
    /* Print subsequent elements padded with a comma and space. */
    for (i = 1; i < len; i++) {
        printf(", %d", arr[i]);
    }

    printf("] @ %p\n", arr);
}

/* Comparison function to determine the sort order. */
int cmp(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

/* Swap the specified amount of bytes between two memory areas. */
void memswap(void *ptr1, void *ptr2, size_t len) {
    char *a = ptr1, *aend = a + len, *b = ptr2, tmp;
    while (a < aend) {
        tmp = *a;
        *a = *b;
        *b = tmp;
        a++;
        b++;
    }
}

int main(int argc, char *argv[]) {
    int arr[ARRLEN] = {380, -751, 812, 542, 436, -300, 794, -978, -870, 711};

    /* Print array before and after sort. */
    printintarray(arr, ARRLEN);
    sort_bubblesort(arr, 10, sizeof (int), cmp);
    printintarray(arr, ARRLEN);

    return 0;
}

void sort_bubblesort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,  /* Pointer to first element in array. */
        *ptrend,                   /* Pointer to last element in array. */
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
    ptrend = ptrstart + (count - 1) * elesize;
    while (ptrstart < ptrend) {
        ptrlastswap = ptrstart;

        /* Iterate pairs until reach sorted end of array. */
        for (ptr1 = ptrstart; ptr1 < ptrend; ptr1 = ptr2) {
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
        ptrend = ptrlastswap;
    }
}

void sort_cocktailshakersort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,  /* Pointer to first element in array. */
        *ptrend,                   /* Pointer to last element in array. */
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
    ptrend = ptrstart + (count - 1) * elesize;
    while (ptrstart < ptrend) {
        ptrlastswap = ptrstart;

        /* Iterate pairs until reach sorted end of array. */
        for (ptr1 = ptrstart; ptr1 < ptrend; ptr1 = ptr2) {
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
        ptrend = ptrlastswap;

        /* Iterate pairs until reach sorted start of array. */
        for (ptr2 = ptrend; ptr2 > ptrstart; ptr2 = ptr1) {
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
        ptrstart = ptrlastswap;
    }
}

void sort_oddevensort(void *arr, size_t count, size_t elesize, int (*cmp)(const void *, const void *)) {
    char *ptrstart = (char *)arr,  /* Pointer to first element in array. */
        *ptrend,                   /* Pointer to last element in array. */
        *ptr1,                     /* Pointer to first element to be compared. */
        *ptr2;                     /* Pointer to second element to be compared. */
    int issorted = FALSE;          /* Boolean flag whether array is sorted. */

    /* Avoid unsigned arithmetic loss. */
    if (count == 0) {
        return;
    }

    /* Iterate until array sorted. */
    ptrend = ptrstart + (count - 1) * elesize;
    while (!issorted) {
        issorted = TRUE;

        /* Iterate even index starting pairs. */
        for (ptr1 = ptrstart; ptr1 < ptrend; ptr1 = ptr2 + elesize) {
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
        for (ptr1 = ptrstart + elesize; ptr1 < ptrend; ptr1 = ptr2 + elesize) {
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
    char *ptrstart = (char *)arr,  /* Pointer to first element in array. */
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
        for (i = gap; i < count; i++) {
            ptr1 = ptrstart + (i - gap) * elesize;
            ptr2 = ptrstart + i * elesize;

            /* Compare pair and swap larger element towards the end of the
             * array.
             */
            if (cmp(ptr1, ptr2) > 0) {
                memswap(ptr1, ptr2, elesize);
                issorted = FALSE;
            }
        }
    }
}
