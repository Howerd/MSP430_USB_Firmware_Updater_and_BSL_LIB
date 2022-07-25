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
#ifdef __cplusplus
extern "C" {
#endif

#include "BSL_Definitions.h"

#define FAMILY_5438  1
#define FAMILY_ROM   2
#define FAMILY_FLASH 3

#define COM_UART 1
#define COM_USB  2

// BSL operation was successful
#define BSL_SUCCESS           0x00
// BSL Acknowledged
#define BSL_ACK               0x00
// BSL failed
#define BSL_FAILED            0x01
// BSL could not find any devices
#define BSL_DEVICE_NOT_FOUND  0x01
// BSL failed to register USB HID with Windows Handle
#define BSL_USB_HANDLE_ERROR  0x06

// CRC Error Codes
// CRC passes
#define CRC_SUCCESS           0x00
// CRC failed
#define CRC_FAILED            -1

/* #defines for TI I/O File Operation */
// Successful in opening file
#define OPERATION_SUCCESSFUL  0
// File is at EOF
#define TXT_EOF               -1
// Error found during opening
#define ERROR_OPENING_FILE    -2
// No data being read yet
#define NO_DATA_READ          -3

dataBuffer (*BSL_get_RX_Buffer_l)();
void (*BSL_SetVerbose_l)( unsigned int verb );
unsigned char (*BSL_initialize_BSL_l)( unsigned char* com);
unsigned char (*BSL_close_BSL_l)( void );
//unsigned char (*BSL_TX_BufferSize)();
unsigned char (*BSL_TX_BSL_Version_l)();
unsigned char (*BSL_TX_DataBlock_l)( unsigned long int addr, unsigned long int length, unsigned char *buffer );
unsigned char (*BSL_eraseCheck_l)( unsigned long int addr, unsigned long int length );
unsigned char (*BSL_LoadPC_l)( unsigned long int addr );
unsigned char (*BSL_setMemOffset_l)( unsigned long int addr );
unsigned char (*BSL_CRC_Check_l)( unsigned long int addr, unsigned long int length );
unsigned char (*BSL_massErase_l)();
unsigned char (*BSL_toggleInfo_l)();
unsigned char (*BSL_eraseSegment_l)( unsigned long int addr );
unsigned char (*BSL_eraseMainOrInfo_l)( unsigned long int addr );
unsigned char (*BSL_RX_Password_l)( DataBlock data );
unsigned char (*BSL_RX_DataBlock_l)(DataBlock data);
unsigned char (*BSL_RX_DataBlock_Fast_l)(DataBlock data);
unsigned char (*BSL_changeBaudRate_l)( unsigned int rate );

DataBlock readTI_Text(char * dataArray, unsigned int bytesToRead);

#ifdef __cplusplus
}
#endif
