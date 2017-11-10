#include <msp430.h> 

#include "pushbutton.h"
#include "debounce.h"
#include "timerA.h"
#include "rotary_encoder.h"
#include "spi.h"
#include "LED.h"

// Function Prototypes
void ConfigureClockModule();
void ManageSoftwareTimers(void);

// Global variables
unsigned int g1msTimeout = 0;
unsigned int g1msTimer = 0;

void main(void)
{
	SwitchDefine gRotaryEncoderReset; // Used to reset the timer and LED display.
	SwitchDefine gRotaryEncoderA;
	SwitchDefine gRotaryEncoderB;

	EncoderDefinitions myRotaryEncoder;
	EncoderState myEncoderState = Initialize; // Current state of the rotary encoder
	EncoderState current_state = Initialize;
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
    ConfigureClockModule();

    // Initialize hardware.
	InitializePushButtonPortPin();
	ConfigureTimerA();
	InitializeRotaryEncoderPortPins();
	InitializeSPI();
	InitializeLEDPortPins();

	// Initialize the input "switches" to the rotary encoder.
	InitializeSwitch(&gRotaryEncoderReset,(char *) &PUSHBUTTON_PORT_IN,(unsigned char) PUSHBUTTON_BIT,
			HIGH_THRESHOLD,LOW_THRESHOLD);
	InitializeSwitch(&gRotaryEncoderA,(char *) &ROTARY_ENCODER_A_PORT,(unsigned char) ROTARY_ENCODER_A_BIT,
			ROTARY_ENCODER_BOUNCE_TIME_MAX,ROTARY_ENCODER_BOUNCE_TIME_MAX);
	InitializeSwitch(&gRotaryEncoderB,(char *) &ROTARY_ENCODER_B_PORT,(unsigned char) ROTARY_ENCODER_B_BIT,
			ROTARY_ENCODER_BOUNCE_TIME_MAX,ROTARY_ENCODER_BOUNCE_TIME_MAX);

	_enable_interrupts();
	
	while (1) {

		// First, update software timers.
		ManageSoftwareTimers();

		// Then, determine the debounced inputs to rotary encoder state machine.
		myRotaryEncoder.Reset = Debouncer(&gRotaryEncoderReset);
		myRotaryEncoder.SwitchA = Debouncer(&gRotaryEncoderA);
		myRotaryEncoder.SwitchB = Debouncer(&gRotaryEncoderB);

		// Next, use the debounced inputs and the current state to generate the next state.
		myEncoderState = RotaryEncoder(&myRotaryEncoder,&myEncoderState);

		// Now, perform the appropriate output task(s).
		if (myEncoderState == Initialize) {
		    SPISendByte(0x3f);  //send zero
		    SPISendByte(0x3f);  //send zero
		    SET_XLAT;
		    RESET_XLAT;
		    BLANK_ENABLE;
		}
		else {
		    if(current_state == myEncoderState && g1msTimer >= 1000){
		        myRotaryEncoder.RotaryEncoderStateCount -= 4;
		        g1msTimer = 0;
		        TOGGLE_LED1; //toggle red LED every second for verifictation
		    }
		    if(myRotaryEncoder.RotaryEncoderStateCount > 396){ //if encoder reaches 99 start decreasing
		        myRotaryEncoder.RotaryEncoderStateCount =396;
		    }
		    if(myRotaryEncoder.RotaryEncoderStateCount < 0){  //if encoder reaches zero stay there
		        myRotaryEncoder.RotaryEncoderStateCount = 0;
		    }
		    SetLEDDisplay(myRotaryEncoder.RotaryEncoderStateCount/4);
		}
		current_state = myEncoderState;
	}
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}

void ManageSoftwareTimers(void)
{
	if (g1msTimeout) {
		g1msTimeout--;
		g1msTimer++;
	}
}

