/*
 * MSP430 USB Firmware Upgrade Example
 *
 * An example software to field firmware upgrade a MSP430 USB based device
 *
 * Copyright (C) {2010} Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "BSL_UART_Protocol_5xx.h"
#include "BSL_USB_Protocol_5xx.h"
#include "BSL_UART_Protocol_ROM.h"
#include "BSL_IO_UART.h"
#include "BSL_IO_USB.h"
//#include "BSL_5xx_Command_Definitions.h"
#include "BSL_Commands_5xx.h"
#include "BSL_Commands_ROM.h"
//#include "BSL_Function_pointers.h"
#include "BSL430_lib.h"
#include "TextFileIO.h"
#include "BSL_Definitions.h"

int BSL_Version;
unsigned int BSL_Com;

FILE *logFile;

void (*BSL_set_TX_Packet)( unsigned char (*txFunc)( dataBuffer db ) );
void (*BSL_set_RX_Packet)( unsigned char (*rxFunc)( dataBuffer *db ) );
void createTI_TextArray( DataBlock data , char* dataArray );

unsigned int BSL_bufferSize;
unsigned char versionString[50];

__declspec(dllexport) unsigned char* BSL_TX_BSL_Version_String()
{
    dataBuffer rx_buf;
    if( BSL_TX_BSL_Version_l() == BSL_ACK )
    {
        rx_buf = BSL_get_RX_Buffer_l();
        if((BSL_Version == FAMILY_5438)||(BSL_Version == FAMILY_FLASH))
        {
            strcpy_s( (char*)versionString, sizeof(versionString), "Vendor:[xx],CI:[xx],API:[xx],PI:[xx]");
            if( rx_buf.data[1] == 0x00 )
            {
                versionString[8] = 'T';
                versionString[9] = 'I';
            }
            // CI
            versionString[16] = ((rx_buf.data[2]>>4)&0x0F)+'0';
            versionString[17] = ((rx_buf.data[2])&0x0F)+'0';
            // API
            versionString[25] = ((rx_buf.data[3]>>4)&0x0F)+'0';
            versionString[26] = ((rx_buf.data[3])&0x0F)+'0';
            // PI
            versionString[33] = ((rx_buf.data[4]>>4)&0x0F)+'0';
            versionString[34] = ((rx_buf.data[4])&0x0F)+'0';
        }
        else if (BSL_Version == FAMILY_ROM)
        {
        }
    }
    else
    {
        strcpy_s((char*)versionString, sizeof(versionString), "ERROR" );
    }
    return versionString;
}

__declspec(dllexport) signed int BSL_TX_BufferSize()
{
    if((BSL_Version == FAMILY_5438)||(BSL_Version == FAMILY_FLASH))
    {
        if ( Flash_TX_BufferSize() == BSL_ACK )
        {
            dataBuffer rx_buf;
            signed int retValue;
            rx_buf = BSL_get_RX_Buffer_l();
            retValue = rx_buf.data[1];
            retValue |= rx_buf.data[2]<<8;
            return retValue;
        }
        else
        {
            return -2;
        }
    }
    else
    {
        return -1;
    } // wrong family
}

__declspec(dllexport) unsigned int BSL_TX_TXT_File( char* fileName, unsigned int addr, unsigned int length )
{
    unsigned char buf[(512)];

    // below is the better way to do things, but crashes currently
    //unsigned char *buf = malloc( (size_t)length);

    //curLength = BSL_bufferSize;
    openTI_TextForWrite( fileName );
    /*
    for( curAddr = addr; curAddr < (addr+length); curAddr+=curLength)
    {
    if( (addr+length)-curAddr < BSL_bufferSize )
    {
    curLength = (addr+length)-curAddr;
}
     */
    if ( BSL_TX_DataBlock_l ( addr, length, buf ) == BSL_ACK )
    {
        writeTI_TextFile( addr, buf, length);
    }
    else
    {
        return 0xFF;
    }

    //} // for whole file

    endTI_TextWrite();
    return BSL_ACK;
}

