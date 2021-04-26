/*
 * movement.c
 *
 *  Created on: Feb 9, 2021
 *      Author: ghuinker
 */
#include "open_interface.h"


double  move_forward (oi_t  *sensor_data,   double distance_mm)
{
    double sum = 0;
    oi_setWheels(500,500); //move forward at full speed
    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        if(sensor_data->bumpLeft || sensor_data->bumpRight){
            oi_setWheels(0,0);
            return sum;
        }
    }
    oi_setWheels(0,0); //stop
    return sum;
}

double move_backward (oi_t  *sensor_data,   double distance_mm)
{
    double sum = 0;
    oi_setWheels(-500,-500); //move forward at full speed
    while (sum > distance_mm * -1) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}

void turn_right(oi_t  *sensor, double degrees)
{
    double sum = 0;
    oi_setWheels(250, -250);
    while (sum < degrees){
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0,0);
}

void turn_left(oi_t  *sensor, double degrees)
{
    double sum = 360;
    oi_setWheels(-250, 250);
    while (sum > 360 - degrees){
        oi_update(sensor);

        sum += sensor->angle;
    }
    oi_setWheels(0,0);
}
