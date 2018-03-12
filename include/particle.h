/** @file particle.h
 *
 * @brief Simple definition of a particule.
 *
 * @author Jean-Raphaël GAGLIONE
 *
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "physics.h"

/** @brief An alias to the structure representing the particles. */
typedef struct particle particle_t;

/** @brief The structure representing the particles. */
struct particle {
    /** @brief Collision counter
     *
     * Represents the number of collisions in which the particule
     * has been involved since the beginning of the simulation.
     */
    size_t col_counter;

    /** @brief Absolute time of the snapshot
     *
     * Represents the time at which the particule is described.
     */
    time_t timestamp;

    /** @brief Mass of the particule
     *
     * Represents the mass of the particule.
     *
     * The mass of a particle is assumed to be constant over time.
     */
    mass_t mass;

    /** @brief Radius of the particule
     *
     * Represents the radius of the spherical particule.
     *
     * The radius of a particle is assumed to be constant over time.
     */
    loc_t radius;

    /** @brief Position of the particule at snapshot time
     *
     * Represent the position as a vector of `NB_DIM` dimention.
     *
     * The position is the one at time `timestamp`;
     * the current position can be retrieved at time `t`, assuming only
     * well determined forces have been applied between thoses two times
     * (ie. no collision happened).
     */
    loc_t position[NB_DIM];

    /** @brief Velocity of the particule at snapshot time
     *
     * Represent the velocity as a vector of `NB_DIM` dimention.
     *
     * The velocity is the one at time `timestamp`;
     * the current velocity can be retrieved at time `t`, assuming only
     * well determined forces have been applied between thoses two times
     * (ie. no collision happened).
     */
    loc_t velocity[NB_DIM];
};



/** @brief Update the particle position at a new timestamp.
 *
 * The particle position and velocity are set to their values at snapshot `timestamp`.
 * @param p  particule concerned
 * @param timestamp  absolute timestamp
 */
void update (particle_t *p, time_t timestamp);


/** @brief Compute the time before the the paticle touches the given hyperplane.
 *
 * In 2D, hyperplanes are lines:
 * - for collisions with vertical line, use `dim=0`
 * - for collisions with horizontal line, use `dim=1`
 * @param p  particule concerned
 * @param dim  dimention orthogonal to the hyperplane
 * @param pos  position of that plane along the given dimention axis
 * @return  relative time before crossing
 */
time_t time_before_crossing_hplane (particle_t const *p, size_t dim, loc_t pos);

/** @brief Update the particle location after a collision with the given hyperplane.
 *
 * The collision is assumed to occur at time `timestamp`.
 *
 * In 2D, hyperplanes are lines:
 * - for collisions with vertical line, use `dim=0`
 * - for collisions with horizontal line, use `dim=1`
 * @param p  particule concerned
 * @param dim  dimention orthogonal to the hyperplane
 */
void collide_hplane (particle_t *p, size_t dim);


/** @brief Compute the time at which two particles touch each other.
 *
 * The returned time can be lower than the `timestamp` times, meaning that
 * the collision is a passed event.
 *
 * @param p1  first particule concerned
 * @param p2  second particule concerned
 * @return  relative time of the collision, regarding the timestamp of `p1`
 */
time_t time_before_contact (particle_t const *p1, particle_t const *p2);

/** @brief Update the particles location after a collision between them.
 *
 * The two particles are assumed to have the same `timestamp` time, and
 * the collision is assumed to occur at that time.
 *
 * @param p1  first particule concerned
 * @param p2  second particule concerned
 */
void collide_particule (particle_t *p1, particle_t *p2);

#endif
