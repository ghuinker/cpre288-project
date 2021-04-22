#include "Timer.h"
#include "lcd.h"
#include <string.h>

int main(void){
   timer_init();
   lcd_init();

   lcd_printf("Hello World");
}
