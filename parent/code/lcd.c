#include <msp430.h>
#include <stdio.h>

volatile unsigned long millis_count = 0;

// Initialize Internal Clock to 1MHz
void initClock() {
    BCSCTL1 = CALBC1_1MHZ;  // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;
}

// Initialize UART for 9600 Baud
void initUART() {
    P1SEL |= BIT1 | BIT2;  // Set P1.1 = RXD, P1.2 = TXD
    P1SEL2 |= BIT1 | BIT2;

    UCA0CTL1 |= UCSSEL_2;  // Select SMCLK (1MHz)
    UCA0BR0 = 104;         // 1MHz / 9600 = 104
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;     // Modulation
    UCA0CTL1 &= ~UCSWRST;  // Start UART
}

// TimerA Interrupt to Increment Milliseconds
void initTimerA() {
    TA0CCR0 = 1000 - 1;  // 1ms tick at 1MHz
    TA0CCTL0 = CCIE;     // Enable TimerA interrupt
    TA0CTL = TASSEL_2 | MC_1;  // SMCLK, Up mode
    __enable_interrupt();
}

// Send Character over UART
void UART_sendChar(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait until buffer is ready
    UCA0TXBUF = c;
}

// Send String over UART
void UART_sendString(char *str) {
    while (*str) {
        UART_sendChar(*str++);
    }
}

// Print Time via UART
void printTime() {
    char buffer[20];
    unsigned long seconds = millis_count / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;

    sprintf(buffer, "Time: %02lu:%02lu:%02lu\n", hours, minutes, seconds);
    UART_sendString(buffer);
}

int main() {
    WDTCTL = WDTPW | WDTHOLD;  // Stop Watchdog Timer
    initClock();   // Set Clock to 1MHz
    initUART();    // Initialize UART
    initTimerA();  // Start Timer for millis

    UART_sendString("MSP430 UART Initialized\n");

    while (1) {
        printTime();
        __delay_cycles(1000000);  // Wait 1 second
    }
}

// TimerA Interrupt Service Routine (ISR)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(void) {
    millis_count++;
}

