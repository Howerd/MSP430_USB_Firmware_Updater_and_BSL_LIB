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
/*==========================================================================*\
|                                                                            |
|                                                                            |
| PC-side Bootstrap Loader communication Application                         |
|                                                                            |
| See main.c for full version and legal information                          |
|                                                                            |
\*==========================================================================*/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <conio.h>
#include "hiddevice.h"
#include "BSL_IO_USB.h"

int          USB_verbose = 0; /* verbose mode flag, default off */
struct strHidDevice device; /*HID device to use */
HDEVNOTIFY deviceNotifyHandle;

BYTE USB_initializeCommPort(unsigned short VID, unsigned short PID , unsigned long INDEX);
/*******************************************************************************
 *Function:    setVerbose
 *Description: sets Verbose mode to on or off for debugging
 *Parameters:
 *             int val   1 == verbose mode on, 0 == verbose mode off
 *Returns:
 *             none
 *******************************************************************************/
void USB_setVerbose( unsigned int verb )
{
    USB_verbose = verb;
}

/*******************************************************************************
 *Function:    USB_initialize_BSL
 *Description: Initializes USB BSL ports.
 *Parameters:
 *             unsigned char* comPort    comPort == "" - Uses TI default VID &
 *                                                       PID
 *                                       comPORT == "VID=0xXXXX,PID=0xXXXX" -
 *                                                       Sets USB BSL to specified
 *                                                       custom VID & PID
 *Returns:
 *             BYTE                      0x00 - Success
 *                                       0x01 - HID device not found
 *
 *******************************************************************************/
BYTE USB_initialize_BSL(unsigned char* comPort)
{
    unsigned short _VID;
    unsigned short _PID;
    char str[100];
    char* token1,
    * next_token1;

    DWORD currentDeviceIndex = 0;

    strcpy_s(str,sizeof(str),comPort);
    //strcpy(str,comPort);

    // If empty, use TI default values, else use values that are passed in
    if(strcmp(comPort,"") == 0)
    {
        _VID = 0x2047;
        _PID = 0x0200;
    }
    else
    {
        char temp[100];

        token1 = strtok_s(str , "," , &next_token1);
        while( token1 != NULL )
        {
            strcpy_s(temp, sizeof(temp), token1);
            // Get next token:
            token1 = strtok_s( NULL , "," , &next_token1);

            if(strstr(temp,"VID") != NULL)
            {
                int VID2;                           // Have to be declared as an int or
                // else it'll throw a stack error
                sscanf_s( temp, "VID=%x", &VID2);
                _VID = VID2;
            }
            else if(strstr(temp,"PID") != NULL)
            {
                int PID2;                           // Have to be declared as an int or
                // else it'll throw a stack error
                sscanf_s( temp, "PID=%x", &PID2);
                _PID = PID2;
            }
            else if(strstr(temp,"DEVICEINDEX") != NULL)
            {
                int DEVICEINDEX2;                           // Have to be declared as an int or
                // else it'll throw a stack error
                sscanf_s( temp, "DEVICEINDEX=%x", &DEVICEINDEX2);
                currentDeviceIndex = DEVICEINDEX2;
            }
        }
    }

    return USB_initializeCommPort(_VID,_PID,currentDeviceIndex);
}

/*******************************************************************************
 *Function:    USB_close_BSL
 *Description: Closes USB port for communication
 *Parameters:
 *             none
 *Returns:
 *             BYTE              0x00 - Success
 *                               0x01 - HID device not found
 *
 *******************************************************************************/
BYTE USB_close_BSL(void)
{
    return HID_Close( &device );
}

/*******************************************************************************
 *Function:    initializeCommPort
 *Description: initializes USB port for communication
 *Parameters:
 *             WORD VID          USB VID
 *             WORD PID          USB PID
 *Returns:
 *             BYTE              0x00 - Success
 *                               0x01 - HID device not found
 *
 *******************************************************************************/
BYTE USB_initializeCommPort(unsigned short VID, unsigned short PID , unsigned long deviceIndex)
{
    HID_Init( &device );
    return HID_Open( &device, VID, PID, deviceIndex );
}

/*******************************************************************************
 *Function:    USB_RegisterForDeviceNotification
 *Description: Registers USB device for Windows notification
 *Parameters:
 *             HWND hWnd         Current GUI Windows handle
 *Returns:
 *             BYTE              0x00 - Success
 *                               0x06 - HID device handle error
 *
 *******************************************************************************/
BYTE USB_RegisterForDeviceNotification(HWND hWnd)
{
    return HID_RegisterForDeviceNotification(hWnd, &deviceNotifyHandle);
}

/*******************************************************************************
 *Function:    USB_RegisterForDeviceNotification
 *Description: Registers USB device for Windows notification
 *Parameters:
 *             HWND hWnd         Current GUI Windows handle
 *Returns:
 *             BYTE              0x00 - Success
 *                               0x06 - HID device handle error
 *
 *******************************************************************************/
BYTE USB_UnRegisterDeviceNotification(void)
{
    return HID_UnRegisterForDeviceNotification(&deviceNotifyHandle);
}

/*******************************************************************************
 *Function:    USB_numOfDevices
 *Description: Returns the number of USB device based on a vid or pid
 *Parameters:
 *             void
 *Returns:
 *             DWORD             # of USB device based on VID or PID
 *
 *******************************************************************************/
DWORD USB_numOfDevices(unsigned short VID , unsigned short PID)
{
    return HID_GetNumOfDevices(VID , PID);
}

void USB_sendData( unsigned char* buf, unsigned int num_bytes )
{
    unsigned int i;
    unsigned char USB_buf[MAX_USB_FRAME_SIZE];
    //printf( "Step 1\n");
    memset( USB_buf, 0xAC, MAX_USB_FRAME_SIZE );

    //printf( "Step 2 %x @ %x\n", num_bytes, buf);
    USB_buf[0]= (char)num_bytes;
    num_bytes++;
    for( i = 1; i < num_bytes; i++)
    {
        //printf( "i = %x @ %x\n",i,&i );
        //printf( "writing %x to %x\n",buf[i-1],&USB_buf[i] );
        USB_buf[i] = buf[i-1];
    }

    //printf( "Step 3\n");
    //HID_WriteFile( &device, buf, size );
    if( USB_verbose )
    {
        int i;
        for( i = 0; i< MAX_USB_FRAME_SIZE; i++ )
        {
            printf( "{%2.2x} ",(USB_buf[i]&0xFF));
        }
    }

    HID_WriteFile( &device, USB_buf, num_bytes+1 );
    //HID_WriteFile( &device, buf, num_bytes );
    return;
}

int USB_receiveData( unsigned char* buf )
{
    int retVal = 0;
    char buf2[MAX_USB_FRAME_SIZE+1];

    BYTE status;
    status = HID_ReadFile( &device, buf2);

    if ( status == HID_DEVICE_SUCCESS)
    {
        int i;
        if( USB_verbose )
        {
            printf( "\n--------------------------------\n");
        }
        retVal = buf2[1];
        for( i = 0; i<= retVal; i++ )
        {
            buf[i] = buf2[i+2];

        }
        if( USB_verbose )
        {
            for( i = 0; i< sizeof buf2; i++ )
            {
                printf( "[%2.2x] ",(buf2[i]&0xFF));
            }
        }
    }
    else
    {
        // Return error status from HID
        retVal = status;
    }

    return retVal;

}

void USB_delay(int time) /* exported! */
{
#ifndef WIN32
    DWORD startTime= GetTickCount();
    while (UART_calcTimeout(startTime) < time);
#else
    Sleep(time);
#endif
}
