/*
 * cyBot_FindObjects.c
 *
 *  Created on: Apr 23, 2021
 *      Author: brnorman
 */
#include <stdio.h>
#include "cyBot_FindObjects.h"
#include "servo.h"
#include "ping.h"
#include "lcd.h"
#include <math.h>
#include "open_interface.h"
#include "movement.h"

#define PI 3.14159265358979323846

/*typedef struct cyBot_FindObjects{

    int angle; //degrees
    float distance; //cm
    float width; //cm


}cyBot_Object_t;*/

int cyBot_FindObjects(cyBot_Object_t getObjects[], int size_of_input){


    int i=0;
    int z=0;
    int temp=0;
    int object_number = 0;
    int angle1 = 0;
    int angle2 = 0;
    int object_angle = 0;
    int a[size_of_input];
    oi_t*sensor_data=oi_alloc();
    oi_init(sensor_data);


    servo_move(0);
    for (i=0;i<181;i++){
        temp = IR_Distance();
        if ((temp > 10) && (temp < 80)){
            angle1=i;
            while ((temp > 10) && (temp < 80)){
                i++;
                servo_move(i);
                temp = IR_Distance();

            }
           angle2=i;
           object_angle=(angle2+angle1)/2;
           a[object_number]=angle2-angle1;
           getObjects[object_number].angle=object_angle;
           object_number++;

        }


        servo_move(i);

    }


    for (z=0;z<size_of_input;z++){
          servo_move(getObjects[z].angle);
          timer_waitMillis(1000);
          getObjects[z].distance = ping_getDistance();
          timer_waitMillis(1000);
          getObjects[z].width = (2*PI*getObjects[z].distance)*((float)a[z] / 360);
          timer_waitMillis(1000);


      }



    /*char header[]="Object# Angle Distance Width\n";
    for (i=0; i<strlen(header); i++){
       cyBot_sendByte(header[i]);
   }*/

    printf("%d ", getObjects[0].angle);
    printf("%d ", getObjects[1].angle);
    printf("%d ", getObjects[2].angle);

    printf("%lf ", getObjects[0].distance);
    printf("%lf ", getObjects[1].distance);
    printf("%lf ", getObjects[2].distance);

    printf("%lf ", getObjects[0].width);
    printf("%lf ", getObjects[1].width);
    printf("%lf ", getObjects[2].width);






return 0;

}


