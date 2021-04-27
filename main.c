
#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "movement.h"
#include "open_interface.h"
#include "servo.h"
#include "ping.h"
#include "adc.h"
#include "cyBot_FindObjects.h"
#include "resetSimulation.h"
#include <inc/tm4c123gh6pm.h>

// Window: 80x25

int MOVEMENT_DIST = 50;

/*uint32_t wheelDropLeft : 1;
    uint32_t wheelDropRight : 1;
    uint32_t bumpLeft : 1;
    uint32_t bumpRight : 1;
    uint32_t cliffLeft : 1;
    uint32_t cliffFrontLeft : 1;
    uint32_t cliffFrontRight : 1;
    uint32_t cliffRight : 1;

    uint32_t lightBumperRight : 1;
    uint32_t lightBumperFrontRight : 1;
    uint32_t lightBumperCenterRight : 1;
    uint32_t lightBumperCenterLeft : 1;
    uint32_t lightBumperFrontLeft : 1;
    uint32_t lightBumperLeft : 1;

    uint32_t wallSensor : 1;
    uint32_t virtualWall : 1;*/

void print_screen(oi_t  *sensor_data, bool backup_warning, cyBot_Object_t objects[], int objects_found, int ping_distances[])
{
    // Clear the screen
    char temp[80];
    temp[25] = '\0';

    uart_sendStr("\e[1;1H\e[2J");

    uart_sendStr("Key: wasd-Move, f-Quick Scan, F - Full Scan");
    int i=0;
    for(i=0; i < objects_found; i++){
       sprintf(temp, "Dist %lf, Angle %d, Width: %lf", objects[i].distance, objects[i].angle, objects[i].width);
       uart_sendStr(temp);
    }
    sprintf(temp, "%d", objects_found);
    uart_sendStr(temp);

    uart_sendStr("PING");
    for(i = 0; i< 10; i++){
        sprintf(temp, "%d", ping_distances[i]);
        uart_sendStr(temp);
    }

    uart_sendStr("");
    for(i = 0; i< 80; i++){
        if(i == 40)
            uart_sendChar('U');
        else
            uart_sendChar(' ');
    }
    uart_sendStr("");

    // Sensors
    uart_sendStr("Sensors:");
    if(sensor_data->bumpLeft)
        uart_sendStr("Bump Left");
    if(sensor_data->bumpRight)
        uart_sendStr("Bump Right");

    if(sensor_data->wheelDropLeft)
        uart_sendStr("Wheel Drop Left");
    if(sensor_data->wheelDropRight)
        uart_sendStr("Wheel Drop Right");

    if(sensor_data->cliffFrontLeftSignal != 1000)
        uart_sendStr("Cliff Front Left");
    if(sensor_data->cliffFrontRightSignal != 1000)
        uart_sendStr("Cliff Front Right");
    if(sensor_data->cliffRightSignal != 1000)
        uart_sendStr("Cliff Right");
    if(sensor_data->cliffLeftSignal != 1000)
        uart_sendStr("Cliff Left");
    if(sensor_data->wallSensor)
        uart_sendStr("Wall");
    if(sensor_data->virtualWall)
        uart_sendStr("Virtual Wall");

    // Warnings
    if(backup_warning)
        uart_sendStr("Please Back UP!!");

    // Print Field
}

bool needs_to_backup(oi_t *sensor_data)
{
    if(sensor_data->bumpLeft)
        return true;
    if(sensor_data->bumpRight)
        return true;
    if(sensor_data->wheelDropLeft)
        return true;
    if(sensor_data->wheelDropRight)
        return true;
    if(sensor_data->cliffFrontLeftSignal != 1000)
        return true;
    if(sensor_data->cliffFrontRightSignal != 1000)
        return true;
    if(sensor_data->cliffRightSignal != 1000)
        return true;
    if(sensor_data->cliffLeftSignal != 1000)
        return true;
    if(sensor_data->wallSensor)
        return true;
    if(sensor_data->virtualWall)
        return true;
    return false;
}

int main(void)
{

    uart_init();
    servo_init();

    ping_init();
    adc_init();

    //button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();


    oi_t *sensor_data = oi_alloc();

    oi_init(sensor_data);


    int frames = 0;
    bool backup_warning = false;
    bool has_moved = false;
    int ping_dist = -1;
    int i =0;
    int objects_found = 0;



    servo_move(90);

    int num_objects = 10;
    cyBot_Object_t objects[num_objects];

    int ping_distances[10];





    while (1)
    {
        char c = 0;
        c = uart_receive();
        if(c == 's'){
            move_backward(sensor_data, MOVEMENT_DIST);
            backup_warning = false;
        }
        else if(needs_to_backup(sensor_data)){
            backup_warning = true;
        }
        else if (c == 'w'){
            move_forward(sensor_data, MOVEMENT_DIST);
        }
        else if (c == 'a'){
            turn_left(sensor_data, 25);
        }
        else if (c == 'd'){
            turn_right(sensor_data, 25);
        }
        else if(c == 'f'){
            objects_found = 0;

            for(i=0; i<10; i++){
                servo_move(40 + (i*10));
                timer_waitMillis(100);
                ping_distances[i] = ping_getDistance();
            }
        }
        else if(c == 'F'){
            objects_found = cyBot_FindObjects(objects, num_objects);
            servo_move(90);
        }
        // Redraw Putty
        if (frames % 100000 == 0)
        {
            print_screen(sensor_data, backup_warning, objects, objects_found, ping_distances);
            uart_sendStr("Current Key: \n\r");
            uart_sendChar(c);
        } else {
            frames++;
        }

    }

}
