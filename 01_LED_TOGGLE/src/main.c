#include <stm32f401xe.h>

void delay(int);

void delay(int k ){

	int i ;

	for (i=0;i<4096*k;i++);

	return;

}



int main (){

	RCC ->AHB1ENR=1;

	GPIOA->MODER=0xA8000000;

	GPIOA->MODER=0x0000400;

	GPIOA->ODR=0x0020;

	while (1){

		GPIOA->ODR=0x0020;

		delay(100);

		GPIOA->ODR=0x0000;

		delay(100);



	}

}