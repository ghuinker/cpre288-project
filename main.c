
#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "movement.h"
#include "open_interface.h"

// Window: 80x25

int MOVEMENT_DIST = 50;
int main(void)
{
    button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();

    uart_init();
    oi_t *sensor_data = oi_alloc();

    oi_init(sensor_data);

    while (1)
    {
        char c = 0;
        c = uart_receive();
        if (c == 'w'){
            move_forward(sensor_data, MOVEMENT_DIST);
        }
        if(c == 's'){
            move_backward(sensor_data, MOVEMENT_DIST);
        }
        if (c == 'a'){
            turn_left(sensor_data, 25);
        }
        if (c == 'd'){
            turn_right(sensor_data, 25);

        }

    }

}
