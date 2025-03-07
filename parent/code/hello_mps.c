#include <msp430.h>

volatile unsigned long seconds = 0;
volatile unsigned int minutes = 0;
volatile unsigned int hours = 0;

void initClock();
void initUART();
void initTimerA();
void sendString(const char *str);
void sendNumber(unsigned int num);

void main() {
    WDTCTL = WDTPW | WDTHOLD;  // Stop Watchdog Timer
    initClock();               // Set clock speed (DCO)
    initUART();                // Initialize UART
    initTimerA();              // Start Timer_A for 1s tick

    sendString("MSP430G2553 Live Time Counter:\r\n");

    while (1) {
        // Print the current time every second
        sendString("Time: ");
        sendNumber(hours);
        sendString(":");
        if (minutes < 10) sendString("0");  // Add leading zero
        sendNumber(minutes);
        sendString(":");
        if (seconds < 10) sendString("0");  // Add leading zero
        sendNumber(seconds);
        sendString("\r\n");

        __delay_cycles(1000000);  // Wait ~1 second (for UART stability)
    }
}

// --------------------------
// Clock Initialization (DCO)
// --------------------------
void initClock() {
    BCSCTL1 = CALBC1_1MHZ;  // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;
}

// --------------------------
// UART Initialization (9600 baud)
// --------------------------
void initUART() {
    P1SEL |= BIT1 | BIT2;  // Set P1.1 = RXD, P1.2 = TXD
    P1SEL2 |= BIT1 | BIT2;

    UCA0CTL1 |= UCSSEL_2;  // Use SMCLK (1MHz)
    UCA0BR0 = 104;         // 1MHz / 9600 = 104
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;     // Modulation
    UCA0CTL1 &= ~UCSWRST;  // Start UART
}

// --------------------------
// Timer_A Initialization (1s Interrupt)
// --------------------------
void initTimerA() {
    TA0CCR0 = 32768 - 1;  // 32,768 cycles = ~1s with ACLK (assuming 32.768kHz XTAL)
    TA0CCTL0 = CCIE;      // Enable Timer_A interrupt
    TA0CTL = TASSEL_1 | MC_1;  // ACLK, Up mode
    __enable_interrupt();  // Enable global interrupts
}

// --------------------------
// Timer_A ISR (Updates hh:mm:ss every 1 second)
// --------------------------
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void) {
    seconds++;
    if (seconds >= 60) {
        seconds = 0;
        minutes++;
        if (minutes >= 60) {
            minutes = 0;
            hours++;
            if (hours >= 24) {
                hours = 0;  // Reset to 00:00:00 after 24 hours
            }
        }
    }
}

// --------------------------
// Send String via UART
// --------------------------
void sendString(const char *str) {
    while (*str) {
        while (!(IFG2 & UCA0TXIFG));  // Wait for TX buffer
        UCA0TXBUF = *str++;
    }
}

// --------------------------
// Send Number via UART
// --------------------------
void sendNumber(unsigned int num) {
    char buffer[5];
    int i = 0;

    if (num == 0) {
        sendString("0");
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        while (!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = buffer[--i];
    }
}

