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
#include "BSL_Definitions.h"

dataBuffer Flash_get_RX_Buffer();
void Flash_set_TX_Packet( unsigned char (*txFunc)( dataBuffer  db ) );
void Flash_set_RX_Packet( unsigned char (*rxFunc)( dataBuffer  *db ) );

unsigned char Flash_TX_BufferSize();
unsigned char Flash_TX_BSL_Version();
unsigned char Flash_TX_DataBlock( unsigned long int addr, unsigned long int length, unsigned char *buffer );
unsigned char Flash_LoadPC( unsigned long int addr );
unsigned char Flash_CRC_Check( unsigned long int addr, unsigned long int length );
unsigned char Flash_massErase();
unsigned char Flash_toggleInfo();
unsigned char Flash_eraseSegment( unsigned long int addr );
unsigned char Flash_RX_Password_5438( DataBlock data );
unsigned char Flash_RX_Password( DataBlock data );
unsigned char Flash_RX_DataBlock(DataBlock data);
unsigned char Flash_RX_DataBlock_Fast(DataBlock data);
unsigned char Flash_changeBaudRate( unsigned int rate );

unsigned char Send_Unknown(void);
