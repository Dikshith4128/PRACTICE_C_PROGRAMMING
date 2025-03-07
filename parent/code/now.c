#include <msp430.h>

#define SS_PIN BIT0  // P2.0 as Slave Select (CS)
#define SPI_CS_LOW() (P2OUT &= ~SS_PIN)  // Activate Slave
#define SPI_CS_HIGH() (P2OUT |= SS_PIN)  // Deactivate Slave

void initClock();
void initSPI();
void initUART();
void UART_sendChar(char c);
void UART_sendString(char *str);
void SPI_send(char data);
char SPI_transfer(char data);
void delay_ms(unsigned int ms);

int main() {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    initClock();
    initUART();
    initSPI();

    UART_sendString("MSP430 SPI Initialized\n");

    char message[] = "Hello, SPI!";
    
    while (1) {
        SPI_CS_LOW();  // Activate SPI Slave

        for (int i = 0; message[i] != '\0'; i++) {
            char receivedData = SPI_transfer(message[i]);
            
            // Print received SPI data
            UART_sendString("Received: ");
            UART_sendChar(receivedData);
            UART_sendChar('\n');
        }

        SPI_CS_HIGH();  // Deactivate SPI Slave
        delay_ms(1000);
    }
}

// Initialize clock to 1MHz (Default DCO)
void initClock() {
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}

// Initialize SPI in Master Mode
void initSPI() {
    P1SEL |= BIT5 | BIT6 | BIT7;  // P1.5 = SCLK, P1.6 = MOSI, P1.7 = MISO
    P1SEL2 |= BIT5 | BIT6 | BIT7;
    
    UCB0CTL1 |= UCSWRST;      // Put USCI in reset mode
    UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;  // SPI Mode 0, MSB first, Master
    UCB0CTL1 = UCSSEL_2;      // Use SMCLK
    UCB0BR0 = 8;              // SPI Clock = SMCLK / 8
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;     // Enable SPI
    
    // Configure SS (Slave Select)
    P2DIR |= SS_PIN;  // Set SS as output
    SPI_CS_HIGH();     // Set SS high (inactive)
}

// Initialize UART (9600 baud, TX on P1.2, RX on P1.1)
void initUART() {
    P1SEL |= BIT1 | BIT2;
    P1SEL2 |= BIT1 | BIT2;

    UCA0CTL1 |= UCSSEL_2;  // Use SMCLK
    UCA0BR0 = 104;         // 1MHz / 9600 = 104
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;  // Enable UART
}

// Send a single character via UART
void UART_sendChar(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait for TX buffer to be ready
    UCA0TXBUF = c;
}

// Send a string via UART
void UART_sendString(char *str) {
    while (*str) {
        UART_sendChar(*str++);
    }
}

// Send data via SPI (Master mode)
void SPI_send(char data) {
    while (!(IFG2 & UCB0TXIFG));  // Wait for TX buffer to be ready
    UCB0TXBUF = data;
}

// Send & Receive data via SPI (Full-Duplex Transfer)
char SPI_transfer(char data) {
    while (!(IFG2 & UCB0TXIFG));  // Wait for TX buffer to be ready
    UCB0TXBUF = data;
    
    while (!(IFG2 & UCB0RXIFG));  // Wait for RX buffer
    return UCB0RXBUF;  // Read received data
}

// Software Delay
void delay_ms(unsigned int ms) {
    while (ms--) __delay_cycles(1000);
}

