#include "serial_flash.h"

void InitializeSerialFlash()
{
    SET_ENABLE_U3_AS_AN_OUTPUT;
    SET_ENABLE_U2_AS_AN_OUTPUT;
    SET_HOLD_U3_AS_AN_OUTPUT;
    SET_HOLD_U2_AS_AN_OUTPUT;
    SET_WRITE_PROTECT_AS_AN_OUTPUT;
    DISABLE_HOLD_U3;
    DISABLE_HOLD_U2;
    DISABLE_WRITE_PROTECT;
}

unsigned int ReadFlashMemoryID(unsigned char ComponentNumber)
{
    unsigned char id0;
    unsigned char id1;
    unsigned int ReturnValue;
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(READ_ID);
    SPISendByte(0x00);
    SPISendByte(0x00);
    SPISendByte(0x00);

    //recive ID, one byte at the time
    id1 = SPIReceiveByte(); //BI
    id0 = SPIReceiveByte();
    ReturnValue = (unsigned int)(id1 << 8);
    ReturnValue = ReturnValue | (unsigned int)id0;
    //Disable flash memory
    if(ComponentNumber == FLASH_MEMORY_U3){
        DISABLE_FLASH_MEMORY_U3;
    }
    else{
        DISABLE_FLASH_MEMORY_U2;
    }

    return ReturnValue;
}

unsigned char ReadFlashMemoryStatusRegister(unsigned char ComponentNumber)
{
    unsigned char id0;
    /*if(ComponentNumber == FLASH_MEMORY_U3){
           ENABLE_FLASH_MEMORY_U3;
    }
    else{
            ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(WRDI);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;*/

    if(ComponentNumber == FLASH_MEMORY_U3){
           ENABLE_FLASH_MEMORY_U3;
    }
    else{
            ENABLE_FLASH_MEMORY_U2;
    }

    SPISendByte(RDSR); //sending command to read
    id0 = SPIReceiveByte(); // reading value

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    return id0;
}

