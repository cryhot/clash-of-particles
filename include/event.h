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

#define EVENT_COLLIDE_PARTICLE  1
#define EVENT_COLLIDE_HPLANE    2
#define EVENT_REFRESH           3

/** @brief An alias to the structure representing the events. */
typedef struct event event_t;

/** @brief The structure representing the events.
 *
 * Here is how to interprete event :
 * - `particule_a==NULL` and `particule_b==NULL`: refreshing event ({@link EVENT_REFRESH}).
 * - `particule_a!=NULL` and `particule_b==NULL`: collision with an hyperplane (EVENT_COLLIDE_HPLANE).
 * Normal dimention ton the hyperplane can be retrieved with `particule_b_col`.
 * - `particule_a!=NULL` and `particule_b!=NULL`: collision between two particles (EVENT_COLLIDE_PARTICLE)
 */
struct event {
    /** @brief Absolute time of the event
     */
    time_t timestamp;

    /** @brief Pointer to the first particle involved in the collision
     */
    particle_t *particule_a;

    /** @brief Pointer to the second particle involved in the collision
     */
    particle_t *particule_b;

    /** @brief Number of collision of the first particule when the event was planned.
     *
     * Represents orthogonal dimention when particule is `NULL`,
     * in case of `EVENT_COLLIDE_HPLANE`.
     */
    size_t particule_a_col;

    /** @brief Number of collision of the second particule when the event was planned.
     *
     * Represents orthogonal dimention when particule is `NULL`,
     * in case of `EVENT_COLLIDE_HPLANE`.
     */
    size_t particule_b_col;

};

/** @brief Determine if an event is still valid.
 * @param e  event to be tested
 * @return  `true` if the event is currently valid
 */
bool event_is_valid(event_t *e);

#endif
