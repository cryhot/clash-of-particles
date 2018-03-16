#include "simulation.h"
#include "event.h"
#include <stdlib.h>

void
simulation_loop(particle_t *particle_list[], size_t nb_part, time_t duration, void (*callback)(void), time_t callback_rate)
{

}


size_t
load_particles(particle_t *particle_list[], size_t max_count, FILE* file)
{
    char buffer[4096];
    fgets(buffer, 4096, file);
    size_t count = 0;
    size_t i = 0;
    if (fscanf(file, "%lu", &count) != 1) goto err0; // get number of particles
    if (count > max_count) goto err0;
    for (i = 0; i < count; i++) {
        particle_t *p = malloc(sizeof *p);
        particle_list[i] = p;
        p->timestamp   = 0;
        p->col_counter = 0;
        double value;
        for (size_t d = 0; d < NB_DIM; d++) { // get position
            if (fscanf(file, "%lf,", &value) != 1) goto err0;
            p->position[d] = value * loc_UNIT;
        }
        for (size_t d = 0; d < NB_DIM; d++) { // get velocity
            if (fscanf(file, "%lf,", &value) != 1) goto err0;
            p->velocity[d] = value * loc_UNIT;
        }
        if (fscanf(file, "%lf,", &value) != 1) goto err0;
        p->mass = value * mass_UNIT;
        if (fscanf(file, "%lf", &value) != 1) goto err0;
        p->radius = value * loc_UNIT;
    }
    return count;
    err0: while(i>0) free(particle_list[i--]);
    return 0;
}

size_t
load_raw_particles(particle_t *particle_list[], size_t max_count, FILE* file)
{
    char buffer[4096];
    fgets(buffer, 4096, file);
    size_t count = 0;
    size_t i = 0;
    if (fscanf(file, "%lu", &count) != 1) goto err0; // get number of particles
    if (count > max_count) goto err0;
    for (i = 0; i < count; i++) {
        particle_t *p = malloc(sizeof *p);
        particle_list[i] = p;
        p->timestamp   = 0;
        p->col_counter = 0;
        for (size_t d = 0; d < NB_DIM; d++) // get position
            if (fscanf(file, "%"loc_F",", &(p->position[d])) != 1) goto err0;
        for (size_t d = 0; d < NB_DIM; d++) // get velocity
            if (fscanf(file, "%"loc_F",", &(p->velocity[d])) != 1) goto err0;
        if (fscanf(file, "%"mass_F",%"loc_F, &(p->mass), &(p->radius)) != 2) goto err0;
    }
    return count;
    err0: while(i>0) free(particle_list[i--]);
    return 0;
}

void
generate_particles(particle_t *particle_list[], size_t count)
{

}


void
export_particles(particle_t *particle_list[], size_t count, FILE* file, char *header)
{
    fprintf(file, "%s\n", (header!=NULL)?header:"no description specified");
    fprintf(file, "%lu\n", count);
    for (size_t i = 0; i < count; i++) {
        particle_t *p = particle_list[i];
        for (size_t d = 0; d < NB_DIM; d++)
            fprintf(file, "%lf,", (double)(p->position[d]/loc_UNIT));
        for (size_t d = 0; d < NB_DIM; d++)
            fprintf(file, "%lf,", (double)(p->velocity[d]/loc_UNIT));
        fprintf(file, "%lf,", (double)(p->mass/mass_UNIT));
        fprintf(file, "%lf\n", (double)(p->radius/loc_UNIT));
    }
}

void
export_raw_particles(particle_t *particle_list[], size_t count, FILE* file, char *header)
{
    fprintf(file, "%s\n", (header!=NULL)?header:"no description specified");
    fprintf(file, "%lu\n", count);
    for (size_t i = 0; i < count; i++) {
        particle_t *p = particle_list[i];
        for (size_t d = 0; d < NB_DIM; d++)
            fprintf(file, "%"loc_F",", p->position[d]);
        for (size_t d = 0; d < NB_DIM; d++)
            fprintf(file, "%"loc_F",", p->velocity[d]);
        fprintf(file, "%"mass_F",%"loc_F"\n", p->mass, p->radius);
    }
}
