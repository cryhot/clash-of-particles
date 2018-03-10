#define _GNU_SOURCE
#include "heap.h"
#include "dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NB 0x40000
#define MIN_NB 0x10

int main(int argc, char const *argv[]) {
    char const *filename = NULL;
    FILE *out = stdout;

    if (argc>1) filename = argv[1];

    if (filename != NULL) {
        out = fopen(filename, "w");
        if (out == NULL) {
            fprintf(stderr, "Cannot write to out %s!\n", filename);
            exit(EXIT_FAILURE);
        }
    }

    // pre-create dummies
    dummy_t dummies[MAX_NB];
    for (size_t i = 0; i < MAX_NB; i++) {
        dummies[i].key = i;
        dummies[i].value = NULL;
    }
    dummies[0].key += 0;

    for (size_t nb = MIN_NB; nb <= MAX_NB; nb <<=2) {
        heap_t *dummy_heap = heap_new(*compare_dummies);
        clock_t start, end;

        start = clock();
        for (size_t i = 0; i < nb; i++) {
            heap_insert(dummy_heap, dummies+i);
        }
        end = clock();
        double insert_time = (double) (end - start) / CLOCKS_PER_SEC;

        start = clock();
        for (size_t i = 0; i < nb; i++) {
            heap_extract_min(dummy_heap);
        }
        end = clock();
        double extract_time = (double) (end - start) / CLOCKS_PER_SEC;

        heap_deallocate(dummy_heap);
        fprintf(out, "%lu,%lf,%lf\n", nb, insert_time, extract_time);
    }

    fclose(out);
    out = NULL;

    return 0;
}
