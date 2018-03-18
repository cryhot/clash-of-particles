/** @file event.h
 *
 * @brief Simple definition of an event.
 *
 * @author Jean-Raphaël GAGLIONE
 *
 */

#ifndef EVENT_H
#define EVENT_H

#include "particle.h"
#include <stdbool.h>

/** @brief Enumeration of the different possible event types.
 * @see event get_event_type
 */
enum event_type {
    /** @brief Collision between two particles.
     *
     * `event.particle_a!=NULL` and `event.particle_b!=NULL`
     *
     * @see new_event_collide_particle
     */
    EVENT_COLLIDE_PARTICLE,

    /** @brief Collision of a particle with an hyperplane.
     *
     * `event.particle_a!=NULL` and `event.particle_b==NULL`
     *
     * Normal dimention to the hyperplane can be retrieved with `event.particle_b_col`.
     * @see new_event_collide_hplane
     */
    EVENT_COLLIDE_HPLANE,

    /** @brief Refreshing event.
     *
     * `event.particle_a==NULL` and `event.particle_b==NULL`
     *
     * `event.particle_a_col` and `event.particle_b_col` can be ignored.
     * @see new_event_refresh
     */
    EVENT_REFRESH,
};

/** @brief An alias to the structure representing the events. */
typedef struct event event_t;

/** @brief The structure representing the events.
 *
 * Here is how to interprete event :
 * - `event.particle_a==NULL` and `event.particle_b==NULL`: refreshing event ({@link EVENT_REFRESH}).
 * - `event.particle_a!=NULL` and `event.particle_b==NULL`: collision with an hyperplane ({@link EVENT_COLLIDE_HPLANE}).
 * Normal dimention to the hyperplane can be retrieved with `event.particle_b_col`.
 * - `event.particle_a!=NULL` and `event.particle_b!=NULL`: collision between two particles ({@link EVENT_COLLIDE_PARTICLE})
 */
struct event {
    /** @brief Absolute time of the event
     */
    time_t timestamp;

    /** @brief Pointer to the first particle involved in the collision
     */
    particle_t *particle_a;

    /** @brief Pointer to the second particle involved in the collision
     */
    particle_t *particle_b;

    /** @brief Number of collision of the first particle when the event was planned.
     *
     * Represents orthogonal dimention when particle is `NULL`,
     * in case of `EVENT_COLLIDE_HPLANE`.
     */
    size_t particle_a_col;

    /** @brief Number of collision of the second particle when the event was planned.
     *
     * Represents orthogonal dimention when particle is `NULL`,
     * in case of `EVENT_COLLIDE_HPLANE`.
     */
    size_t particle_b_col;

};

/** @brief Determine if an event is still valid.
 * @param e  event to be tested
 * @return  `true` if the event is currently valid
 */
bool event_is_valid (event_t *e);

/** @brief Compare two events.
 * @param event1  the first event to compare
 * @param event2  the second event to compare
 * @return  -1,0,1 if the first event is respectively lower, equal, greater
 *          than the second event
 */
int compare_events (void *event1, void *event2);


/** @brief Determine an event type.
 * @param e  event to be tested
 * @return  event type
 */
enum event_type get_event_type (event_t *e);

/** @brief Create a new {@link EVENT_COLLIDE_PARTICLE particle collision event}.
 * @param timestamp  absolute time of the event
 * @param p1  first particle involved in the collision
 * @param p2  second particle involved in the collision
 * @return  the event, which was allocated
 */
event_t *event_collide_particle (time_t timestamp, particle_t *p1, particle_t *p2);

/** @brief Create a new {@link EVENT_COLLIDE_HPLANE hplane collision event}.
 * @param timestamp  absolute time of the event
 * @param p1  the particle involved in the collision
 * @param dim  dimention orthogonal to the hyperplane
 * @return  the event, which was allocated
 */
event_t *event_collide_hplane (time_t timestamp, particle_t *p, size_t dim);

/** @brief Create a new {@link EVENT_REFRESH refresh event}.
 * @param timestamp  absolute time of the event
 * @param result  the refresh event, modified in place
 * @return  the event, which was allocated
 */
event_t *event_refresh (time_t timestamp);

#endif