__declspec(dllexport) unsigned int BSL_RX_TXT_File( char* fileName, unsigned char fast)
{
    DataBlock data;
    //int i;
    //printf( "\n");

    openTI_TextForRead( fileName );

    data.eof = 0;   // clear the EOF flag

    while( data.eof == 0 )
    {
        data = readTI_TextFile(BSL_bufferSize);
        /*
        for( i = 0; i < data.numberOfBytes; i++)
        {
        printf( "%2.2x ", data.data[i] );
    }
         */
        if( !fast)
        {
            if ( BSL_RX_DataBlock_l( data ) != BSL_ACK )
            {
                return data.startAddr;
            }
        }
        else
        {
            BSL_RX_DataBlock_Fast_l( data );
        }
    }
    closeTI_Text();
    //printf( "\n");
    return BSL_ACK;
}

__declspec(dllexport) unsigned int BSL_RX_TXT( char * dataArray, unsigned char fast)
{
    DataBlock data;
    char * dataArrayLine;
    char * newDataArray;
    unsigned long currentAddr = NO_DATA_READ;
    unsigned long totalBytesTransmitted = 0;

    newDataArray = dataArray;

    if( currentAddr == NO_DATA_READ )
    {
        sscanf_s(&newDataArray[1], "%x\n", &currentAddr);
        newDataArray = strstr((char* ) dataArray, (char* )"\n");
    }
    data.startAddr = currentAddr;

    char* rest[0x100];

    // Establish string to get first token line
    dataArrayLine = strtok_s( &newDataArray[0], (char* )"\n", rest);

    while( currentAddr != EOF )
    {
        unsigned int bytesRead = 0;
        int status = 1;
        data.startAddr = currentAddr;

        do
        {
            int stringLength=0;
            int stringPosition=0;
            stringLength = strlen( dataArrayLine );

            if( status == 0 )
            {
                currentAddr = EOF;
            }
            else if( dataArrayLine[0] == '@' )
            {
                sscanf_s(&newDataArray[1], "%x\n", &currentAddr);
                status = 0;
            }
            else if ( dataArrayLine[0] == 'q' || dataArrayLine[0] == 'Q' )
            {
                status = 0;
                currentAddr = EOF;
            }
            else
            {
                for( stringPosition = 0; stringPosition < (stringLength-2); stringPosition+=3 )
                {
                    sscanf_s( &dataArrayLine[stringPosition], "%hhx", &data.data[bytesRead]);
                    bytesRead++;
                    currentAddr++;
                }
            }

            // Establish string to get first token line
            dataArrayLine = strtok_s( NULL, (char* )"\n", rest);
            if(dataArrayLine == NULL)
            {
                currentAddr = EOF;
                status = 0;
            }
        }
        while( (status != 0) && (bytesRead < BSL_bufferSize) );
        data.numberOfBytes = bytesRead;
        totalBytesTransmitted = totalBytesTransmitted + bytesRead;

        // Transmit packets through BSL
        if( !fast)
        {
            if ( BSL_RX_DataBlock_l( data ) != BSL_ACK )
            {
                return data.startAddr;
            }
        }
        else
        {
            BSL_RX_DataBlock_Fast_l( data );
        }
    }

    //printf( "\n");
    return BSL_ACK;
}

__declspec(dllexport) void BSL_setFamily( unsigned int family )
{
    if( family == FAMILY_ROM )
    {
        BSL_RX_Password_l = &ROM_RX_Password;
        BSL_RX_DataBlock_l = &ROM_RX_DataBlock;
        BSL_TX_DataBlock_l = &ROM_TX_DataBlock;
        BSL_eraseSegment_l = &ROM_eraseSegment;
        BSL_massErase_l = &ROM_massErase;
        BSL_eraseCheck_l = &ROM_eraseCheck;
        BSL_setMemOffset_l = &ROM_setMemOffset;

        BSL_set_TX_Packet = &ROM_set_TX_Packet;
        BSL_set_RX_Packet = &ROM_set_RX_Packet;
        BSL_get_RX_Buffer_l = &ROM_get_RX_Buffer;
        BSL_eraseMainOrInfo_l = &ROM_eraseMainOrInfo;
        BSL_Version = FAMILY_ROM;
        BSL_bufferSize = 240;
    }
    else if ((family == FAMILY_FLASH)||(family == FAMILY_5438))
    {
        BSL_set_TX_Packet = &Flash_set_TX_Packet;
        BSL_set_RX_Packet = &Flash_set_RX_Packet;
        BSL_TX_BSL_Version_l = &Flash_TX_BSL_Version;
        BSL_TX_DataBlock_l = &Flash_TX_DataBlock;
        BSL_LoadPC_l = &Flash_LoadPC;
        BSL_CRC_Check_l = &Flash_CRC_Check;
        BSL_massErase_l = &Flash_massErase;
        BSL_toggleInfo_l = &Flash_toggleInfo;
        BSL_eraseSegment_l = &Flash_eraseSegment;
        BSL_RX_Password_l = &Flash_RX_Password;
        BSL_RX_DataBlock_l = &Flash_RX_DataBlock;
        BSL_RX_DataBlock_Fast_l = &Flash_RX_DataBlock_Fast;
        BSL_get_RX_Buffer_l = &Flash_get_RX_Buffer;
        BSL_changeBaudRate_l = &Flash_changeBaudRate;
        BSL_bufferSize = 240;
        BSL_Version = FAMILY_FLASH;
    }
    if ( family == FAMILY_5438 )
    {
        BSL_RX_Password_l = &Flash_RX_Password_5438;
        //BSL_TX_BufferSize = &Flash_TX_BufferSize;
        BSL_Version = FAMILY_5438;
    }
}

