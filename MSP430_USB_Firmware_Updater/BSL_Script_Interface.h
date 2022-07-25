#pragma once

//#include "BSL_Commands_ROM.h"
//#include "BSL_Commands_5xx.h"

//#include "CRC.h"
extern "C"
{
  #include "BSL_Interface.h"
  #include "TextFileIO.h"
}

#define BSL_SUCCESS 0x01;
#define BSL_FAILED  0x00;

#define MODE_ROM 1
#define MODE_5438 2
#define MODE_FLASH 3
#define MODE_USB 4

#define DATA_ACK_ROM (char)0x90 
#define DATA_NAK_ROM (char)0xA0
#define DATA_ACK_FLASH (char) 0x00

class BSL_Script_Interface
{
private:
    int currentAddr;
    int family;
    int bufferSize;
    int verbose_mode;

public:
    BSL_Script_Interface(char interfaceMode);
    ~BSL_Script_Interface(void);

    char mode;
    char *token;
    //unsigned char (*BSL_RX_DataBlock_Fast)(DataBlock data);

    BYTE initComm(void);
//    BYTE closeComm(void);
//    BYTE numOfDevices(void);

    bool RX_Password(void);
//    bool RX_Password(CString* RX_Pass);

    //void delay(int time);
    //bool massErase(int cycles);
    //bool massErase(void);
    //void eraseCheck(void);
    //void eraseSegment(void);
    //bool RX_DataBlock(CString* RX_Data);
    //bool RX_DataBlock(CString rxFilename);
//    DataBlock TX_DataBlock(int addr, int length);
    //void TX_BSL_Version(void);
    //bool setPC(int newAddress);
   // bool TX_BufferSize(void);
    //bool changeBaudRate(int rate);
    //bool eraseMainOrInfo(int addr);
//    void CRC(UINT addr, UINT length, UINT value);
    //void invoke_BSL(void);
    //void setBufferSize(int newBufferSize);
//    BYTE resetDevice(void);
    //bool verifyMemory(CString* RX_Data);
    //bool verifyMemory(CString rxFilename);

protected:
    char line[100];
    bool protectedMassErase(int cycles);
    bool protected_RX_Password(DataBlock pass);

//    DataBlock readTI_TextHex(int bytesToRead, CString* RX_Data); //TODO: buffer size hardcoded. change later

    BOOL Start(void);



};
