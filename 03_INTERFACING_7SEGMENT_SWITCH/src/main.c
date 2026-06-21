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

    int count = 0;

    RCC->AHB1ENR = 1;
    GPIOA -> MODER&=~0X0000FFFF;
    // PA0 input, PA1-PA7 output
    GPIOA->MODER |= 0x5554;

    // Display 0 initially
    GPIOA->ODR = 0x7E;

    while (1)
    {
        if ((GPIOA->IDR & 0x01) == 0)
        {
            count++;

            GPIOA->ODR = num[count % 10];

            while ((GPIOA->IDR & 0x01) == 0)
            {
                // Wait for button release
            }
        }
    }
}