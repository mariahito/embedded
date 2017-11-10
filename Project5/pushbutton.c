#include "pushbutton.h"
#include "LED.h"

void InitializePushButtonPortPin (void)
{

    P1DIR  &=~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
    P1IE |= BIT3;
    P1DIR &= ~BIT6;
}

#pragma vector = PORT1_VECTOR
// Button interrupt service routine
__interrupt void Button_routine(void)
{
    TA0CCTL0 ^= CCIE;
    P1IFG &= ~BIT3;
}
