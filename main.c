
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

void print_screen(oi_t  *sensor_data, bool backup_warning, int *ping_distances)
{
    // Clear the screen
    char temp[26];
    temp[25] = '\0';

    uart_sendStr("\e[1;1H\e[2J");

    uart_sendStr("Key: wasd-Move, f-Quick Scan, F - Full Scan");
    int i=0;
    for(i=0; i < 18; i++){
        sprintf(temp, "Ping Dist: %d", ping_distances[i]);
        uart_sendStr(temp);
    }


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

    if(sensor_data->cliffFrontLeft)
        uart_sendStr("Cliff Front Left");
    if(sensor_data->cliffFrontRight)
        uart_sendStr("Cliff Front Right");
    if(sensor_data->cliffRight)
        uart_sendStr("Cliff Right");
    if(sensor_data->cliffLeft)
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
    if(sensor_data->cliffFrontLeft)
        return true;
    if(sensor_data->cliffFrontRight)
        return true;
    if(sensor_data->cliffRight)
        return true;
    if(sensor_data->cliffLeft)
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

    int ping_distances[18];


    servo_move(90);



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
            for(i=0; i< 18; i++){
                servo_move(0 + (i * 10));
                timer_waitMillis(10);
                ping_distances[i] = ping_getDistance();
            }
        }
        // Redraw Putty
        if (frames % 100000 == 0)
        {
            print_screen(sensor_data, backup_warning, ping_distances);
            uart_sendStr("Current Key: \n\r");
            uart_sendChar(c);
        } else {
            frames++;
        }

    }

}
