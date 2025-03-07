#include <msp430.h>
#include <stdio.h>

// Define LCD pins
#define RS BIT0
#define EN BIT1
#define D4 BIT2
#define D5 BIT3
#define D6 BIT4
#define D7 BIT5

#define LCD_PORT P2OUT
#define LCD_DIR P2DIR

volatile unsigned long millis_count = 0;  // Time counter

// Function Prototypes
void initClock();
void initTimerA();
void delay_ms(unsigned int ms);
void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_init();
void LCD_sendString(char *str);
void LCD_setCursor(unsigned char row, unsigned char col);
void printTimeLCD();

int main() {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    initClock();  // Set clock to 1MHz
    initTimerA(); // Initialize TimerA
    LCD_init();   // Initialize LCD

    while (1) {
        printTimeLCD(); // Print time on LCD
        delay_ms(1000);
    }
}

// Initialize internal clock to 1MHz
void initClock() {
    BCSCTL1 = CALBC1_1MHZ;  
    DCOCTL = CALDCO_1MHZ;
}

// Initialize TimerA to count milliseconds
void initTimerA() {
    TA0CCR0 = 1000 - 1;  // 1ms tick at 1MHz
    TA0CCTL0 = CCIE;     // Enable TimerA interrupt
    TA0CTL = TASSEL_2 | MC_1;  // SMCLK, Up mode
    __enable_interrupt();
}

// TimerA Interrupt Service Routine (ISR)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void) {
    millis_count++;
}

// Delay function (Software Delay)
void delay_ms(unsigned int ms) {
    while (ms--) __delay_cycles(1000);  // 1MHz clock, so 1000 cycles = 1ms
}

// Send command to LCD
void LCD_command(unsigned char cmd) {
    LCD_PORT = (LCD_PORT & 0xC0) | ((cmd >> 4) & 0x0F); // Send upper nibble
    LCD_PORT &= ~RS;
    LCD_PORT |= EN;
    delay_ms(1);
    LCD_PORT &= ~EN;

    LCD_PORT = (LCD_PORT & 0xC0) | (cmd & 0x0F); // Send lower nibble
    LCD_PORT |= EN;
    delay_ms(1);
    LCD_PORT &= ~EN;
}

// Send data to LCD
void LCD_data(unsigned char data) {
    LCD_PORT = (LCD_PORT & 0xC0) | ((data >> 4) & 0x0F); // Upper nibble
    LCD_PORT |= RS;
    LCD_PORT |= EN;
    delay_ms(1);
    LCD_PORT &= ~EN;

    LCD_PORT = (LCD_PORT & 0xC0) | (data & 0x0F); // Lower nibble
    LCD_PORT |= EN;
    delay_ms(1);
    LCD_PORT &= ~EN;
}

// Initialize LCD in 4-bit mode
void LCD_init() {
    LCD_DIR |= (RS | EN | D4 | D5 | D6 | D7);  // Set LCD pins as output
    delay_ms(50);
    
    LCD_command(0x02);  // Initialize in 4-bit mode
    LCD_command(0x28);  // 2-line, 5x8 font
    LCD_command(0x0C);  // Display ON, Cursor OFF
    LCD_command(0x06);  // Auto Increment Cursor
    LCD_command(0x01);  // Clear display
    delay_ms(5);
}

// Set cursor position on LCD
void LCD_setCursor(unsigned char row, unsigned char col) {
    unsigned char pos;
    if (row == 0) pos = 0x80 + col;  // First row
    else pos = 0xC0 + col;  // Second row
    LCD_command(pos);
}

// Send string to LCD
void LCD_sendString(char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

// Print live time to LCD
void printTimeLCD() {
    unsigned long seconds = millis_count / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;

    char buffer[16];
    sprintf(buffer, "Time: %02lu:%02lu:%02lu", hours, minutes, seconds);

    LCD_setCursor(0, 0);
    LCD_sendString(buffer);
}

