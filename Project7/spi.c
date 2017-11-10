#include "spi.h"

void InitializeSPI()
{
    SET_USCIA0_MISO_AS_AN_INPUT;
   SET_USCIA0_MOSI_AS_AN_OUTPUT;
   SET_SPI_SCK_AS_AN_OUTPUT;

   TURN_OFF_SCK_SPI;
}

void SPISendByte(unsigned char SendValue)
{
    unsigned char local = SendValue;
    int i;
    for(i=0; i < 8; i++){
        if(BIT7 & local){
            SET_MOSI;
        }
        else{
            RESET_MOSI;
        }
        local = local << 1;
        //  SCLK:   0->1->0
        TOGGLE_SCK_SPI;
        TOGGLE_SCK_SPI;
    }
}

unsigned char SPIReceiveByte()
{
    unsigned char ReceiveValue = 0;
    unsigned char MISO_reading = 0;
    int i;
    for(i=0; i < 8; i++){
        ReceiveValue <<= 1;
        MISO_reading = USCIA0_MISO_PORT & BIT1;
        MISO_reading >>= 1;
        ReceiveValue |= MISO_reading;
        //  SCLK:   0->1->0
        TOGGLE_SCK_SPI;
        TOGGLE_SCK_SPI;
    }
    return ReceiveValue;
}

