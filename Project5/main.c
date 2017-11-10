#include <msp430.h>
#include "LED.h"
#include "pushbutton.h"
#include "timerA.h"

// Global variable
char LEDBit = LED1;

// Function prototypes
void ConfigureClockModule(void);

void main(void)
{
    // Stop the watchdog timer, and configure the clock module.
    WDTCTL = WDTPW + WDTHOLD;
    ConfigureClockModule();

    // Initialize port pins associated with the LEDs, and then turn off LEDs.
    InitializeLEDPortPins();

    // Initialize the port pin associated with the pushbutton
    InitializePushButtonPortPin();

    // Configure timer A to generate the required interrupt.
    ConfigureTimerA();

    _enable_interrupts();

    // Infinite loop
    while (1) {

    }
}
void TimeDelay(unsigned int delay, unsigned int resolution){
    unsigned i;
    TA0CTL = (MC_0 |TACLR);
    TA0CCR0 = resolution;
    TA0CTL = (TASSEL_2|ID_3|MC_1);

    for(i=0; i<delay; i++){
        while(!(TA0CTL & TAIFG));
        TA0CTL &= !TAIFG;

        TA0CTL =(MC_0 |TACLR);
    }
}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations.
    DCOCTL  = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
}