void WriteFlashMemoryStatusRegister(unsigned char WriteValue,unsigned char ComponentNumber)
{
    DISABLE_WRITE_PROTECT;
    if(ComponentNumber == FLASH_MEMORY_U3){
           ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    SPISendByte(EWSR);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    if(ComponentNumber == FLASH_MEMORY_U3){
       ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    SPISendByte(WRSR);
    SPISendByte(WriteValue);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;
}

void ReadFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber, unsigned char ReadMode)
{
    unsigned char LSBatAddress = (unsigned char)StartAddress;
    unsigned char middle = (unsigned char)(StartAddress >>8);
    unsigned char MSBatAddress = (unsigned char)(StartAddress >>16);

    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(ReadMode);
    SPISendByte(MSBatAddress);
    SPISendByte(middle);
    SPISendByte(LSBatAddress);

    if(ReadMode==HIGH_SPEED_READ){
        SPISendByte(0x00);
    }

    unsigned int i;
    for(i= 0; i < NumberOfDataValues; i++){
        DataValuesArray[i] = SPIReceiveByte();
    }
    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

}

void ByteProgramFlashMemory(unsigned long MemoryAddress, unsigned char WriteValue, unsigned char ComponentNumber)
{
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    DISABLE_WRITE_PROTECT;
    SPISendByte(WREN);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    unsigned char LSBatAddress = (unsigned char)MemoryAddress;
    unsigned char middle = (unsigned char)(MemoryAddress >>8);
    unsigned char MSBatAddress = (unsigned char)(MemoryAddress >>16);
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    SPISendByte(BYTE_PROGRAM);
    SPISendByte(MSBatAddress);
    SPISendByte(middle);
    SPISendByte(LSBatAddress);
    SPISendByte(WriteValue);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    while(FlashMemoryBusy(ComponentNumber));
}

void AAIProgramFlashMemory(unsigned long StartAddress, unsigned char* DataValuesArray,
unsigned int NumberOfDataValues, unsigned char ComponentNumber)
{
    DISABLE_WRITE_PROTECT;
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    SPISendByte(WREN);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    unsigned char LSBatAddress = (unsigned char)StartAddress;
    unsigned char middle = (unsigned char)(StartAddress >>8);
    unsigned char MSBatAddress = (unsigned char)(StartAddress >>16);
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(AAI_PROGRAM);

    SPISendByte(MSBatAddress);
    SPISendByte(middle);
    SPISendByte(LSBatAddress);

    unsigned char Byte_1;
    Byte_1 = DataValuesArray[0];
    SPISendByte(Byte_1);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    while(FlashMemoryBusy(ComponentNumber));
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    unsigned int i;
    for(i=1; i<NumberOfDataValues;i++){
        SPISendByte(AAI_PROGRAM);
        unsigned char  Bytei;
        Bytei =DataValuesArray[i];
        SPISendByte(Bytei);

        DISABLE_FLASH_MEMORY_U3;
        DISABLE_FLASH_MEMORY_U2;

        while(FlashMemoryBusy(ComponentNumber));
        if(ComponentNumber == FLASH_MEMORY_U3){
            ENABLE_FLASH_MEMORY_U3;
        }
        else{
            ENABLE_FLASH_MEMORY_U2;
        }
    }

    SPISendByte(WRDI);


    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    /*if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(RDSR);*/

    while(FlashMemoryBusy(ComponentNumber));



    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

}

void ChipEraseFlashMemory(unsigned char ComponentNumber)
{
    if(ComponentNumber == FLASH_MEMORY_U3){
            ENABLE_FLASH_MEMORY_U3;
        }
        else{
            ENABLE_FLASH_MEMORY_U2;
        }

    DISABLE_WRITE_PROTECT;
    SPISendByte(WREN);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(CHIP_ERASE);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    while(FlashMemoryBusy(ComponentNumber));
}

void SectorBlockEraseFlashMemory(unsigned long StartAddress, unsigned char ComponentNumber, unsigned char EraseMode)
{
    DISABLE_WRITE_PROTECT;

    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }

    SPISendByte(WREN);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    unsigned char LSBatAddress = (unsigned char)StartAddress;
    unsigned char middle = (unsigned char)(StartAddress >>8);
    unsigned char MSBatAddress = (unsigned char)(StartAddress >>16);
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    SPISendByte(EraseMode);

    SPISendByte(MSBatAddress);
    SPISendByte(0);
    SPISendByte(0);

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;
    while(FlashMemoryBusy(ComponentNumber));
}

void SetBlockProtection(unsigned char ProtectionLevel, unsigned char ComponentNumber)
{
    unsigned char currentValue = ReadFlashMemoryStatusRegister(ComponentNumber);

    if(ProtectionLevel == 0){
        currentValue &= ~SR_BP0;
        currentValue &= ~SR_BP1;
    }

    else if(ProtectionLevel == 1){
        currentValue &=  ~SR_BP1;
        currentValue |=  SR_BP0;

    }
    else if(ProtectionLevel == 2){
        currentValue |=  SR_BP1;
        currentValue &=  ~SR_BP0;
    }
    else if(ProtectionLevel == 3){
        currentValue |=  SR_BP0;
        currentValue |=  SR_BP1;
    }
    WriteFlashMemoryStatusRegister(currentValue,ComponentNumber);
}

unsigned char FlashMemoryBusy(unsigned char ComponentNumber)
{
    if(ComponentNumber == FLASH_MEMORY_U3){
        ENABLE_FLASH_MEMORY_U3;
    }
    else{
        ENABLE_FLASH_MEMORY_U2;
    }
    unsigned char busyByte = ReadFlashMemoryStatusRegister(ComponentNumber);
    unsigned char busyBit = busyByte & SR_BUSY;

    DISABLE_FLASH_MEMORY_U3;
    DISABLE_FLASH_MEMORY_U2;

    return busyBit;
}
