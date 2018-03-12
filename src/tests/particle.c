#define NB_DIM 2
#define T_EPS 1e-6
#define S_EPS 1e-6
#include "particle.h"
#include <stdio.h>
#include <stdlib.h>

#undef NDEBUG
#include <assert.h>

particle_t *
new_test_particle(loc_t px, loc_t py, loc_t vx, loc_t vy, mass_t m, loc_t r)
{
    particle_t *particle = malloc(sizeof *particle);
    particle->position[0] = px;
    particle->position[1] = py;
    particle->velocity[0] = vx;
    particle->velocity[1] = vy;
    particle->mass        = m;
    particle->radius      = r;
    particle->timestamp   = 0;
    particle->col_counter = 0;
    return particle;
}

void
check_collision_hplane(particle_t const *p, size_t dim,
                       time_t time2col,
                       loc_t new_vx, loc_t new_vy)
{
    particle_t p_temp = *p; // clone particle
    time_t t1 = time_before_crossing_hplane(&p_temp, dim, 0); // first wall
    time_t t2 = time_before_crossing_hplane(&p_temp, dim, 1); // second wall
    time_t t = NEVER;
    if (IS_FUTURE_TIME(t1) && IS_BEFORE(t1, t)) t = t1;
    if (IS_FUTURE_TIME(t2) && IS_BEFORE(t2, t)) t = t2;
    if (!IS_FUTURE_TIME(time2col)) {
        printf("%15.8s\n", "INFINITY");
        assert(!IS_FUTURE_TIME(t));
        return;
    }
    assert(EQ_TIME_ZERO(t-time2col));
    update(&p_temp, t);
    collide_hplane(&p_temp, dim);
    printf("%15.2lf    (%9.6lf, %9.6lf)\n", t, p_temp.velocity[0], p_temp.velocity[1]);
    assert(EQ_LOC_ZERO(p_temp.velocity[0]-new_vx));
    assert(EQ_LOC_ZERO(p_temp.velocity[1]-new_vy));
}

void
check_collision_particule(particle_t const *p1, particle_t const *p2,
                          time_t time2col,
                          loc_t new_v1x, loc_t new_v1y,
                          loc_t new_v2x, loc_t new_v2y)
{
    particle_t p1_temp = *p1; // clone particle
    particle_t p2_temp = *p2; // clone particle
    time_t t = time_before_contact(&p1_temp, &p2_temp);
    if (!IS_FUTURE_TIME(time2col)) {
        printf("%15.8s\n", "INFINITY");
        assert(!IS_FUTURE_TIME(t));
        return;
    }
    assert(EQ_TIME_ZERO(t-time2col));
    update(&p1_temp, t);
    update(&p2_temp, t);
    // printf("p1(%lf %lf)\n", p1_temp.position[0], p1_temp.position[1]);
    // printf("p2(%lf %lf)\n", p2_temp.position[0], p2_temp.position[1]);
    collide_particule(&p1_temp, &p2_temp);
    printf("%15.2lf    (%9.6lf, %9.6lf)\n", t, p1_temp.velocity[0], p1_temp.velocity[1]);
    printf("%15.8s    (%9.6lf, %9.6lf)\n", "", p2_temp.velocity[0], p2_temp.velocity[1]);
    assert(EQ_LOC_ZERO(p1_temp.velocity[0]-new_v1x));
    assert(EQ_LOC_ZERO(p1_temp.velocity[1]-new_v1y));
}

int
main(void)
{
    printf("====================\n");
    printf("generating particules...\n");
    particle_t *p1 = new_test_particle(.25, .25,  .50,  .00, 0.5, 1e-2);
    particle_t *p2 = new_test_particle(.25, .25, -.50,  .00, 0.5, 1e-2);
    particle_t *p3 = new_test_particle(.25, .25,  .00,  .50, 0.5, 1e-2);
    particle_t *p4 = new_test_particle(.25, .25,  .00, -.50, 0.5, 1e-2);
    particle_t *p5 = new_test_particle(.25, .25,  .25, -.40, 0.5, 1e-2);
    particle_t *p6 = new_test_particle(.50, .25,  .00,  .00, 0.8, 5e-3);
    particle_t *p7 = new_test_particle(.75, .25, -.25,  .00, 0.5, 1e-2);
    particle_t *p8 = new_test_particle(.60, .80,  .25, -.40, 0.8, 5e-3);
    printf("====================\n");
    printf("testing collisions with vertical walls...\n");
    printf("%-17.17s  %-22.12s\n", "time to collision", "new velocity");
    check_collision_hplane(p1, 0,  1.48, -.50,  .00);
    check_collision_hplane(p2, 0,  0.48,  .50,  .00);
    check_collision_hplane(p3, 0, NEVER,    0,    0);
    check_collision_hplane(p4, 0, NEVER,    0,    0);
    check_collision_hplane(p5, 0,  2.96, -.25, -.40);
    printf("OK!\n");
    printf("====================\n");
    printf("testing collisions with horizontal walls...\n");
    printf("%-17.17s  %-22.12s\n", "time to collision", "new velocity");
    check_collision_hplane(p1, 1, NEVER,    0,    0);
    check_collision_hplane(p2, 1, NEVER,    0,    0);
    check_collision_hplane(p3, 1,  1.48,  .00, -.50);
    check_collision_hplane(p4, 1,  0.48,  .00,  .50);
    check_collision_hplane(p5, 1,  0.60,  .25,  .40);
    printf("OK!\n");
    printf("====================\n");
    printf("testing collisions between particles...\n");
    printf("%-17.17s  %-22.12s\n", "time to collision", "new velocity");
    check_collision_particule(p1, p6, 0.470000, -.115385,  .000000,  .384615,  .000000);
    check_collision_particule(p1, p7, 0.640000, -.250000,  .000000,  .500000,  .000000);
    check_collision_particule(p1, p8, 1.352274,  .067993, -.329141,  .520004, -.194287);
    check_collision_particule(p7, p8,    NEVER,        0,        0,        0,        0);
    printf("OK!\n");
    printf("====================\n");
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
    free(p6);
    free(p7);
    free(p8);
    printf("TEST OK!\n");
    printf("====================\n");
    return 0;
}
