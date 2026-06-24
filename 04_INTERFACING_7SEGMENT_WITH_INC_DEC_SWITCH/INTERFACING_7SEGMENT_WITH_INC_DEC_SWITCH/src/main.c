#include "stm32f401xe.h"

int main()
{
     //PA.0 will read from button and will increment the value when switch is pressed 
    //PA.1 TO PA.7 will serve as output to 7 segemtn display

    int num[10] =
    {
        0x7E,//0
        0x0C,//1
        0xB6,//2
        0x9E,//3
        0xCC,//4
        0xDA,//5
        0xFA,//6
        0x0E,//7
        0xFE,//8
        0xCE//9
    };
    int count=0;
    RCC->AHB1ENR = 1;// enabling clk of port A 
    GPIOA -> MODER&=~0XFFFFFFFF;
    GPIOA ->MODER|=0X55555550;
    GPIOA -> ODR|=(0x7E)<<1;//INITIAL DISPLAY 00
    GPIOA -> ODR|=(0x7E)<<8;
    
    while (1){
        if  ((GPIOA->IDR & 0x01) == 0){
            count++;

            GPIOA->ODR = ((num[(count % 10)])<<1)|((num[(((count / 10))%10)])<<8);
            

            while ((GPIOA->IDR & 0x01) == 0)
            {
                // Wait for button release
            }
        }
        if ((GPIOA ->IDR & 0x02)==0){
            count--;

            GPIOA->ODR = ((num[(count % 10)])<<1)|(((num[(count / 10)%10]))<<8);
            

            while ((GPIOA->IDR & 0x02) == 0)
            {
                // Wait for button release
            }
        }
    }
        
    }

    
