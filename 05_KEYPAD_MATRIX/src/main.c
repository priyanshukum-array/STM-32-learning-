#include "stm32f401xe.h"

// Prototypes
int keyscan_col1(void);
int keyscan_col2(void);
int keyscan_col3(void);
void delay(volatile uint32_t ms);

// Global state variable
int key = -1;

/* * LED Lookup Table:
 * Pins PB12, PB13, PB14, and PB15 are our outputs (Page 15).
 * We shift the raw binary values up by 12 bits so they match the actual 
 * hardware positions of your 4 LEDs in the ODR register.
 */
uint32_t led_pattern[12] =
{
    (0x0 << 12), // 0  -> 0000 binary
    (0x1 << 12), // 1  -> 0001 binary
    (0x2 << 12), // 2  -> 0010 binary
    (0x3 << 12), // 3  -> 0011 binary
    (0x4 << 12), // 4  -> 0100 binary
    (0x5 << 12), // 5  -> 0101 binary
    (0x6 << 12), // 6  -> 0110 binary
    (0x7 << 12), // 7  -> 0111 binary
    (0x8 << 12), // 8  -> 1000 binary
    (0x9 << 12), // 9  -> 1001 binary
    (0xA << 12), // 10 -> '*' key (Displays 10 in binary: 1010)
    (0xB << 12)  // 11 -> '#' key (Displays 11 in binary: 1011)
};

int main(void)
{
    /* 1. Enable Port B Clock (Page 19) */
    RCC->AHB1ENR |= 0x2;

    /* * 2. Configure GPIO Port B Modes (Page 13)
     * - PB0, PB1, PB2   : Inputs (00) for columns
     * - PB3, PB4, PB5, PB6: Outputs (01) for keypad rows
     * - PB12, PB13, PB14, PB15: Outputs (01) for LEDs
     * * Mask calculation: 0x55001540
     */
    GPIOB->MODER = 0x55001540; 

    /* * 3. Configure Internal Pull-up/Pull-down (Page 14)
     * - PB0, PB1, PB2 need internal pull-up resistors (01) so they sit at 
     * logic HIGH when no key is actively pressed.
     * Binary configuration: 00010101 -> 0x15
     */
    GPIOB->PUPDR = 0x15;

    /* 4. Idle State Configuration (Page 15) */
    // Keep rows (PB3-PB6) initially high, and clear our status LEDs
    GPIOB->ODR = 0x0078; 

    while (1)
    {
        // Sample columns continuously to check for user interaction
        if ((GPIOB->IDR & (1 << 0)) == 0)      // Check Col 1 (PB0)
        {
            key = keyscan_col1();
            while((GPIOB->IDR & (1 << 0)) == 0); // Simple debounce: wait for release
        }
        else if ((GPIOB->IDR & (1 << 1)) == 0) // Check Col 2 (PB1)
        {
            key = keyscan_col2();
            while((GPIOB->IDR & (1 << 1)) == 0); // Simple debounce: wait for release
        }
        else if ((GPIOB->IDR & (1 << 2)) == 0) // Check Col 3 (PB2)
        {
            key = keyscan_col3();
            while((GPIOB->IDR & (1 << 2)) == 0); // Simple debounce: wait for release
        }

        /* 5. Update Status Display (Page 15) */
        if (key != -1)
        {
            // Clear only the upper LED pins (PB12-PB15) without wiping out row configurations
            GPIOB->ODR &= ~0xF000; 
            // Push our newly isolated binary lookup pattern out to the hardware
            GPIOB->ODR |= led_pattern[key];
            
            key = -1; // Reset latch state
        }
        
        delay(10); // Small stability delay
    }
}

/* --- COLUMN 1 SCANNING LOGIC --- */
int keyscan_col1(void)
{
    // Ground Row 1 (PB3), set others high -> 0111 0000 -> 0x70
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x70; 
    if ((GPIOB->IDR & (1 << 0)) == 0) return 1;

    // Ground Row 2 (PB4), set others high -> 0110 1000 -> 0x68
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x68; 
    if ((GPIOB->IDR & (1 << 0)) == 0) return 4;

    // Ground Row 3 (PB5), set others high -> 0101 1000 -> 0x58
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x58; 
    if ((GPIOB->IDR & (1 << 0)) == 0) return 7;

    // Ground Row 4 (PB6), set others high -> 0011 1000 -> 0x38
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x38;
    if ((GPIOB->IDR & (1 << 0)) == 0) return 10; // Latch value 10 for '*'

    return -1;
}

/* --- COLUMN 2 SCANNING LOGIC --- */
int keyscan_col2(void)
{
    // Ground Row 1 (PB3) -> 0x70
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x70; 
    if ((GPIOB->IDR & (1 << 1)) == 0) return 2;

    // Ground Row 2 (PB4) -> 0x68
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x68; 
    if ((GPIOB->IDR & (1 << 1)) == 0) return 5;

    // Ground Row 3 (PB5) -> 0x58
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x58; 
    if ((GPIOB->IDR & (1 << 1)) == 0) return 8;

    // Ground Row 4 (PB6) -> 0x38
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x38; 
    if ((GPIOB->IDR & (1 << 1)) == 0) return 0; // Key '0' is located here!

    return -1;
}

/* --- COLUMN 3 SCANNING LOGIC --- */
int keyscan_col3(void)
{
    // Ground Row 1 (PB3) -> 0x70
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x70; 
    if ((GPIOB->IDR & (1 << 2)) == 0) return 3;

    // Ground Row 2 (PB4) -> 0x68
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x68; 
    if ((GPIOB->IDR & (1 << 2)) == 0) return 6;

    // Ground Row 3 (PB5) -> 0x58
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x58; 
    if ((GPIOB->IDR & (1 << 2)) == 0) return 9;

    // Ground Row 4 (PB6) -> 0x38
    GPIOB->ODR = (GPIOB->ODR & ~0x78) | 0x38; 
    if ((GPIOB->IDR & (1 << 2)) == 0) return 11; // Latch value 11 for '#'

    return -1;
}

/* Basic hardware execution block delay loops */
void delay(volatile uint32_t ms)
{
    while(ms--)
    {
        for(volatile int i = 0; i < 1600; i++);
    }
}