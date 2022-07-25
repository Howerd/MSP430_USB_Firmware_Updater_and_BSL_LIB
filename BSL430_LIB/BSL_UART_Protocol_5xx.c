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
#include "BSL_UART_Protocol_5xx.h"
#include "BSL_Command_Definitions_5xx.h"
#include "BSL_IO_UART.h"
#include "BSL_Definitions.h"
#include "CRC.h"

unsigned char UART_5xx_RXPacket( dataBuffer *db )
{
  unsigned int i;
  unsigned char scrap_var;
  //RX_size = 0x0000;
  unsigned char RX_checksum_low = 0x00;
  unsigned char RX_checksum_high = 0x00;
  unsigned char RX_header;
  
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  RX_header = readByte();
/*---------------------------------------HEADER-------------------------*/
  if( RX_header == TIMEOUT_ERROR )
  {
#ifdef DEBUG_ERRORS
    printf( "\n---------------Timeout on header----------------\n");
#endif
    ReadGargbageUntilTimeout();
    return (BYTE)TIMEOUT_ERROR;
  }
  else if( RX_header != (BYTE)0x80 )
  {
#ifdef DEBUG_ERRORS
    printf( "\n---------------Header != 0x80-------------------\n");
#endif
    ReadGargbageUntilTimeout();
    return HEADER_INCORRECT;
  }
/*---------------------------------------Size 1-------------------------*/
  scrap_var = readByte();
  if( scrap_var == TIMEOUT_ERROR )
  {    
#ifdef DEBUG_ERRORS
    printf( "\n---------------Timeout on rx size(1)------------\n");
#endif
    ReadGargbageUntilTimeout();
    return (BYTE)TIMEOUT_ERROR;
  }
  db->size = scrap_var;
/*---------------------------------------Size 2-------------------------*/
  scrap_var = readByte();
  if( scrap_var == TIMEOUT_ERROR )
  {   
#ifdef DEBUG_ERRORS
    printf( "\n---------------Timeout on rx size(2)------------\n");
#endif 
    ReadGargbageUntilTimeout();
    return (BYTE)TIMEOUT_ERROR;
  }

  db->size |= (((DWORD)scrap_var)<<8);
  Flash_crcInit( 0xFFFF);
/*---------------------------------------Reading Data-------------------------*/
  for( i = 0; i < db->size; i++ )
  {
    db->data[i] = (BYTE)readByte();
	Flash_crcInput( (BYTE)db->data[i] );
  }
/*---------------------------------------Checksum low-------------------------*/
  RX_checksum_low = readByte();
  if( RX_checksum_low != Flash_getLowByte() )
  {
    printf( "\nFail on low byte Expected:\t%x Got:\t%2.2x\n", (Flash_getLowByte()&0xFF),(RX_checksum_low&0xFF) );
    ReadGargbageUntilTimeout();
    return CHECKSUM_INCORRECT;
  }
/*---------------------------------------Checksum high-------------------------*/
  RX_checksum_high = readByte();
  if( RX_checksum_high != Flash_getHighByte() )
  {
    printf( "\nFail on high byte Expected:\t%x Got:\t%2.2x\n", (Flash_getHighByte()&0xFF),(RX_checksum_high&0xFF) );
    ReadGargbageUntilTimeout();
    return CHECKSUM_INCORRECT;
  }
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  return FLASH_ACK;
}



unsigned char UART_5xx_TXPacket( dataBuffer db )
{
  unsigned int i;
  unsigned char answer;
  Flash_crcInit( 0xFFFF );
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  writeByte( (unsigned char)0x80 );
  writeByte( (unsigned char)(db.size&0xFF) );
  writeByte( (unsigned char)(db.size>>8&0xFF) );
  for( i = 0; i < db.size; i++ )
  {
    writeByte( db.data[i] );
	Flash_crcInput( db.data[i] );
  }
  writeByte( Flash_getLowByte() );
  writeByte( Flash_getHighByte() );
  answer = readByte();
  if( answer != FLASH_ACK )
  {
    clearBuffers();
  }
  if( UART_GetVerbose() == 1 )
  {
    printf("\n-------------------------------------------------\n");
  }
  return answer;
}
