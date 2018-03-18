/** @file simulation.h
 *
 * @brief Utilitary functions relative to simulation.
 *
 * @author Jean-Raphaël GAGLIONE
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "particle.h"
#include <stddef.h>
#include <stdio.h>

/** @brief Run simulation loop.
 * @param particle_list  list of particles used in the simulation
 * @param nb_part  lenght of `particle_list`
 * @param duration  duration of the simulation (use negative time to run backward)
 * @param callback  callback function (for example a drawing function)
 * @param callback_rate  time between two callback
 */
void simulation_loop (particle_t *particle_list[], size_t nb_part, time_t duration, void (*callback)(time_t timestamp), time_t callback_rate);


/** @brief Fill a list of particles from a file.
 *
 * Values are described as doubles, relative to types unit (location/time/mass).
 * @param particle_list  list to fill
 * @param max_count  max number of particle that can be read - more particles are considered as an error
 * @param file  file from which to read
 * @return  number of particles read, or `-1` if an error occured
 */
size_t load_particles (particle_t *particle_list[], size_t max_count, FILE* file);

/** @brief Fill a list of particles from a file.
 * @param particle_list  list to fill
 * @param max_count  max number of particle that can be read - more particles are considered as an error
 * @param file  file from which to read
 * @return  number of particles read, or `-1` if an error occured
 */
size_t load_raw_particles (particle_t *particle_list[], size_t max_count, FILE* file);

/** @brief Fill a list of particles with random particles.
 *
 * Values are described as raw data type (location/time/mass).
 * @param particle_list  list to fill
 * @param count  number of particle to generate
 */
void generate_particles (particle_t *particle_list[], size_t count);


/** @brief Export a list of particles to a file.
 *
 * Values are described as doubles, relative to types unit (location/time/mass).
 * @param particle_list  list to export
 * @param count  number of particle to export
 * @param file  file to which to write
 */
void export_particles (particle_t *particle_list[], size_t count, FILE* file, char *header);

/** @brief Export a list of particles to a file.
 *
 * Values are described as raw data type (location/time/mass).
 * @param particle_list  list to export
 * @param count  number of particle to export
 * @param file  file to which to write
 */
void export_raw_particles (particle_t *particle_list[], size_t count, FILE* file, char *header);

#endif
