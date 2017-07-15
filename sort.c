/*
 * sort.c
 * Version 20170714
 * Written by Harry Wong (RedAndBlueEraser)
 */

#include <stdio.h>
#include <stdlib.h>

#define ARRLEN 10

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

int main(int argc, char *argv[]) {
    int arr[ARRLEN] = {380, -751, 812, 542, 436, -300, 794, -978, -870, 711};

    /* Print array before and after sort. */
    printintarray(arr, ARRLEN);
    qsort(arr, 10, sizeof (int), cmp);
    printintarray(arr, ARRLEN);

    return 0;
}
