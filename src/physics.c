#include "physics.h"
#include <math.h>

time_t
path_time(loc_t dist, loc_t speed)
{
    return dist / speed * time_UNIT; // returns +/- inf if speed==0
}


void
loc_append(loc_t loc[NB_DIM], loc_t const loc2[NB_DIM], long double k)
{
    for (size_t d = 0; d < NB_DIM; d++)
        loc[d] += loc2[d] * k;
}

void
loc_delta(loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM], loc_t ans[NB_DIM])
{
    for (size_t d = 0; d < NB_DIM; d++)
        ans[d] = loc1[d] - loc2[d];
}

loc_t
loc_scal_prod(loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM])
{
    loc_t ans = 0;
    for (size_t d = 0; d < NB_DIM; d++)
        ans += loc1[d] * loc2[d];
    return ans;
}

loc_t
loc_distance(loc_t const loc1[NB_DIM], loc_t const loc2[NB_DIM])
{
    loc_t sq_dist = 0;
    for (size_t d = 0; d < NB_DIM; d++) {
        loc_t diff = loc1[d] - loc2[d];
        sq_dist += diff * diff;
    }
    return sqrt(sq_dist);
}

void
coords_update(loc_t pos[NB_DIM], loc_t vel[NB_DIM], time_t dt)
{
    loc_append(pos, vel, (long double)dt/time_UNIT); // uniform motion
}
