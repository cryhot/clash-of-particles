#define NB_DIM 2
#define time_EPS 1e-6
#define loc_EPS 1e-6
#include "particle.h"
#include <stdio.h>
#include <stdlib.h>

#undef NDEBUG
#include <assert.h>

particle_t *
new_test_particle(float px, float py, float vx, float vy, float m, float r)
{
    particle_t *particle = malloc(sizeof *particle);
    particle->position[0] = px*loc_UNIT;
    particle->position[1] = py*loc_UNIT;
    particle->velocity[0] = vx*loc_UNIT;
    particle->velocity[1] = vy*loc_UNIT;
    particle->mass        = m*mass_UNIT;
    particle->radius      = r*loc_UNIT;
    particle->timestamp   = 0;
    particle->col_counter = 0;
    return particle;
}

void
check_collision_hplane(particle_t const *p, size_t dim,
                       float time2col,
                       float new_vx, float new_vy)
{
    particle_t p_temp = *p; // clone particle
    time_t t1 = time_before_crossing_hplane(&p_temp, dim, 0*loc_UNIT); // first wall
    time_t t2 = time_before_crossing_hplane(&p_temp, dim, 1*loc_UNIT); // second wall
    time_t t = NEVER;
    if (IS_FUTURE_TIME(t1) && IS_BEFORE(t1, t)) t = t1;
    if (IS_FUTURE_TIME(t2) && IS_BEFORE(t2, t)) t = t2;
    if (!isfinite(time2col)) {
        printf("%15.8s\n", "INFINITY");
        assert(!IS_FUTURE_TIME(t));
        return;
    }
    assert(EQ_TIME_ZERO(t/time_UNIT-time2col));
    update(&p_temp, t);
    collide_hplane(&p_temp, dim);
    printf("%15.2f    (%9.6f, %9.6f)    (%9.6f, %9.6f)\n",
           (float)(t/time_UNIT),
           (float)((float)p_temp.velocity[0]/loc_UNIT),
           (float)((float)p_temp.velocity[1]/loc_UNIT),
           (float)((float)p_temp.position[0]/loc_UNIT),
           (float)((float)p_temp.position[1]/loc_UNIT));
    assert(EQ_LOC_ZERO((float)p_temp.velocity[0]/loc_UNIT-new_vx));
    assert(EQ_LOC_ZERO((float)p_temp.velocity[1]/loc_UNIT-new_vy));
}

void
check_collision_particle(particle_t const *p1, particle_t const *p2,
                          float time2col,
                          float new_v1x, float new_v1y,
                          float new_v2x, float new_v2y)
{
    particle_t p1_temp = *p1; // clone particle
    particle_t p2_temp = *p2; // clone particle
    time_t t = time_before_contact(&p1_temp, &p2_temp);
    if (!IS_FUTURE_TIME(time2col)) {
        printf("%15.8s\n", "INFINITY");
        assert(!IS_FUTURE_TIME(t));
        return;
    }
    assert(EQ_TIME_ZERO(t/time_UNIT-time2col));
    update(&p1_temp, t);
    update(&p2_temp, t);
    collide_particle(&p1_temp, &p2_temp);
    printf("%15.2f    (%9.6f, %9.6f)    (%9.6f, %9.6f)\n",
           (float)(t/time_UNIT),
           (float)((float)p1_temp.velocity[0]/loc_UNIT),
           (float)((float)p1_temp.velocity[1]/loc_UNIT),
           (float)((float)p1_temp.position[0]/loc_UNIT),
           (float)((float)p1_temp.position[1]/loc_UNIT));
    printf("%15.8s    (%9.6f, %9.6f)    (%9.6f, %9.6f)\n",
           "",
           (float)((float)p2_temp.velocity[0]/loc_UNIT),
           (float)((float)p2_temp.velocity[1]/loc_UNIT),
           (float)((float)p2_temp.position[0]/loc_UNIT),
           (float)((float)p2_temp.position[1]/loc_UNIT));
    assert(EQ_LOC_ZERO((float)p1_temp.velocity[0]/loc_UNIT-new_v1x));
    assert(EQ_LOC_ZERO((float)p1_temp.velocity[1]/loc_UNIT-new_v1y));
    assert(EQ_LOC_ZERO((float)p2_temp.velocity[0]/loc_UNIT-new_v2x));
    assert(EQ_LOC_ZERO((float)p2_temp.velocity[1]/loc_UNIT-new_v2y));
}

int
main(void)
{
    printf("====================\n");
    printf("generating particles...\n");
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
    printf("%-17.17s  %-22.12s    %-22.12s\n", "time to collision", "new velocity", "new position");
    check_collision_hplane(p1, 0,  1.48, -.50,  .00);
    check_collision_hplane(p2, 0,  0.48,  .50,  .00);
    check_collision_hplane(p3, 0, NEVER,    0,    0);
    check_collision_hplane(p4, 0, NEVER,    0,    0);
    check_collision_hplane(p5, 0,  2.96, -.25, -.40);
    printf("OK!\n");
    printf("====================\n");
    printf("testing collisions with horizontal walls...\n");
    printf("%-17.17s  %-22.12s    %-22.12s\n", "time to collision", "new velocity", "new position");
    check_collision_hplane(p1, 1, NEVER,    0,    0);
    check_collision_hplane(p2, 1, NEVER,    0,    0);
    check_collision_hplane(p3, 1,  1.48,  .00, -.50);
    check_collision_hplane(p4, 1,  0.48,  .00,  .50);
    check_collision_hplane(p5, 1,  0.60,  .25,  .40);
    printf("OK!\n");
    printf("====================\n");
    printf("testing collisions between particles...\n");
    printf("%-17.17s  %-22.12s    %-22.12s\n", "time to collision", "new velocity", "new position");
    check_collision_particle(p1, p6, 0.470000, -.115385,  .000000,  .384615,  .000000);
    check_collision_particle(p1, p7, 0.640000, -.250000,  .000000,  .500000,  .000000);
    check_collision_particle(p1, p8, 1.352274,  .067993, -.329141,  .520004, -.194287);
    check_collision_particle(p7, p8,    NEVER,        0,        0,        0,        0);
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
