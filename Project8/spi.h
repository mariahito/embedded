#ifndef USCI_SPI_H
#define USCI_SPI_H

#include <msp430.h>

#define LED_DISPLAY_BASE 10
#define LED_DISPLAY_MAX 99
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 Master-Out-Slave-In (MOSI) 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_MOSI_BIT                 BIT7
#define USCIA0_MOSI_PORT                P1OUT
#define USCIA0_MOSI_DDR                 P1DIR
#define SET_USCIA0_MOSI_AS_AN_OUTPUT    USCIA0_MOSI_DDR |= USCIA0_MOSI_BIT
#define SET_MOSI                        USCIA0_MOSI_PORT |= USCIA0_MOSI_BIT
#define RESET_MOSI                      USCIA0_MOSI_PORT &= ~USCIA0_MOSI_BIT


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * LED Display Driver Latch (XLAT)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P2.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define XLAT_BIT                        BIT0
#define XLAT_PORT                       P2OUT
#define XLAT_DRR                        P2DIR
#define SET_XLAT                        XLAT_PORT |= XLAT_BIT
#define RESET_XLAT                      XLAT_PORT &= ~XLAT_BIT
#define SET_XLAT_AS_AOUTPUT             XLAT_DRR |= XLAT_BIT


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * LED Display Driver Blank
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.4
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define BLANK_BIT                      BIT4
#define BLANK_PORT                     P1OUT
#define BLANK_DRR                      P1DIR
#define BLANK_ENABLE                   BLANK_PORT &= ~BLANK_BIT
#define BLANK_DISABLE                  BLANK_PORT |= BLANK_BIT
#define BLANK_AS_OUTPUT                BLANK_DRR |= BLANK_BIT


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Slave Clock for GPIO Flash Memory Board
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.4
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define SPI_SCK_BIT                 BIT5
#define SPI_SCK_PORT                P1OUT // SPI Slave Clock output
#define SPI_SCK_DDR                 P1DIR // SPI Slave Clock direction
#define SET_SPI_SCK_AS_AN_OUTPUT    SPI_SCK_DDR |= SPI_SCK_BIT
#define TOGGLE_SCK_SPI              SPI_SCK_PORT ^= SPI_SCK_BIT
#define TURN_OFF_SCK_SPI            SPI_SCK_PORT &= ~SPI_SCK_BIT
#define SET_SCK                     SPI_SCK_PORT |= SPI_SCK_BIT


/*
 * This function initializes all hardware and port pins to support SPI.
 */
void InitializeSPI();

/*
 * This function sends the byte, SendValue, using SPI.
 */
void SPISendByte(unsigned char SendValue);

/*
 * This function receives a byte using SPI.
 *
 * Return Value: The byte that is received over SPI.
 */
unsigned char SPIReceiveByte();

#endif
