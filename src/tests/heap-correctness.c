#define _GNU_SOURCE
#include "heap.h"
#include "dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    printf("====================\n");
    heap_t *dummy_heap = heap_new(*compare_dummies);
    dummy_t *d;

    for (size_t i = 0; i < 50; i++) {
        d = malloc(sizeof *d);
        d->key = i;
        if (asprintf(&(d->value), "dummy #%lu", i)<0) return 1;

        heap_insert(dummy_heap, d);
    }

    double k = -INFINITY;
    while ((d=heap_extract_min(dummy_heap)) != NULL) {
        printf("extract <%s>\n", d->value);
        if (d->key < k) {
            printf("ERROR: %f < %f!\n", d->key, k);
            printf("====================\n");
            return 1;
        }
        k = d->key;
        free(d->value);
        free(d);
    }

    heap_deallocate(dummy_heap);

    printf("OK!\n");

    printf("====================\n");
    return 0;
}
