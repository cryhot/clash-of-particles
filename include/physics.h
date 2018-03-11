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


/** @brief An alias to the type used for times. */
typedef double time_t;

/** @brief An alias to the type used for spatial locations. */
typedef double loc_t;

#ifndef EPS
/** @brief An constant for the spatial epsilon value. */
#define EPS 1e-16
#endif

/** @brief Checks if a time represent a future but still reachable event. */
#define IS_FUTURE_TIME(t) (isfinite(t) && 0 < (t))

/** @brief Constant for a non-existant time. */
#define NEVER NAN;

/** @brief Checks if a location value is zero, considering the epsilon value. */
#define EQ_LOC_ZERO(l) (fabs(l) < EPS)


/** @brief Compute the time to achieve a certain distance at a certain speed.
 * @param dist  length of the "path"
 * @param speed  velocity along the path
 * @return  time to achieve the path at the given speed.
 */
time_t path_time(loc_t dist, loc_t speed);


/** @brief Add in place the second vector multiplied by a coefficient to the first vector.
 * @param loc   first vector, modified in place
 * @param loc2  second vector to append
 * @param k     multiplier of the second vector
 */
void loc_append (loc_t loc[NB_DIM], loc_t const loc2[NB_DIM], long double k);

/** @brief Compute the difference of two vectors.
 *
 * The result (`loc1-loc2`) is stored in `ans`
 * @param loc1  first vector
 * @param loc2  second vector
 * @param ans   vector to modify
 */
void loc_delta (loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM], loc_t ans[NB_DIM]);

/** @brief Compute the scalar product of two vectors.
 * @param loc1  first vector
 * @param loc2  second vector
 * @return  the scalar product `loc1 . loc2`
 */
loc_t loc_scal_prod (loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM]);

/** @brief Compute the euclidean distance between two vectors.
 * @param loc1  first vector
 * @param loc2  second vector
 * @return  distance between the two locations
 */
loc_t loc_distance (loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM]);

/** @brief update a given position and velocity according to a time interval.
 * @param pos  position vector
 * @param vel  velocity vector
 * @param dt  relative time between the beginning and the ending of the update
 */
void coords_update (loc_t pos[NB_DIM], loc_t vel[NB_DIM], time_t dt);

#endif
