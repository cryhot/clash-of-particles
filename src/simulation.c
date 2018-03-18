#include "simulation.h"
#include "event.h"
#include "heap.h"
#include <stdlib.h>

/** @brief Compute future collision of a particule with an hyperplane. */
static void compute_collisions_hplane(heap_t *event_heap, particle_t *p) {
    time_t t_min = NEVER;
    size_t i_min = 0;
    for (size_t i = 0; i < NB_DIM*2; i++) { // iterate through dimentions and walls
        loc_t pos = (i&1)*loc_UNIT; // position = 0|1
        if (EQ_LOC_ZERO(p->position[i>>1]-pos-p->radius) || EQ_LOC_ZERO(p->position[i>>1]-pos+p->radius)) continue; // skip current wall
        time_t t = time_before_crossing_hplane(p, i>>1, pos);
        if (IS_FUTURE_TIME(t) && IS_BEFORE(t, t_min)) {
            t_min = t;
            i_min = i;
        }
    }
    // printf(">> %lu (%"time_F") %d\n", i_min, t_min, IS_FUTURE_TIME(t_min));
    if (IS_FUTURE_TIME(t_min))
        heap_insert(event_heap, event_collide_hplane(p->timestamp+t_min, p, i_min>>1));
}

/** @brief Compute future collision between two particules. */
static void compute_collisions_particules(heap_t *event_heap, particle_t *p1, particle_t *p2) {
    time_t t = time_before_contact(p1, p2);
    if (!IS_FUTURE_TIME(t)) return;
    heap_insert(event_heap, event_collide_particle(p1->timestamp+t, p1, p2));
}


void
simulation_loop(particle_t *particle_list[], size_t nb_part, time_t duration, void (*callback)(time_t timestamp), time_t callback_rate)
{
    heap_t *event_heap = heap_new(&compare_events); // queue of future events
    if (!EQ_TIME_ZERO(callback_rate)) // create first refresh event
        heap_insert(event_heap, event_refresh(0));
    for (size_t i = 0; i < nb_part; i++) { // compute every collision events at initial state
        compute_collisions_hplane(event_heap, particle_list[i]);
        for (size_t j = i+1; j < nb_part; j++) {
            compute_collisions_particules(event_heap, particle_list[i], particle_list[j]);
        }
    }

    event_t *event;
    while ((event=heap_extract_min(event_heap)) != NULL) { // mail loop: process queued events
        if (IS_BEFORE(duration, event->timestamp)) { // end of simulation reached
            free(event);
            break;
        }
        if (!event_is_valid(event)) { // discard invalid events
            free(event);
            continue;
        }
        time_t t = event->timestamp;
        switch (get_event_type(event)) {
            case EVENT_COLLIDE_PARTICLE:
                // update concerned particles
                update(event->particle_a, t);
                update(event->particle_b, t);
                collide_particle(event->particle_a, event->particle_b);
                // compute collisions
                compute_collisions_hplane(event_heap, event->particle_a);
                compute_collisions_hplane(event_heap, event->particle_b);
                for (size_t i = 0; i < nb_part; i++) {
                    if (particle_list[i] == event->particle_a) continue;
                    if (particle_list[i] == event->particle_b) continue;
                    compute_collisions_particules(event_heap, event->particle_a, particle_list[i]);
                    compute_collisions_particules(event_heap, event->particle_b, particle_list[i]);
                }
                break;
            case EVENT_COLLIDE_HPLANE:
                // update concerned particles
                update(event->particle_a, t);
                collide_hplane(event->particle_a, event->particle_b_col);
                // compute collisions
                compute_collisions_hplane(event_heap, event->particle_a);
                for (size_t i = 0; i < nb_part; i++) {
                    if (particle_list[i] == event->particle_a) continue;
                    compute_collisions_particules(event_heap, event->particle_a, particle_list[i]);
                }
                break;
            case EVENT_REFRESH:
                (*callback)(t);
                heap_insert(event_heap, event_refresh(t+callback_rate));
                break;
        }
        free(event);
    }

    heap_deallocate(event_heap);

    // set particles position at simulation final time.
    for (size_t i = 0; i < nb_part; i++) {
        update(particle_list[i], duration);
    }
}


size_t
load_particles(particle_t *particle_list[], size_t max_count, FILE* file)
{
    size_t count = 0;
    size_t i = 0;
    char buffer[4096];
    if (fgets(buffer, 4096, file)==NULL) goto err0;
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
    size_t count = 0;
    size_t i = 0;
    char buffer[4096];
    if (fgets(buffer, 4096, file)==NULL) goto err0;
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
