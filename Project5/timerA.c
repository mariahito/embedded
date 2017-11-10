#include "LED.h"
#include "timerA.h"

void ConfigureTimerA(void)
{
   TA0CTL  =   (MC0   |   TACLR);
   TA0CTL  |=  (TASSEL_2 |ID_3  |   MC_1);
   //assign value to Time A0; capture/compare register0
   TA0CCR0 =   249;
   TA0CCR1 = 0;
  //enable capture
   TA0CCTL0    |=  CCIE;
   TA0CCTL1    |=  CCIE;
   TA0CTL      |=  TAIE;
}
unsigned on, off;
unsigned int direction = 0;
#pragma vector = TIMER0_A0_VECTOR
// Interrupt service routine for CCIFG0
	__interrupt void Timer0_A0_routine(void)
{
    //light at min intensity
   if(TACCR1 == 0)
      direction = 1;
   //light at max intensity
   else if (TACCR0 == TACCR1)
       direction = 0;
   //light at min increase duty cycle
   if(direction == 0)
       TACCR1 --;
   //light at max decrease duty cycle
   else if(direction == 1)
       TACCR1++;
}

#pragma vector = TIMER0_A1_VECTOR
// Interrupt service routine for CCIFG1 and TAIFG
	__interrupt void Timer0_A1_routine(void)
{
    switch (TAIV){
	case TA0IV_NONE:
		break;
	case TA0IV_TACCR1: // CCIFG1 interrupt
	    TURN_OFF_LED1;
		break;

	case TA0IV_TAIFG:   // TAIFG interrupt
	    if(TA0CCR1 <= 15){
	        break;
	    }
	    TURN_ON_LED1;

		break;
	default: for (;;); // Should not be possible
	}
}
