/** @file physics.h
 *
 * @brief Set of utility functions and constants relative to physics.
 *
 * @author Jean-Raphaël GAGLIONE
 *
 * Vectors are represented as `loc_t[NB_DIM]`
 *
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include <stddef.h>
#include <math.h>

#ifndef NB_DIM
/** @brief An constant for the number of dimentions. */
#define NB_DIM 2
#endif

#ifndef EPS
/** @brief An constant for the spatial epsilon value. */
#define EPS 1e-16
#endif


/** @brief An alias to the type used for times. */
typedef double time_t;

/** @brief An alias to the type used for spatial locations. */
typedef double loc_t;

/** @brief Checks if a time represent a future but still reachable event. */
#define IS_FUTURE_TIME(t) (0 < (t) && (t) < INFINITY)

/** @brief Checks if a location value is zero, considering the epsilon value. */
#define EQ_LOC_ZERO(l) (fabs(l) < EPS)


/** @brief Add in place the second vector multiplied by a coefficient to the first vector.
 * @param loc   first vector, modified in place
 * @param loc2  second vector to append
 * @param k     multiplier of the second vector
 */
void loc_append(loc_t loc[NB_DIM], loc_t const loc2[NB_DIM], time_t k);

/** @brief Compute the euclidean distance between two vectors.
 * @param loc1  first vector
 * @param loc2  second vector
 * @param ans   vector to modify
 */
void loc_delta(loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM], loc_t ans[NB_DIM]);

/** @brief Compute the euclidean distance between two vectors.
 * @param loc1  first vector
 * @param loc2  second vector
 * @return  distance between the two locations
 */
loc_t loc_distance(loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM]);

/** @brief update a given position and velocity according to a time interval.
 * @param pos  position vector
 * @param vel  velocity vector
 * @param dt  relative time between the beginning and the ending of the update
 */
void coords_update(loc_t pos[NB_DIM], loc_t vel[NB_DIM], time_t dt);

#endif
