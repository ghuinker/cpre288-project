/*
 * adc.c
 *
 *  Created on: Mar 16, 2021
 *      Author: wmotting
 */

#include "adc.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "Timer.h"
#include <math.h>
#include <inc/tm4c123gh6pm.h>

void adc_init(){

    SYSCTL_RCGCADC_R |= 0x2;
    SYSCTL_RCGCGPIO_R |= 0X2;
    while ((SYSCTL_PRGPIO_R & 0b00000010) == 0) {};

    GPIO_PORTB_DIR_R &= 0xFFFFFFEF;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= 0xFFFFFFEF;
    GPIO_PORTB_AMSEL_R |= 0x10;
    //GPIO_PORTB_PCTL_R |= 0x00020000;

    while((SYSCTL_PRADC_R & 0b00000010) == 0){};
    ADC1_PC_R &= ~0xF;
    ADC1_PC_R |= 0x1;
    ADC1_SSPRI_R = 0x0123;
    ADC1_ACTSS_R &= ~0x0008;
    ADC1_EMUX_R &= ~0xF000;
    ADC1_SSMUX3_R &= ~0x000F;
    ADC1_SSMUX3_R += 10;
    ADC1_SSCTL3_R |= 0x0006;
    ADC1_IM_R &= ~0x0008;
    ADC1_ACTSS_R |= 0x0008;

    adc_read();

}

int adc_read(){

    uint32_t result;
    ADC1_PSSI_R = 0x8;
    while((ADC1_RIS_R&0x08)==0){
        result = ADC1_SSFIFO3_R & 0xFFF;
    }
    ADC1_ISC_R = 0x8;



    return result;

}

int IR_Distance() {
    int i;
      int rawDistance[10];
      int distances[10];
      int rawSum = 0;
      int sum = 0;

      int avg = 0;
      int rawAvg = 0;

      for(i = 0; i < 10; i++){

             rawDistance[i] = adc_read();
             distances[i] = (164.0*exp(rawDistance[i] * -0.002514) + 36.43 * exp(rawDistance[i] * -0.0004139));
             rawSum += rawDistance[i];
             sum += distances[i];
             timer_waitMillis(20);



         }

         avg = sum / i;

         rawAvg = rawSum / i;
         //lcd_printf("%d - %d", rawAvg, avg );
         return avg;
}
