#include "adc.h"

void adc_init(void)
{
    //enable clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= 0x02;

    // enable clock on ADC0
    SYSCTL_RCGCADC_R |= 0x01;

    //wait for GPIOB and UART1 peripherals to be ready
    while ((SYSCTL_PRGPIO_R & 0x02) == 0)
    {
    };
    while ((SYSCTL_PRADC_R & 0x01) == 0)
    {
    };

    GPIO_PORTB_DIR_R |= 0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    // 13.4 Sample Sequencer Configuration
    ADC0_ACTSS_R &= ~0x0001;
    ADC0_EMUX_R &= ~0x000F;

    ADC0_SSMUX0_R |= 0x01;
    ADC0_SSMUX0_R += 9;
    ADC0_SSCTL0_R |= 0x6;

    ADC0_ACTSS_R |= 0x01;
}

int adc_read(void)
{
    int result;
    ADC0_PSSI_R = 0x0001;
    while((ADC0_RIS_R & 0x01)==0){};
    result = ADC0_SSFIFO0_R & 0xFFF;
    ADC0_ISC_R |= 0x0001;
    return result;
}
