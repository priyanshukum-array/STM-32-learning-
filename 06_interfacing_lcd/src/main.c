#include "stm32f401xe.h"

void MSDelay(unsigned int Times);
void lcdprint(char ch);
void sendcmd(char ch);
void lcdinit(void);
void printstr(char *ch);

void lcdprint(char ch)
{
    GPIOA->ODR &= ~0xFF03;
    GPIOA->ODR |= ((uint32_t)(uint8_t)ch << 8);
    GPIOA->ODR |= 0x1;        // RS = 1 (Data)
    GPIOA->ODR |= 0x2;        // EN = 1
    MSDelay(5);
    GPIOA->ODR &= ~0x2;       // EN = 0
    MSDelay(2);
}

void sendcmd(char ch)
{
    GPIOA->ODR &= ~0xFF03;
    GPIOA->ODR |= ((uint32_t)(uint8_t)ch << 8);
    GPIOA->ODR &= ~0x1;       // RS = 0 (Command)
    GPIOA->ODR |= 0x2;        // EN = 1
    MSDelay(50);
    GPIOA->ODR &= ~0x2;       // EN = 0
    MSDelay(50);
}

void lcdinit(void)
{
    MSDelay(20);      // Wait after power-up
    sendcmd(0x38);    // 8-bit, 2-line, 5x8 font
    sendcmd(0x0F);    // Display ON, Cursor ON, Blink ON
    sendcmd(0x06);    // Auto increment cursor
    sendcmd(0x01);    // Clear display
    MSDelay(500);       // Clear display needs extra delay
}

void printstr(char *ch)
{
    for (int i = 0; ch[i] != '\0'; i++)
    {
        lcdprint(ch[i]);
    }
}

void MSDelay(unsigned int Times)
{
    unsigned int i, j;
    for (i = 0; i < Times; i++)
    {
        for (j = 0; j < 50000; j++);
    }
}

int main(void)
{
    RCC->AHB1ENR |= 0x1;

    GPIOA->MODER &= ~0xFFFF001F;
    GPIOA->MODER |= 0x55550005;

    MSDelay(20);

    lcdinit();

    sendcmd(0x80);          // First line
    printstr(" hello ");

    sendcmd(0xC0);          // Second line
    printstr("world");

    while (1)
    {
    }
}