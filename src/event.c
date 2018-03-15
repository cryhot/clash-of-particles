#include "event.h"
#include <stdlib.h>

bool
event_is_valid(event_t *e)
{
    if (e->particule_a != NULL && e->particule_a->col_counter != e->particule_a_col)
        return false;
    if (e->particule_b != NULL && e->particule_b->col_counter != e->particule_b_col)
        return false;
    return true;
}

int
compare_events(void *event1, void *event2)
{
    event_t *e1 = event1;
    event_t *e2 = event2;
    return (IS_BEFORE(e1->timestamp, e2->timestamp)) ? -1 :
           (IS_BEFORE(e2->timestamp, e1->timestamp)) ?  1 :
                                                        0 ;
}


enum event_type
get_event_type(event_t *e)
{
    if (e->particule_a != NULL) {
        if (e->particule_b != NULL)
            return EVENT_COLLIDE_PARTICLE;
        else
            return EVENT_COLLIDE_HPLANE;
    } else {
        if (e->particule_b != NULL)
            return EVENT_COLLIDE_HPLANE; // non standard
        else
            return EVENT_REFRESH;
    }
}

bool
event_collide_particle(event_t *result, particle_t *p1, particle_t *p2)
{
    time_t dt = time_before_contact(p1, p2);
    if (!IS_FUTURE_TIME(dt)) return false;
    result->timestamp = p1->timestamp + dt;
    result->particule_a = p1;
    result->particule_a_col = p1->col_counter;
    result->particule_b = p2;
    result->particule_b_col = p2->col_counter;
    return true;
}

bool
event_collide_hplane(event_t *result, particle_t *p, size_t dim, loc_t pos)
{
    time_t dt = time_before_crossing_hplane(p, dim, pos);
    if (!IS_FUTURE_TIME(dt)) return false;
    result->timestamp = p->timestamp + dt;
    result->particule_a = p;
    result->particule_a_col = p->col_counter;
    result->particule_b = NULL;
    result->particule_b_col = dim;
    return true;
}

bool
event_refresh(event_t *result, time_t timestamp)
{
    result->timestamp = timestamp;
    result->particule_a = NULL;
    result->particule_a_col = 0;
    result->particule_b = NULL;
    result->particule_b_col = 0;
    return true;
}
