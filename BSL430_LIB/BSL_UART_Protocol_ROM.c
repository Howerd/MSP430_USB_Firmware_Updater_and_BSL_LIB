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
#include "BSL_UART_Protocol_ROM.h"
#include "BSL_Command_Definitions_ROM.h"
#include "BSL_IO_UART.h"
#include "CRC.h"

unsigned char UART_ROM_RXPacket( dataBuffer *db )
{
  unsigned char scrap_var;
  unsigned int i;
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  // 0x80 is read by TXPacket below
  scrap_var = readByte();             // first byte is always dummy
  scrap_var = readByte();             // second is size, disposable
  db->size = readByte();              // next size byte
  for( i = 0; i < db->size; i++ )
  {
    db->data[i] = readByte();
  }
  scrap_var = readByte();             // CKL to do: check checksum
  scrap_var = readByte();             // CKH
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  return ROM_ACK;
}


unsigned char UART_ROM_TXPacket( dataBuffer db )
{  
  unsigned char answer;
  unsigned int i;
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  ROM_crc_init();
  writeByte( (unsigned char)0x80 );
  answer = readByte();
  if( answer != ROM_ACK )
  {
    return answer;
  } 
  writeByte( (unsigned char)0x80 );
  ROM_crcByte( 0x80 );
  for( i = 0; i < db.size; i++ )
  {
	ROM_crcByte( db.data[i] );
    writeByte( db.data[i] );
  } // for
  writeByte( ROM_getLowByte() );
  writeByte( ROM_getHighByte() );
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  return readByte();
}