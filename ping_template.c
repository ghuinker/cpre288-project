/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"
#include "driverlib/interrupt.h"
#include "lcd.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

  // YOUR CODE HERE

    SYSCTL_RCGCGPIO_R |= 0x2;
    while((SYSCTL_PRGPIO_R & 0x2) == 0){}
    SYSCTL_RCGCTIMER_R |= 0x8;
    while((SYSCTL_PRTIMER_R & 0x8) == 0){}
    GPIO_PORTB_DEN_R |= 0x8;
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R |= (GPIO_PORTB_PCTL_R & 0xFFFF0FFF) | 0x00007000;

    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R = 0x4;
    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBMR_R &= ~0x10;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_IMR_R |= 0x400;
    TIMER3_ICR_R |= 0x400;

    // Configure and enable the timer



    NVIC_PRI9_R |= 0xE0;
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    TIMER3_CTL_R |= 0x100;
}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;


    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x8;

    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DATA_R &= ~0x08;
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x08;

    TIMER3_ICR_R |= 0x400;

    GPIO_PORTB_AFSEL_R |= 0x8;

    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;


}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
    //if(TIMER3_MIS_R & 0x400){

        //TIMER3_ICR_R &= ~0x400;
        TIMER3_ICR_R |= 0x400;

        if(STATE == LOW){

            START_TIME = TIMER3_TBV_R;
            STATE = HIGH;



        }
        else if(STATE == HIGH){

            END_TIME = TIMER3_TBV_R;
            STATE = DONE;
        }

   // }



}

float ping_getDistance (void){

    // YOUR CODE HERE
    unsigned long tempS, tempE;


    unsigned long time_dif = 0;
    unsigned long overflow_count = 0;

    uint8_t overflow = 0;

    float dist = 0;

    ping_trigger();

    while(STATE != DONE){

    }

    tempS = START_TIME;
    tempE = END_TIME;

    overflow = END_TIME > START_TIME;

    if(overflow){

        overflow_count += 1;

    }
    time_dif = (START_TIME - END_TIME) + ((unsigned long) overflow_count);

    dist = time_dif * 6.25e-8 * 343 * 100 / 2;

    return dist;

}
