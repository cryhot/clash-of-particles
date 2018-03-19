#define _POSIX_C_SOURCE 199506L
#include "simulation.h"
#include "disc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_PARTICLES 100
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
                 (i<count-1) ? 1+i%7 : 1+(int)(timestamp*29)%7 ); // last particle blinks
    }
    UpdateScreen();
}

static void
generate_one_particles(particle_t *particle_list[], size_t index, unsigned int *seed)
{
    const long double MAX_RADIUS = 0.010*2;
    const long double MIN_REL_RADIUS = 0.4; // relative to max radius
    const long double MAX_VELOCITY = 0.0005;
    const long double MAX_MASS = 0.8;
    const long double MIN_REL_MASS = 0.5; // relative to max mass
    particle_t *p = malloc(sizeof *p);
    long double radius = ( MIN_REL_RADIUS+rand_r(seed)*(1-MIN_REL_RADIUS)/RAND_MAX )*MAX_RADIUS;
    for (size_t d = 0; d < NB_DIM; d++)
        p->position[d] = ( radius + rand_r(seed)*(1.L-2.L*radius)/RAND_MAX )*loc_UNIT;
    p->radius = radius*loc_UNIT;
    for (size_t j = 0; j < index; j++)
        if (loc_distance(p->position, particle_list[j]->position) < p->radius+particle_list[j]->radius) {
            free(p);
            generate_one_particles(particle_list, index, seed); // try again
            return; // recursive terminal
        }
    p->timestamp   = 0;
    p->col_counter = 0;
    do { // uniform repartition in a sphere
        for (size_t d = 0; d < NB_DIM; d++)
            p->velocity[d] = ( rand_r(seed)*(2.L*MAX_VELOCITY)/RAND_MAX - MAX_VELOCITY )*loc_UNIT;
    } while (loc_scal_prod(p->velocity,p->velocity)>MAX_VELOCITY*loc_UNIT*MAX_VELOCITY*loc_UNIT);
    p->mass = ( MIN_REL_MASS+rand_r(seed)*(1-MIN_REL_MASS)/RAND_MAX )*MAX_MASS*mass_UNIT;
    // p->color = 1 + rand_r(seed)%7;
    particle_list[index] = p;
}

/* Demo for back in time calculation */
int main(int argc, char const *argv[]) {
    unsigned int seed = 6502;
    count = 0;

    CreateWindow("Breakdance!", W_SIZE, W_SIZE);

    int time_flow = 1; // time direction
    for (int s = 0; s < MAX_PARTICLES; s++) {
        generate_one_particles(particle_list, count++, &seed);

        simulation_loop(particle_list, count, s*20*time_flow*time_UNIT, &draw_frame, 2*time_UNIT);
        
        time_flow *= -1;
        for (size_t i = 0; i < count; i++) {
            particle_list[i]->timestamp = 0;
        }
    }


    CloseWindow();

    for (size_t i = 0; i < count; i++) {
        free(particle_list[i]);
    }

    return 0;
}
