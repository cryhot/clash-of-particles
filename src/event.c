#include "event.h"
#include <stdlib.h>

bool
event_is_valid(event_t *e)
{
    if (e->particle_a != NULL && e->particle_a->col_counter != e->particle_a_col)
        return false;
    if (e->particle_b != NULL && e->particle_b->col_counter != e->particle_b_col)
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
    if (e->particle_a != NULL) {
        if (e->particle_b != NULL)
            return EVENT_COLLIDE_PARTICLE;
        else
            return EVENT_COLLIDE_HPLANE;
    } else {
        if (e->particle_b != NULL)
            return EVENT_COLLIDE_HPLANE; // non standard
        else
            return EVENT_REFRESH;
    }
}

event_t *
event_collide_particle(time_t timestamp, particle_t *p1, particle_t *p2)
{
    event_t *event = malloc(sizeof *event);
    event->timestamp = timestamp;
    event->particle_a = p1;
    event->particle_a_col = p1->col_counter;
    event->particle_b = p2;
    event->particle_b_col = p2->col_counter;
    return event;
}

event_t *
event_collide_hplane(time_t timestamp, particle_t *p, size_t dim)
{
    event_t *event = malloc(sizeof *event);
    event->timestamp = timestamp;
    event->particle_a = p;
    event->particle_a_col = p->col_counter;
    event->particle_b = NULL;
    event->particle_b_col = dim;
    return event;
}

event_t *
event_refresh(time_t timestamp)
{
    event_t *event = malloc(sizeof *event);
    event->timestamp = timestamp;
    event->particle_a = NULL;
    event->particle_a_col = 0;
    event->particle_b = NULL;
    event->particle_b_col = 0;
    return event;
}
