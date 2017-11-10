//
// This program presents a simple User Interface (UI) for controlling the LEDs.
//
#include <msp430.h>
#include <stdio.h>

#include "LED.h"
#include "uart.h"

#define BUFFER_SIZE 80
#define TRUE 1
#define FALSE 0
#define UART 0

// Global variable
char CharacterHasNotBeenReceived;
char ReceivedCharacter = (char) NULL;;

// Function prototypes
void ConfigureClockModule(void);
void PrintString(char *, unsigned char);
void PrintCharacter(char, unsigned char);

void main(void)
{
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

    // Initialize LEDs
    InitializeLEDPortPins();

	// Initialize USCI A0 to use UART.
	InitializeUART();

	_enable_interrupts();

    // "Clear" the screen with a carriage return ('\r') and line feed ('\n'),
	// followed by the prompt string.
    
	// Continue with the UI indefinitely.
	while (TRUE) {
	    PrintString("\r\nEnter a command - (1) Turn on LED (2) Turn off LED (3) Blink LED (x) Exit",UART);
		// Wait for a character to be entered.
		CharacterHasNotBeenReceived = TRUE;
		while (CharacterHasNotBeenReceived);

		//Turn on LEDs
        if(ReceivedCharacter == '1'){
            PrintString("\r\nTurn on which LED ? (1) Red (2) Green :",UART);
            CharacterHasNotBeenReceived = TRUE;
            while (CharacterHasNotBeenReceived);

            if(ReceivedCharacter == '1'){
                TURN_ON_LED1;
                PrintString("\r\nRed LED is on now",UART);
            }
            else if(ReceivedCharacter == '2'){
                TURN_ON_LED2;
                PrintString("\r\nGreen LED is on now",UART);
            }
		}

        //Turn off LEDs
        else if(ReceivedCharacter == '2'){
            PrintString("\r\nTurn off which LED ? (1) Red (2) Green :",UART);
            CharacterHasNotBeenReceived = TRUE;
            while (CharacterHasNotBeenReceived);
            if(ReceivedCharacter == '1'){
                TURN_OFF_LED1;
                PrintString("\r\nRed LED is off now",UART);
            }
            else if(ReceivedCharacter == '2'){
                TURN_OFF_LED2;
                PrintString("\r\nGreen LED is off now",UART);
            }
        }

        //Blink LEDs
        else if(ReceivedCharacter == '3'){
		    //toggle the LED states to the complementary state
            TOGGLE_LED1;
            TOGGLE_LED2;
            TimeDelay(450,150);
            TOGGLE_LED1;
            TOGGLE_LED2;
            TimeDelay(450,150);
            TOGGLE_LED1;
            TOGGLE_LED2;
            PrintString("\r\nBlink LEDs",UART);
		}
        else if(ReceivedCharacter == 'x'){
            TURN_OFF_LED1;
            TURN_OFF_LED2;
            break;
        }

		// Echo character	
		//PrintString("\r\n>>>",UART);
		//PrintCharacter(ReceivedCharacter,UART);
	}
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) to use factory calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

void PrintString(char * StringToPrint, unsigned char PrintDeviceType)
{
	unsigned short i;                     // Used for indexing
	char StringBuffer[BUFFER_SIZE];    // Character buffer to hold each string to be printed

	for (i = 0; i < BUFFER_SIZE; i++) StringBuffer[i] = (char) NULL;
	sprintf(StringBuffer, StringToPrint);
	switch (PrintDeviceType) {
	case UART:
		WriteStringUART(StringBuffer);
		break;
	default: break;
	}
}

void PrintCharacter(char CharacterToPrint, unsigned char PrintDeviceType)
{
	switch (PrintDeviceType) {
	case UART:
		WriteCharacterUART(CharacterToPrint);
		break;
	default: break;
	}
}

void TimeDelay(unsigned int delay, unsigned int resolution){
    volatile int i,j;
    // Wait for approximately 1/4 second
    for(i=0; i<delay;i++){
        for(j=0; j<resolution; j++);
    }
}

