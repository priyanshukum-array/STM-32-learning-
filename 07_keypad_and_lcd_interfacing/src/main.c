#include <stm32f401xe.h>

// Function Prototypes
void MSDelay(unsigned int);
void KeypadInit(void);
char KeyScan(void);
void lcdprint(char);
void lcdinit(void);
void sendcmd(char);
void printstr(char*);
void MSDelay(unsigned int Times);

// Delay Function
void MSDelay(unsigned int Times)
{
    unsigned int i, j;
    for(i = 0; i < Times; i++)
    {
        for(j = 0; j < 1400; j++);
    }
}

// Matrix Keypad Scanning Function
char KeyScan(void)
{
    int temp;
    while(1)
    {
        GPIOB->ODR &= ~0x000000FF;
        while((GPIOB->IDR & 0x70) == 0x70);
        
        // --- Scan Row 1 ---
        GPIOB->ODR &= ~0x00FF;
        GPIOB->ODR |= 0x0E;
        temp = GPIOB->IDR & 0x70;
        switch(temp)
        {
            case 0X60: return 1; break;
            case 0X50: return 2; break;
            case 0X30: return 3; break;
        }
        
        // --- Scan Row 2 ---
        GPIOB->ODR &= ~0x00FF;
        GPIOB->ODR |= 0x0D;
        temp = GPIOB->IDR & 0x70;
        switch(temp)
        {
            case 0X60: return 4; break;
            case 0X50: return 5; break;
            case 0X30: return 6; break;
        }
        
        // --- Scan Row 3 ---
        GPIOB->ODR &= ~0x00FF;
        GPIOB->ODR |= 0x0B;
        temp = GPIOB->IDR & 0x70;
        switch(temp)
        {
            case 0X60: return 7; break;
            case 0X50: return 8; break;
            case 0X30: return 9; break;
        }
        
        // --- Scan Row 4 ---
        GPIOB->ODR &= ~0x00FF;
        GPIOB->ODR |= 0x07;
        temp = GPIOB->IDR & 0x70;
        switch(temp)
        {
            case 0X60: return 11; break;
            case 0X50: return 10; break;
            case 0X30: return 12; break;
        }
    }
}

// Keypad Initialization
void KeypadInit(void)
{
    GPIOB->MODER &= ~(0xFF003FFF);
    GPIOB->MODER |=  (0x55000055);
    GPIOB->PUPDR &= ~(0x00003F00);
    GPIOB->PUPDR |=  (0x00001500);
}

// LCD Print Character
void lcdprint(char ch)
{
    GPIOA->ODR &= ~0xFF03;
    GPIOA->ODR |= ch << 8;
    GPIOA->ODR |= 0x01; // Making RS high to tell its data
    GPIOA->ODR |= 0x02; // Making enable high
    MSDelay(5);
    GPIOA->ODR &= ~0x02; // Making enable low
}

// LCD Send Command
void sendcmd(char ch)
{
    GPIOA->ODR &= ~0xFF03;
    GPIOA->ODR |= ch << 8;
    GPIOA->ODR &= ~0x01; // Making RS low for command
    GPIOA->ODR |= 0x02;  // Making enable high
    MSDelay(5);
    GPIOA->ODR &= ~0x02; // Making enable low
}

// LCD Initialization
void lcdinit(void)
{
    GPIOA->MODER &= ~0xFFFF000F;
    GPIOA->MODER |=  0x55550005;
    sendcmd(0x38); // 16 column 2 row mode
    sendcmd(0x0F); // Display on cursor blinking
    sendcmd(0x06); // Auto increment
    sendcmd(0x01); // Clear display
}

// Print String Function
void printstr(char *ch)
{
    int i;
    for(i = 0; ch[i] != '\0'; i++)
    {
        lcdprint(ch[i]);
    }
}

// Main Function Execution
int main(void)
{
    char key;
    RCC->AHB1ENR |= 3; // Enable clocks for Port A and Port B
    KeypadInit();
    lcdinit();
    
    while(1)
    {
        key = KeyScan();
        
        // Reset/Output logic matching your diagram configuration
        GPIOB->ODR &= ~0xF000;
        GPIOB->ODR |= key << 12;
        
        if(key == 11)
        {
            sendcmd(0xC0);
        }
        else if(key == 12)
        {
            sendcmd(0x01);
        }
        else if(key == 10)
        {
            printstr("10");
        }
        else
        {
            lcdprint(key + 48); // Convert raw digit value to ASCII
        }
        MSDelay(500);
    }
}