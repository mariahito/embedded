#include "spi.h"

const unsigned char LEDDisplayCharacters[16] = {
        0x3f, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x67, // 9
        0x77, // A
        0x7C, // B
        0x39, // C
        0x5E, // D
        0x79, // E
        0x71  // F
};
void InitializeSPI()
{
   SET_USCIA0_MOSI_AS_AN_OUTPUT;
   SET_SPI_SCK_AS_AN_OUTPUT;
   BLANK_AS_OUTPUT;
   SET_XLAT_AS_AOUTPUT;

   TURN_OFF_SCK_SPI;
   BLANK_ENABLE;
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
        TOGGLE_SCK_SPI;
        TOGGLE_SCK_SPI;
    }
}

//display count
void SetLEDDisplay (unsigned int DisplayValue){
    unsigned char left;
    unsigned char right;

    left =0;
    right = DisplayValue;
    while(right > LED_DISPLAY_BASE-1){
        right -= LED_DISPLAY_BASE;
        left++;
    }
    SPISendByte(LEDDisplayCharacters[right]);
    SPISendByte(LEDDisplayCharacters[left]);
    SET_XLAT;
    RESET_XLAT;
    BLANK_ENABLE;
}



