#include "debounce.h"

#define FALSE 0
#define TRUE 1

extern unsigned int g1msTimer;

void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit,
		unsigned char HoldTime,unsigned char ReleaseTime)
{
	Switch->CurrentState = DbExpectHigh;
	Switch->SwitchPort = SwitchPort;
	Switch->SwitchPortBit = SwitchBit;

	Switch->HoldTime = HoldTime; // units equal milliseconds
	Switch->ReleaseTime = ReleaseTime; // units equal milliseconds
	Switch->EventTime = 0;
}

SwitchStatus GetSwitch(SwitchDefine *Switch)
{
    unsigned char SwitchValue = 0;
    SwitchValue = *(Switch -> SwitchPort) & Switch -> SwitchPortBit;
    if(SwitchValue )
        return Low;
    else
        return High;
}

SwitchStatus Debouncer(SwitchDefine *Switch)
{
	SwitchStatus CurrentSwitchReading;
	SwitchStatus DebouncedSwitchStatus = Low;
	unsigned char X1;
	unsigned char X0;
	DbState NextState;
	CurrentSwitchReading = GetSwitch(Switch);
	unsigned int ElapsedTime = g1msTimer - Switch -> EventTime;

	// First, determine the current inputs, X1 and X0.
	if(Switch -> CurrentState == DbValidateHigh){
	    if(ElapsedTime > Switch -> HoldTime )
	        X1 = TRUE;
	    else
	        X1 = FALSE;
	}
	else if(Switch -> CurrentState == DbValidateLow){
	    if(ElapsedTime > Switch -> ReleaseTime)
	        X1 = TRUE;
	    else
	        X1 = FALSE;
	}
	if(CurrentSwitchReading == High)
	    X0 = TRUE;
	else
	    X0 = FALSE;

	// Next, based on the input values and the current state, determine the next state.
	switch (Switch-> CurrentState) {
		case DbExpectHigh:
            if(X0 == TRUE)
                NextState = DbValidateHigh;
            else
                NextState = DbExpectHigh;
		break;
		case DbValidateHigh:
		    if(X0 == FALSE)
		        NextState = DbExpectHigh;
		    else if(X0 == TRUE && X1 == TRUE)
		        NextState = DbExpectLow;
		    else if(X1 == FALSE && X0 == TRUE)
		        NextState = DbValidateHigh;
		break;
		case DbExpectLow:
		    if(X0 == FALSE)
                NextState = DbValidateLow;
            else
                NextState = DbExpectLow;
		break;
		case DbValidateLow:
		    if(X0 == TRUE)
		        NextState = DbExpectLow;
		    else if (X1 == FALSE && X0 ==FALSE)
		        NextState = DbValidateLow;
		    else if(X1 == TRUE && X0 == FALSE)
		        NextState = DbValidateHigh;
		break;
		default: NextState = DbExpectHigh;
	}
	
	// Perform the output function based on the inputs and current state.
	switch (Switch-> CurrentState) {
		case DbExpectHigh:
		    if(X0 == TRUE)
		        Switch -> EventTime = g1msTimer;
		    DebouncedSwitchStatus = Low;
			SET_DEBUG1_PIN_LOW; SET_DEBUG0_PIN_LOW;
		break;
		case DbValidateHigh:
		    if(X0 == TRUE && X1 == TRUE)
		        DebouncedSwitchStatus = High;
		    else
		        DebouncedSwitchStatus = Low;
			SET_DEBUG1_PIN_LOW; SET_DEBUG0_PIN_HIGH;
		break;
		case DbExpectLow:
		    if(X0 == FALSE)
		        Switch -> EventTime = g1msTimer;
		    DebouncedSwitchStatus = High;
			SET_DEBUG1_PIN_HIGH; SET_DEBUG0_PIN_LOW;
		break;
		case DbValidateLow:
		    if(X0 == FALSE && X1 == TRUE)
		        DebouncedSwitchStatus = Low;
		    else
		        DebouncedSwitchStatus = High;
			SET_DEBUG1_PIN_HIGH; SET_DEBUG0_PIN_HIGH;
		break;
	}
	
	// Finally, update the current state.
	Switch->CurrentState = NextState;

	return DebouncedSwitchStatus;
}