__declspec(dllexport) void BSL_setCom( unsigned int com )
{
    BSL_Com = com;

    if( com == COM_UART )
    {
        BSL_SetVerbose_l = &UART_setVerbose;
        BSL_initialize_BSL_l = &UART_initialize_BSL;

        if (BSL_Version == FAMILY_ROM )
        {
            BSL_set_TX_Packet( &UART_ROM_TXPacket);
            BSL_set_RX_Packet( &UART_ROM_RXPacket);
        }
        else if( (BSL_Version == FAMILY_5438)||(BSL_Version == FAMILY_FLASH) )
        {
            BSL_set_TX_Packet( &UART_5xx_TXPacket);
            BSL_set_RX_Packet( &UART_5xx_RXPacket);
            if( BSL_Version == FAMILY_5438)
            {
                BSL_initialize_BSL_l = &UART_initialize_BSL_5438;
            }
        }
    }
    else if ( com == COM_USB )
    {
        BSL_SetVerbose_l = &USB_setVerbose;
        BSL_initialize_BSL_l = &USB_initialize_BSL;
        BSL_close_BSL_l = &USB_close_BSL;
        BSL_set_TX_Packet( &USB_5xx_TXPacket);
        BSL_set_RX_Packet( &USB_5xx_RXPacket);
        BSL_bufferSize = 48;
    }
}
__declspec(dllexport) BYTE BSL_RegisterUSBDeviceNotification(HWND hWnd)
{
    return USB_RegisterForDeviceNotification(hWnd);
}

__declspec(dllexport) BYTE BSL_UnRegisterUSBDeviceNotification(void)
{
    return USB_UnRegisterDeviceNotification();
}

__declspec(dllexport) DWORD BSL_NumOfUSBDevices(unsigned short VID, unsigned short PID)
{
    return USB_numOfDevices(VID,PID);
}

__declspec(dllexport) BYTE BSL_initialize_BSL( unsigned char* com )
{
    return BSL_initialize_BSL_l(com);
}

__declspec(dllexport) BYTE BSL_close_BSL(void)
{
    return BSL_close_BSL_l();
}

__declspec(dllexport) dataBuffer BSL_get_RX_Buffer(void)
{
    return BSL_get_RX_Buffer_l();
}

FILE* fp = nullptr;
FILE* fp2 = nullptr;

__declspec(dllexport) void BSL_SetVerbose( unsigned int verb )
{
    BSL_SetVerbose_l( verb );
    errno_t err;
    // Writes to a log file if verbose mode is on
    if (verb)
    {
        if((err = freopen_s(&logFile, "c:\\gohak.txt", "w", fp2 )) == NULL)
        {
            printf("Error creating log file.\n");
        }
        printf("Verbose mode on.\n");
    }
    else
    {
        // Stop logging file.
        fclose(logFile);
    }
}

__declspec(dllexport) unsigned char BSL_eraseCheck( unsigned long int addr, unsigned long int length )
{
    return BSL_eraseCheck_l( addr, length );
}

__declspec(dllexport) unsigned char BSL_LoadPC( unsigned long int addr )
{
    return BSL_LoadPC_l( addr );
}

