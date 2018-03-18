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

/** @brief A macro to calculate EPS
 * @param type  type of the EPS
 * @param unit  unit of the EPS
 * @param eps  fraction of the eps (between `0` and `1`)
 */
#define EPS(type,unit,eps) ((type)((unit)*(eps)))

/** @brief An alias to the type used for times. */
typedef long double time_t;
/** @brief The printing format relative to the time type. */
#define time_F "Lf"
/** @brief A constant for the temporal unit. */
#define time_UNIT 1.0
#ifndef time_EPS
/** @brief A constant for the temporal epsilon value, relative to the unit. */
#define time_EPS 1e-15
#endif

/** @brief An alias to the type used for spatial locations. */
typedef long double loc_t;
/** @brief The printing format relative to the location type. */
#define loc_F "Lf"
/** @brief A constant for the spatial unit. */
#define loc_UNIT 1.0
#ifndef loc_EPS
/** @brief A constant for the spatial epsilon value, relative to the unit. */
#define loc_EPS 1e-16
#endif

/** @brief An alias to the type used for masses. */
typedef double mass_t;
/** @brief The printing format relative to the mass type. */
#define mass_F "lf"
/** @brief A constant for the mass unit. */
#define mass_UNIT 1.0

/** @brief Checks if a time represent a future but still reachable event. */
#define IS_FUTURE_TIME(t) (isfinite(t) && -time_EPS <= (t))

/** @brief Checks if a time is before an other time. */
#define IS_BEFORE(t1, t2) (isless((t1),(t2)) || (!isfinite(t2) && isfinite(t1)))

/** @brief Constant for a non-existant time. */
#define NEVER NAN

/** @brief Checks if a time value is zero, considering the epsilon value. */
#define EQ_TIME_ZERO(l) (fabs(l) <= EPS(time_t,time_UNIT,time_EPS))

/** @brief Checks if a location value is zero, considering the epsilon value. */
#define EQ_LOC_ZERO(l) (fabs(l) <= EPS(loc_t,loc_UNIT,loc_EPS))


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
