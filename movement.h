/*
 * movement.h
 *
 *  Created on: Feb 9, 2021
 *      Author: ghuinker
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"


double  move_forward (oi_t  *sensor_data,   double distance_mm);

double  move_backward (oi_t  *sensor_data,   double distance_mm);

void turn_left(oi_t  *sensor, double degrees);

void turn_right(oi_t  *sensor,    double degrees);



#endif /* MOVEMENT_H_ */
