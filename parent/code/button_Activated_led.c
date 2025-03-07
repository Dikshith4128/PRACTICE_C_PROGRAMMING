#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    P1DIR |= BIT0;              // Set P1.0 as output (LED)
    P1DIR &= ~BIT3;             // Set P1.3 as input (Button)
    P1REN |= BIT3;              // Enable pull-up/down resistor
    P1OUT |= BIT3;              // Set pull-up resistor

    while (1) {
        if (!(P1IN & BIT3)) {    // If button is pressed (active low)
            __delay_cycles(20000);  // Simple software debounce
            if (!(P1IN & BIT3)) {  // Confirm button press
                P1OUT ^= BIT0;    // Toggle LED
                while (!(P1IN & BIT3)); // Wait for button release
            }
        }
    }
}