__declspec(dllexport) unsigned char BSL_setMemOffset( unsigned long int addr )
{
    return BSL_setMemOffset_l( addr );
}

__declspec(dllexport) unsigned char BSL_CRC_Check( unsigned long int addr, unsigned long int length )
{
    return BSL_CRC_Check_l( addr, length );
}

__declspec(dllexport) unsigned char BSL_massErase(void)
{
    return BSL_massErase_l();
}

__declspec(dllexport) unsigned char BSL_toggleInfo(void)
{
    return BSL_toggleInfo_l();
}

__declspec(dllexport) unsigned char BSL_eraseSegment( unsigned long int addr )
{
    return BSL_eraseSegment_l( addr );
}

__declspec(dllexport) unsigned char BSL_eraseMainOrInfo( unsigned long int addr )
{
    return BSL_eraseMainOrInfo_l( addr );
}
__declspec(dllexport) unsigned char BSL_RX_Password( DataBlock data )
{
    return BSL_RX_Password_l( data );
}

__declspec(dllexport) unsigned char BSL_changeBaudRate( unsigned int rate )
{
    return BSL_changeBaudRate_l( rate );
}

__declspec(dllexport) DataBlock BSL_readTI_TextFile(int bytesToRead)
{
    return readTI_TextFile(bytesToRead);
}

__declspec(dllexport) int BSL_moreDataToRead_TextFile(void)
{
    return moreDataToRead();
}

__declspec(dllexport) DataBlock BSL_readTI_Text(char * dataArray , int bytesToRead)
{
    return readTI_Text(dataArray, bytesToRead);
}

__declspec(dllexport) int BSL_openTI_TextForRead( char *filename )
{
    return openTI_TextForRead(filename);
}

__declspec(dllexport) void BSL_closeTI_Text( void )
{
    closeTI_Text();
}

__declspec(dllexport) DataBlock BSL_getDefaultPass( void )
{
    DataBlock default_pass = {
        {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
        32,
        0xFFE0,
        0,          // eof flag
    };
    return default_pass;
}

DataBlock readTI_Text(char * dataArray, unsigned int bytesToRead)
{
    DataBlock returnDataBlock;
    char * dataArrayLine;
    char * newDataArray;
    char* nextDataArray;    // not used, but required by strtok_s()
    unsigned long currentAddr = NO_DATA_READ;

    newDataArray = dataArray;

    if( currentAddr == NO_DATA_READ )
    {
        sscanf_s( &newDataArray[1], "%x\n", &currentAddr );
        newDataArray = strstr((char* ) dataArray, (char* )"\n");
    }
    returnDataBlock.startAddr = currentAddr;

    // Establish string to get first token line
    dataArrayLine = strtok_s( newDataArray, (char*)"\n", &nextDataArray );
    // dataArrayLine = strtok( &newDataArray[0], (char* )"\n" );   // deprecated

    unsigned int bytesRead = 0;
    int status = 1;
    returnDataBlock.startAddr = currentAddr;

    do
    {
        int stringLength=0;
        int stringPosition=0;
        stringLength = strlen( dataArrayLine );

        if( status == 0 )
        {
            currentAddr = EOF;
        }
        else if( dataArrayLine[0] == '@' )
        {
            sscanf_s(&dataArrayLine[1], "%x\n", &currentAddr);
            status = 0;
        }
        else if ( dataArrayLine[0] == 'q' || dataArrayLine[0] == 'Q' )
        {
            status = 0;
            currentAddr = EOF;
        }
        else
        {
            for( stringPosition = 0; stringPosition < (stringLength-2); stringPosition+=3 )
            {
                sscanf_s( &dataArrayLine[stringPosition], "%hhx", &returnDataBlock.data[bytesRead] );
                bytesRead++;
                currentAddr = currentAddr + 1;
            }
        }

        dataArrayLine = strtok_s( NULL , (char*)"\n", &nextDataArray);
        if(dataArrayLine == NULL)
        {
            currentAddr = EOF;
            status = 0;
        }
    }
    while( (status != 0) && (bytesRead < bytesToRead) );
    returnDataBlock.numberOfBytes = bytesRead;

    return returnDataBlock;
}

#ifdef __cplusplus
}
#endif
