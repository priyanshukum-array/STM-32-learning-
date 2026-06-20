#include "stm32f401xe.h"
int main(){
    RCC ->AHB1ENR=1;
    GPIOA ->MODER=0X00000040;//declared pa.3 as ouput and .4 as input 
    GPIOA ->ODR=0x08;
    int led_state=0;

    while (1)
    {
        if(!(GPIOA->IDR & 0x00000010))   // button pressed
        {
            led_state = !led_state;

            if(led_state)
                GPIOA->ODR = 0X00000000;
            else
                GPIOA->ODR = 0x08;

            while(!(GPIOA->IDR & 0x00000010)); // wait for release
        }
    }
}