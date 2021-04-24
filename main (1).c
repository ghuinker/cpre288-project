/*
 * main.c
 *
 *  Created on: Apr 22, 2021
 *      Author: brnorman
 */
#include "resetSimulation.h"
#include "servo.h"
#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include <stdio.h>
#include <math.h>
#include <inc/tm4c123gh6pm.h>
#include "cyBot_FindObjects.h"
#include "open_interface.h"


int main(void){
    //resetSimulationBoard();
    servo_init();
    ping_init();
    adc_init();

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();

    oi_t*sensor_data=oi_alloc();
    oi_init(sensor_data);


    int num_objects=3;
    cyBot_Object_t object_data[num_objects];

    cyBot_FindObjects(object_data,num_objects);













}






