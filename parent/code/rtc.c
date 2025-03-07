#include <msp430.h>

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(char *str);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    uart_init();               // Initialize UART

    P1DIR |= BIT0;             // Set P1.0 as output (LED)
    P1OUT &= ~BIT0;            // Turn off LED initially

    while (1) {
        while (!(IFG2 & UCA0RXIFG));  // Wait for data from RTC
        char c = UCA0RXBUF;           // Read received byte
        
        P1OUT |= BIT0;                // Turn on LED (data transmission starts)
        uart_send_char(c);            // Send data to Raspberry Pi
        P1OUT &= ~BIT0;               // Turn off LED (data sent)
    }
}

void uart_init(void) {
    P1SEL |= BIT1 + BIT2;   // P1.1 as RX, P1.2 as TX
    P1SEL2 |= BIT1 + BIT2;  
    UCA0CTL1 |= UCSSEL_2;   // Use SMCLK as clock source
    UCA0BR0 = 104;          // Baud rate 9600 (1MHz clock)
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;      // Modulation
    UCA0CTL1 &= ~UCSWRST;   // Initialize USCI state machine
}

void uart_send_char(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait for TX buffer to be empty
    UCA0TXBUF = c;
}

void uart_send_string(char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

