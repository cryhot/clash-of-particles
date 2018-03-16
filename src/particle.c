#include "particle.h"

void
update(particle_t *p, time_t timestamp)
{
    time_t dt = timestamp - p->timestamp;
    coords_update(p->position, p->velocity, dt);
    p->timestamp = timestamp;
}


time_t
time_before_crossing_hplane(particle_t const *p, size_t dim, loc_t pos)
{
    loc_t dist = pos - p->position[dim];
    dist += p->radius * (dist>0 ? -1 : 1);
    return path_time(dist, p->velocity[dim]);
}

void
collide_hplane(particle_t *p, size_t dim)
{
    p->velocity[dim] *= -1;
    p->col_counter++;
}


time_t
time_before_contact(particle_t const *p1, particle_t const *p2)
{
    particle_t p2_temp = *p2; // clone particle
    update(&p2_temp, p1->timestamp); // see clone at p1 timestamp

    loc_t dvel[NB_DIM];
    loc_t dpos[NB_DIM];
    loc_delta(p2_temp.position, p1->position, dpos);
    loc_delta(p2_temp.velocity, p1->velocity, dvel);
    loc_t dist_min = p1->radius + p2->radius;

    loc_t prod_pv = loc_scal_prod(dpos, dvel);
    loc_t prod_vv = loc_scal_prod(dvel, dvel); // actual relative speed (squared)
    loc_t prod_pp = loc_scal_prod(dpos, dpos); // actual distance (squared)
    loc_t prod_pp_min = dist_min * dist_min;   // minimum distance (squared)
    loc_t deterinant = prod_pv*prod_pv - prod_vv*(prod_pp-prod_pp_min);
    if (deterinant<0) return NEVER;
    return - (prod_pv+sqrt(deterinant)) / prod_vv * time_UNIT;
}

void
collide_particle(particle_t *p1, particle_t *p2)
{
    loc_t dvel[NB_DIM];
    loc_t dpos[NB_DIM];
    loc_delta(p2->position, p1->position, dpos);
    loc_delta(p2->velocity, p1->velocity, dvel);

    double long coeff = 2.0l * loc_scal_prod(dpos, dvel) / (p1->mass+p2->mass) / loc_scal_prod(dpos, dpos);
    // dp.dp should be equal to (r1+r2)^2, and simulation is more stable if not

    loc_append(p1->velocity, dpos,  p2->mass*coeff);
    loc_append(p2->velocity, dpos, -p1->mass*coeff);
    p1->col_counter++;
    p2->col_counter++;
}
