/**
 * lab6-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include "button.h"
#include "cyBot_uart.h"
#include <math.h>

void sendLine(char *str)
{
    int i;
    int len = strlen(str);

    for (i = 0; i < len; i++)
    {
        cyBot_sendByte(str[i]);
    }
    cyBot_sendByte('\r');
    cyBot_sendByte('\n');
}

int main(void) {
    button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	adc_init();
	cyBot_uart_init();

	int value;
	int i;

	int distance;
	char output[20];



    while(1){
        value = adc_read();
        distance = 111075 * pow(value, -1.16);
        sprintf(output, "%d, %d", value, distance);
        sendLine(output);

        while(!button_getButton()){};
        for(i =0; i < 1000000; i++){};
    }

//  int COUNTS = 100000;
//  int total;
//	while(1){
//	    for(i = 0; i < COUNTS; i++){
//	        total += adc_read();
//	    }
//	    lcd_printf("Average: %d", total/COUNTS);
//
//	    while(!button_getButton()){};
//	    for(i =0; i < 10000; i++){};
//	    total = 0;
//	}

}
