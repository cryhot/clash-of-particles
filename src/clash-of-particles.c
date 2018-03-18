#include "simulation.h"
#include "disc.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_PARTICLES 10000
#define W_SIZE 900 // windows size

static particle_t *particle_list[MAX_PARTICLES];
static size_t count;
static void draw_frame(time_t timestamp) {
    assert(NB_DIM==2);
    EmptySpace();
    for (size_t i = 0; i < count; i++) {
        particle_t p = *particle_list[i]; // clone particle
        update(&p, timestamp); // see clone at current time
        DrawDISC(W_SIZE*p.position[0]/loc_UNIT,
                 W_SIZE*p.position[1]/loc_UNIT,
                 W_SIZE*p.radius/loc_UNIT,
                 1+i%7);
    }
    UpdateScreen();
}

int main(int argc, char const *argv[]) {
    FILE *input_file = stdin; // by default, read from standard input
    if (argc>1) { // if a file is specified, read fromè it
        char *endptr;
        count = strtol(argv[1], &endptr, 10);
        if (endptr!=NULL && *endptr=='\0') { // number read
            input_file = NULL; // will be generated
            fprintf(stderr, "Not implemented!\n");
            exit(EXIT_FAILURE);
        } else {
            input_file = fopen(argv[1], "r");
            if (input_file == NULL) {
                fprintf(stderr, "Cannot read file %s!\n", argv[1]);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (input_file!=NULL) {
        count = load_particles(particle_list, MAX_PARTICLES, input_file);
        fclose(input_file);
        input_file = NULL;
    } else
        generate_particles(particle_list, count);

    CreateWindow("Gaz gaz gaz", W_SIZE, W_SIZE);

    simulation_loop(particle_list, count, 20000, &draw_frame, 2*time_UNIT);
    // simulation_loop(particle_list, count, NEVER, &draw_frame, 2*time_UNIT);

    // while (1);
    CloseWindow();

    for (size_t i = 0; i < count; i++) {
        free(particle_list[i]);
    }

    return 0;
}
