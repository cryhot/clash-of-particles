#include "simulation.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_PARTICLES 10000

int main(int argc, char const *argv[]) {
    FILE *input_file = stdin;
    if (argc>1) {
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            fprintf(stderr, "Cannot read file %s!\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    particle_t *particle_list[MAX_PARTICLES];
    size_t count = load_particles(particle_list, MAX_PARTICLES, input_file);

    fclose(input_file);
    input_file = NULL;

    export_particles(particle_list, count, stdout, "<file read with test-loader>");

    for (size_t i = 0; i < count; i++) {
        free(particle_list[i]);
    }

    return 0;
}
