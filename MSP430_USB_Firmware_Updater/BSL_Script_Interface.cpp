#include "StdAfx.h"
#include "BSL_Script_Interface.h"
//#include "BSL_IO_USB.h"

// BSL Scripter Constructor
BSL_Script_Interface::BSL_Script_Interface(char interfaceMode)
{
  if(interfaceMode == MODE_USB)
  {
    token = "USB";
    bufferSize = 48;
  }
  else
  {
    token = "COM1";  // set COM Port
    bufferSize = 240;
  }
    mode = interfaceMode;
    currentAddr = NO_DATA_READ;
    family = -1;
    verbose_mode = 0;
}

BSL_Script_Interface::~BSL_Script_Interface(void)
{
}


BOOL BSL_Script_Interface::Start(void)
{
  SYSTEMTIME lt;
  GetLocalTime(&lt);

/*
  	
    else if( memcmp( line, "VERBOSE", 7 )==0 )
	{
	  printf("Verbose mode ");
      verbose_mode ^= 0x01;
	  setVerbose( verbose_mode );
	  if( verbose_mode )
	  {
	    printf("on \n");
	  }
	  else
	  {
	    printf("off \n");
	  }
	}
*/
  return BSL_SUCCESS;
}
/*
void BSL_Script_Interface::CRC(UINT addr, UINT length, UINT value)
{
  UINT outputValue;
  if( value != 0)
  {
    outputValue = 0;
    printf("CRC from %x of %u bytes to %4.4x\t",addr, length,value);
  }
  else
  {
    outputValue = 1;
    printf("CRC from %x of %u bytes\t\t",addr, length);
  }

  switch( mode )
  {
  case MODE_ROM:
    break;
  case MODE_5438:
  case MODE_FLASH:
    if( Flash_CRC_Check( addr, length) == DATA_ACK_FLASH )
    {
	    int crc;
	    crc = dataBuffer[1];
	    crc |= dataBuffer[2]<<8;
      if( outputValue )
	    {
        printf("%4.4x\n", crc );
	    }
	    else if( crc == value )
	    {
        printf("MATCH");
	    }
	    else if( crc != value )
	    {
        printf("NOT A MATCH (%4.4x)", crc);
	    }
	    else
	    {
        printf("SOME THING ELSE");
	    }
    }
    else
    {
      printf("FAIL\n");
    }
    break;
  case MODE_USB:
    if( USB_CRC_Check( addr, length) == DATA_ACK_FLASH )
    {
      int crc;
      crc = USB_dataBuffer[1];
      crc |= USB_dataBuffer[2]<<8;
      if( outputValue )
      {
        printf("%4.4x\n", crc );
      }
      else if( crc == value )
      {
        printf("MATCH\n");
      }
      else if( crc != value )
      {
        printf("NOT A MATCH (%4.4x)\n", crc);
      }
      else
      {
        printf("SOME THING ELSE\n");
      }
    }
    else
    {
      printf("FAIL\n");
    }
    break;
  default:
    break;
  }
}*/

/*******************************************************************************
*
*******************************************************************************/
/*
void BSL_Script_Interface::delay(int time)
{
  switch( mode )
  {
  case MODE_ROM:
    break;
  case MODE_5438:
  case MODE_FLASH:
    break;
  case MODE_USB:
    USB_delay(time);
    break;
  default:
    break;
  }
}*/

/*******************************************************************************
*
*******************************************************************************/
/*bool BSL_Script_Interface::eraseMainOrInfo(int addr)
{
  if( ROM_eraseMainOrInfo(addr) == DATA_ACK_ROM)
  {
    return BSL_SUCCESS;
  }
  else
  {
    return BSL_FAILED;
  }
}*/

/*******************************************************************************
*
*******************************************************************************/
/*
bool BSL_Script_Interface::changeBaudRate(int rate)
{
  char reply;

  if( family == -1 )
  {
    TX_BSL_Version();
  }

  switch( rate )
  {
  case 4800:
	  reply = DATA_ACK_ROM;
	  if( family == 5 )
	  {
	    reply = Flash_changeBaudRate(BAUD430_4800);
	  }
	  changeCommBaudRate(BAUD430_4800);
	  break;
  case 9600:
	  switch( family )
	  {
	  case 1:
	    reply = ROM_changeBaudRate((char) 0x80, (char) 0x85, (char) 0x00);
		break;
	  case 2:
	    reply = ROM_changeBaudRate((char) 0x80, (char) 0x88, (char) 0x00);
		break;
	  case 4:
	    reply = ROM_changeBaudRate((char) 0x00, (char) 0x98, (char) 0x00);
		break;
	  case 5:
		reply = Flash_changeBaudRate(BAUD430_9600);
		break;
	  }
	  changeCommBaudRate(BAUD430_9600);
	break;
  case 19200:
	  switch( family )
	  {
	  case 1:
	    reply = ROM_changeBaudRate((char) 0xE0, (char) 0x86, (char) 0x01 );
		break;
	  case 2:
	    reply = ROM_changeBaudRate((char) 0x00, (char) 0x8B, (char) 0x01);
		break;
	  case 4:
	    reply = ROM_changeBaudRate((char) 0x00, (char) 0xB0, (char) 0x01);
		break;
	  case 5:
		reply = Flash_changeBaudRate(BAUD430_19200);
		break;
	  }
	  changeCommBaudRate(BAUD430_19200);
	break;
  case 38400:
	  switch( family )
	  {
	  case 1:
	    reply = ROM_changeBaudRate((char) 0xE0, (char) 0x87, (char) 0x02 );
		break;
	  case 2:
	    reply = ROM_changeBaudRate((char) 0x80, (char) 0x8C, (char) 0x02);
		break;
	  case 4:
	    reply = ROM_changeBaudRate((char) 0x00, (char) 0xC8, (char) 0x02);
		break;
	  case 5:
		reply = Flash_changeBaudRate(BAUD430_38400);
		break;
	  }
	  changeCommBaudRate(BAUD430_38400);
	break;
  case 57600:
    reply = Flash_changeBaudRate( BAUD430_57600 );
	changeCommBaudRate(BAUD430_57600);
	break;
  case 115200:
    reply = Flash_changeBaudRate( BAUD430_115200 );
	changeCommBaudRate(BAUD430_115200);
	break;
  }
  delay(10);

  if( reply == DATA_ACK_ROM && family <5 )
  {
    return BSL_SUCCESS;
  }
  else if( reply == DATA_ACK_FLASH && family == 5)
  {
    return BSL_SUCCESS;
  }
  else
  {
    return BSL_FAILED;
  }
}*/

/*******************************************************************************
*
*******************************************************************************/
/*
bool BSL_Script_Interface::TX_BufferSize()
{
  bool status = BSL_FAILED;

  switch( mode )
  {
      case MODE_ROM:
          bufferSize = 240;
          status = BSL_SUCCESS;
          break;
      case MODE_5438:
      case MODE_FLASH:
          if( Flash_TX_BufferSize() == DATA_ACK_FLASH )
          {
              bufferSize = dataBuffer[1];
              bufferSize |= ((int)dataBuffer[2])<<8;
              if( bufferSize > 240 )
              {
                  bufferSize = 240;
              }
              status = BSL_SUCCESS;
          }
          break;
    }
  return status;
}*/

/*******************************************************************************
*
*******************************************************************************/
/*
bool BSL_Script_Interface::setPC(int newAddress)
{
  bool status = BSL_FAILED;

  switch( mode )
  {
  case MODE_ROM:
    if( ROM_loadPC( newAddress )==DATA_ACK_ROM)
	{
      printf("DONE\n");
	}
    else
	{
      printf("FAIL\n");
	}
  break;
  case MODE_5438:
  case MODE_FLASH:
    if( Flash_LoadPC( newAddress )==DATA_ACK_FLASH)
	{
      printf("DONE\n");
	}
    else
	{
      printf("FAIL\n");
	}
    break;
  case MODE_USB:
    if( USB_LoadPC( newAddress )== DATA_ACK_FLASH)
	{
      status = BSL_SUCCESS;
	}
    break;
  default:
    break;
  }

  return status;
}*/

/*******************************************************************************
*
*******************************************************************************/
/*
void BSL_Script_Interface::TX_BSL_Version()
{
  printf("BSL Version:\t\t\t\t");
  switch( mode )
	{
	case MODE_ROM:
      if( ROM_TX_BSL_Version() == DATA_ACK_ROM )
	  {
        printf("%2.2x.%2.2x\n", dataBuffer[10],dataBuffer[11] );
	    if( (dataBuffer[0] & 0xF0) != 0xF0 )
		{
          family = ((dataBuffer[0]&0xF0)>>4);
		}
	    else
		{
	      family = dataBuffer[0]&0x0F;
		}
	  }
      else
	  {
		family = 2;
        printf("ERROR (2xx assumed)\n");
	  }
	  break;
	case MODE_5438:
	case MODE_FLASH:
	  family = 5;
      if( Flash_TX_BSL_Version()==DATA_ACK_FLASH )
	  {
		if( dataBuffer[1] == 0 )
		{
          printf("Vendor:[TI],");
		}
		else
		{
          printf("Vendor:[%2.2x],", dataBuffer[1]);
		}
        printf("CI:[%2.2x],API:[%2.2x],PI:[%2.2x]\n", dataBuffer[2],dataBuffer[3],dataBuffer[4] );
	  }
      else
	  {
        printf("ERROR\n");
	  }
	  break;
	}

  
}*/

/*******************************************************************************
*
*******************************************************************************/
/*DataBlock BSL_Script_Interface::TX_DataBlock(int addr, int length)
{
  int curAddr, curLength, write, bufferOffset;
  char filename[50];
  DataBlock data_from_bsl;

  curLength = bufferSize;

  for( curAddr = addr; curAddr < (addr+length); curAddr+=curLength)
  {
	  if( (addr+length)-curAddr < bufferSize )
	  {
        curLength = (addr+length)-curAddr;
	  }
	  printf("Requesting %u bytes starting at %x\t", curLength, curAddr);
    switch( mode )
	  {
	    case MODE_ROM:
          write = (ROM_TX_DataBlock( curAddr, curLength ) == DATA_ACK_ROM);
	      bufferOffset = 0;
	    break;
	    case MODE_5438:
	    case MODE_FLASH:
	      write = (Flash_TX_DataBlock( curAddr, curLength ) == DATA_ACK_FLASH);
	      bufferOffset = 1;
	    break;
        case MODE_USB:
	      write = (USB_TX_DataBlock( curAddr, curLength ) == DATA_ACK_FLASH);
	      bufferOffset = 1;
	    break;
	  }
    if( write)
    {
      switch( mode )
      {
        case MODE_ROM:
        case MODE_5438:
        case MODE_FLASH:
          for(int i = 0; i < curLength; i++ )
          {
            data_from_bsl.data[i] = dataBuffer[i+bufferOffset];
          }
        break;
        case MODE_USB:
          for(int i = 0; i < curLength; i++ )
          {
            data_from_bsl.data[i] = USB_dataBuffer[i+bufferOffset];
          }
        break;
      }
      data_from_bsl.numberOfBytes = curLength;
      data_from_bsl.startAddr = curAddr;
      //writeTI_TextFile( data_from_bsl );
        printf("DONE\n");
    }
    else
    {
      data_from_bsl.numberOfBytes = 0;
      data_from_bsl.startAddr = 0;
      printf("FAIL\n");
    }
  }
  //endTI_TextWrite();
  
  return data_from_bsl;
}*/

/*******************************************************************************
*
*******************************************************************************/
/*void BSL_Script_Interface::eraseSegment()
{
  int addr;
  sscanf( line, "%*s %x", &addr );
  printf("Erasing segment at addr %x\t\t", addr);
  switch( mode )
  {
  case MODE_ROM:
    if( ROM_eraseSegment( addr ) == DATA_ACK_ROM )
	{
      printf("DONE\n");
	}
    else
	{
      printf("FAIL\n");
	}
	break;
  case MODE_5438:
  case MODE_FLASH:
    if( Flash_eraseSegment( addr ) == DATA_ACK_FLASH )
	{
      printf("DONE\n");
	}
    else
	{
      printf("FAIL\n");
	}
	break;
  } // switch


}
*/
/*******************************************************************************
*
*******************************************************************************/
/*void BSL_Script_Interface::eraseCheck()
{
  int addr, length;
  sscanf( line, "%*s %x %x", &addr, &length );
  printf("Checking erase of %u bytes from %x\t", length, addr);
  switch( mode )
  {
  case MODE_ROM:
    if( ROM_eraseCheck( addr, length ) == DATA_ACK_ROM )
	{
      printf("DONE\n");
	}
	else
	{
      printf("FAIL\n");
	}
	break;
  case MODE_5438:
  case MODE_FLASH:*/
/*    
    if( Flash_CRC_Check( addr, length) == DATA_ACK_FLASH )
	{
	  int i;
	  Flash_crcInit( 0xFFFF);
	  for( i = 0; i < length; i++ )
	  {
        Flash_crcInput(0xFF);
	  }
	  if( (Flash_getHighByte() == dataBuffer[2]) && (Flash_getLowByte() == dataBuffer[1] ) )
	  {
        printf("DONE\n");
	  }
	  else
	  {
        printf("FAIL\n");
	  }
	}
	else
	{
        printf("FAIL\n");
	}

    ;
    //break;
  } // switch
}
*/
/*******************************************************************************
*
*******************************************************************************/
/*
bool BSL_Script_Interface::RX_DataBlock(CString* RX_Data)
{
  DataBlock data;

  bool status = BSL_FAILED;
  CString tempRX_Data = *RX_Data;

  int addressMe[200];
  int i = 0;

  currentAddr = NO_DATA_READ;

  while( !(currentAddr == EOF) )
  {
    data = readTI_TextHex(bufferSize, &tempRX_Data); // TODO: PROBLEM with ODD NUMBERS
    addressMe[i++] = data.startAddr;
    
    switch( mode )
	  {
	    case MODE_ROM:
	      if( ROM_RX_DataBlock(data) == DATA_ACK_ROM )
	      {
          status = BSL_SUCCESS;
	      }
	      else
	      {
          status = BSL_FAILED;
	      }
	      break;
	    case MODE_5438:
	    case MODE_FLASH:
	      if( Flash_RX_DataBlock(data) == DATA_ACK_FLASH )
	      {
          status = BSL_SUCCESS;
	      }
	      else
	      {
          status = BSL_FAILED;
	      }
	      break;
      case MODE_USB:
        // Send Data
	      USB_RX_DataBlock(data);
        break;
      default:
        break;
	  }
  }

  return status;
}*/

/*******************************************************************************
*Function:    bool RX_DataBlock(CString filename)
*Description: Transmits data to MSP430 by reading from a txt file
*Parameters: 
*             CString filename    The filename to be opened and transmitted
*Returns:
*             BSL_FAILED or BSL_SUCCESS
*******************************************************************************/
/*
bool BSL_Script_Interface::RX_DataBlock(CString rxFilename)
{
  bool status = BSL_FAILED;

  char filename[300] = {0};
  if(rxFilename.GetLength() < 300)
  {
    
    for(int i = 0 ; i < rxFilename.GetLength() ; i++)
    {
      filename[i] = rxFilename[i];
    }

    DataBlock data;
    if(openTI_TextForRead( filename ) == OPERATION_SUCCESSFUL)
    {
      while( moreDataToRead() )
      {
        data = readTI_TextFile(48);
	    switch( mode )
	    {
          case MODE_USB:
	        USB_RX_DataBlock(data);
            break;
          default:
            break;
	    }
      }
      status = BSL_SUCCESS;
    }
  }
  else
  {
    // filename too large
    status = BSL_FAILED;
  }

  return status;
}*/

/*******************************************************************************
*Function:    TELL LANE to add this into his function!
*Description: reads a certain amount of bytes from a TI TEXT file
*Parameters: 
*             int bytesToRead       The maximum number of bytes to read from the file
*Returns:
*             A Datablock structure with the requested bytes
*******************************************************************************/
/*DataBlock BSL_Script_Interface::readTI_TextHex(int bytesToRead, CString* RX_Data)
{
  DataBlock returnBlock;
  // TODO: LANE's CODE BUG!
  
  memset(&returnBlock.data,0xFF,sizeof(returnBlock.data)); 

  int bytesRead = 0;
  char string[50];
  int status;

  // CString variable handlers
  int firstNewLineChar;
  CString workingRX_Data;

  if( currentAddr == NO_DATA_READ )
  {
    // Get the first occurence of new line character
    firstNewLineChar = RX_Data->FindOneOf(_T("\n"));
    // Get the first line
    workingRX_Data = RX_Data->Left(firstNewLineChar+1);
    // Get next line of string
    *RX_Data = RX_Data->Right(RX_Data->GetLength() - firstNewLineChar - 1);

    for(int i=0; i < firstNewLineChar+1 ; i++)
    {
      string[i] = workingRX_Data[i];
    }

    sscanf(&string[1], "%x\n", &currentAddr);
  }
  returnBlock.startAddr = currentAddr;
  do
  {
	int stringLength=0;
	int stringPosition=0;

    // Get the first occurence of new line character
    firstNewLineChar = RX_Data->FindOneOf(_T("\n"));
    // Get the first line
    workingRX_Data = RX_Data->Left(firstNewLineChar+1);
    // Get next line of string
    *RX_Data = RX_Data->Right(RX_Data->GetLength() - firstNewLineChar - 1);

    for(int i=0; i < firstNewLineChar+1 ; i++)
    {
      string[i] = workingRX_Data[i];
    }

    if(firstNewLineChar != -1)
      status = 1;
    else
      status = 0;

    stringLength = firstNewLineChar+3;
    if( status == 0 )
	{
      currentAddr = EOF;
	}
    else if( string[0] == '@' )
	{
      sscanf(&string[1], "%x\n", &currentAddr);
	  status = 0;
	}
	else if ( string[0] == 'q' || string[0] == 'Q' )
	{
      status = 0;
	  currentAddr = EOF;
	}
    else
	{
	  for( stringPosition = 0; stringPosition < (stringLength-3); stringPosition+=3 )
	  {
        sscanf( &string[stringPosition], "%2x", &returnBlock.data[bytesRead] );
	    bytesRead++;
		currentAddr++;
	  }
	}
  }
  while( (status != 0) && (bytesRead < bytesToRead) );
  returnBlock.numberOfBytes = bytesRead;
  workingRX_Data.ReleaseBuffer(); // Flush working CString
  return returnBlock;
}
*/
/*
DataBlock BSL_Script_Interface::readTI_TextHex(int bytesToRead, CString* RX_Data)
{
  DataBlock returnBlock;
  
  memset(&returnBlock.data,0xFF,sizeof(returnBlock.data)); 

  int bytesRead = 0;
  char string[50];
  int status;

  // CString variable handlers
  int firstNewLineChar;
  CString workingRX_Data;

  if( currentAddr == NO_DATA_READ )
  {
    // Get the first occurence of new line character
    firstNewLineChar = RX_Data->FindOneOf(_T("\n"));
    // Get the first line
    workingRX_Data = RX_Data->Left(firstNewLineChar+1);
    // Get next line of string
    *RX_Data = RX_Data->Right(RX_Data->GetLength() - firstNewLineChar - 1);

    for(int i=0; i < firstNewLineChar+1 ; i++)
    {
      string[i] = workingRX_Data[i];
    }

    sscanf(&string[1], "%x\r\n", &currentAddr);
  }
  returnBlock.startAddr = currentAddr;
  do
  {
	int stringLength=0;
	int stringPosition=0;

    // Get the first occurence of new line character
    firstNewLineChar = RX_Data->FindOneOf(_T("\n"));
    // Get the first line
    workingRX_Data = RX_Data->Left(firstNewLineChar+1);
    // Get next line of string
    *RX_Data = RX_Data->Right(RX_Data->GetLength() - firstNewLineChar - 1);

    for(int i=0; i < firstNewLineChar+1 ; i++)
    {
      string[i] = workingRX_Data[i];
    }

    if(firstNewLineChar != -1)
      status = 1;
    else
      status = 0;

    stringLength = firstNewLineChar - 1;
    if( status == 0 )
	{
      currentAddr = EOF;
	}
    else if( string[0] == '@' )
	{
      sscanf(&string[1], "%x\n", &currentAddr);
	  status = 0;
	}
	else if ( string[0] == 'q' || string[0] == 'Q' )
	{
      status = 0;
	  currentAddr = EOF;
	}
    else
	{
	  for( stringPosition = 0; stringPosition < (stringLength); stringPosition+=3 )
	  {
      sscanf( &string[stringPosition], "%2x", &returnBlock.data[bytesRead] );
	    bytesRead++;
		  currentAddr++;
	  }
	}
  }
  while( (status != 0) && (bytesRead < bytesToRead) );

  if(bytesRead % 2 == 1)
  {
    // odd numbers. Padd it with 1 byte
    returnBlock.data[bytesRead] = 0xFF;
    bytesRead++;
  }
  returnBlock.numberOfBytes = bytesRead;
  workingRX_Data.ReleaseBuffer(); // Flush working CString
  return returnBlock;
}*/


/*******************************************************************************
* Overloaded function
*******************************************************************************/
/*
bool BSL_Script_Interface::massErase(void)
{
  return protectedMassErase(0);
}/*
/*******************************************************************************
* Overloaded function
*******************************************************************************/
/*
bool BSL_Script_Interface::massErase(int cycles) 
{
  return protectedMassErase(cycles);
}/*

/*******************************************************************************
*
*******************************************************************************/
/*
bool BSL_Script_Interface::protectedMassErase(int cycles) 
{
    switch( mode )
	{
	case MODE_ROM:
      if( cycles != NULL )
	  {
	    for( ; cycles >0; cycles-- )
		{
		  if( ROM_massErase() != DATA_ACK_ROM )
		  {
            cycles = -1;
		  }
		}
	  }
	  else
	  {
        if( ROM_massErase() != DATA_ACK_ROM )
		{
          cycles = -1;
		}
	  }
	  if( cycles == -1 )
	  {
        return BSL_FAILED;
	  }
	  else
	  {
        return BSL_SUCCESS;
	  }
	  break;
	case MODE_5438:
	case MODE_FLASH:
      if( cycles != NULL )
	  {
	    for( ; cycles >0; cycles-- )
		{
		  if( Flash_massErase() != DATA_ACK_FLASH )
		  {
            cycles = -1;
		  }
		}
	  }
	  else
	  {
        if( Flash_massErase() != DATA_ACK_FLASH )
		{
          cycles = -1;
		}
	  }
	  if( cycles == -1 )
	  {
        return BSL_FAILED;
	  }
	  else
	  {
        return BSL_SUCCESS;
	  }
		break;
    case MODE_USB:
      if(  USB_massErase() == DATA_ACK_FLASH )
	  {
        return BSL_SUCCESS;
	  }
	  else
	  {
        return BSL_FAILED;
	  }
        break;
    default:
        return BSL_FAILED;
        break;
	}
}
*/

/*******************************************************************************
*
*******************************************************************************/
/*
]bool BSL_Script_Interface::RX_Password(void)
{
  bool status = BSL_FAILED;

  switch( mode )
  {
    case MODE_ROM:
    case MODE_5438:
    case MODE_FLASH:
      currentAddr = NO_DATA_READ;
      status = protected_RX_Password(defaultPass);
      break;
    case MODE_USB:
      currentAddr = NO_DATA_READ;
      if( USB_RX_Password(defaultPass) == DATA_ACK_FLASH )
	  {
        status = BSL_SUCCESS;
	  }
      break;
    default:
      break;
  }

  return status;
}
*/
/*******************************************************************************
*
*******************************************************************************/
/*bool BSL_Script_Interface::RX_Password(CString* RX_Pass)
{
  DataBlock pass;
  bool status = BSL_FAILED;
  CString tempRX_Pass = *RX_Pass;

  // Parse from CString to DataBlock
  currentAddr = NO_DATA_READ;
  pass = readTI_TextHex(bufferSize, &tempRX_Pass); 

  switch( mode )
  {
    case MODE_ROM:
    case MODE_5438:
    case MODE_FLASH:
      currentAddr = NO_DATA_READ;
      if(pass.numberOfBytes == NULL)
      {
        status = protected_RX_Password(defaultPass);
      }
      else
      {
        status = protected_RX_Password(pass);
      }
      break;
    case MODE_USB:
      currentAddr = NO_DATA_READ;
      if( USB_RX_Password(pass) == DATA_ACK_FLASH )
	  {
        status = BSL_SUCCESS;
	  }
      break;
    default:
      break;
  }

  return status;
}*/

/*******************************************************************************
*
*******************************************************************************/
/*
bool BSL_Script_Interface::protected_RX_Password(DataBlock pass)
{
  bool status = BSL_FAILED;

  // Sending password
  switch( mode )
  {
  case MODE_ROM:
    if( ROM_RX_Password( pass ) == DATA_ACK_ROM )
	{
      status = BSL_SUCCESS;
	}
 	break;
  case MODE_5438:
    if( Flash_RX_Password_5438(pass) == DATA_ACK_FLASH )
	{
      status = BSL_SUCCESS;
	}
  break;
  case MODE_FLASH:
    if( Flash_RX_Password(pass) == DATA_ACK_FLASH )
	{
      status = BSL_SUCCESS;
	}
  break;
  default:
    break;
  }

  return status;
}*/

unsigned char BSL_Script_Interface::initComm(void)
{
  unsigned char status;
  BSL_initialize_BSL((unsigned char *) "");
  switch(mode)
  {
    case MODE_ROM:
    case MODE_5438:
    case MODE_FLASH:
      //initializeCommPort( token, (mode == MODE_ROM)||(mode == MODE_FLASH));
      break;
    case MODE_USB:
      //status = USB_initializeCommPort();
      break;
    default:
      break;
  }
  return status;
}
/*
BYTE BSL_Script_Interface::numOfDevices(void)
{
  BYTE status = 0;
  switch(mode)
  {
    case MODE_USB:
      status = USB_numOfDevices();
      break;
    default:
      break;
  }
  return status;
}

void BSL_Script_Interface::invoke_BSL(void)
{
    invokeBSL();
}*/


/******************************************************************************/
/* Function:    bool closeComm(void)                                          */
/* Description: Closes COM port                                               */
/* Parameters:                                                                */
/*              void                                                          */
/* Returns:                                                                   */
/*              bool  : TRUE if successful close                              */
/******************************************************************************/
/*
unsigned char BSL_Script_Interface::closeComm(void)
{
  unsigned char status;
  switch(mode)
  {
    case MODE_ROM:
    case MODE_5438:
    case MODE_FLASH:
      status = closeCommPort();
      break;
    case MODE_USB:
      status = USB_closeCommPort();
      break;
    default:
      break;
  }
  return status;
}

void BSL_Script_Interface::setBufferSize(int newBufferSize)
{
  bufferSize = newBufferSize;
}
*/

/******************************************************************************/
/* Function:    BYTE BSL_Script_Interface::resetDevice(void)                  */
/* Description: Force a PUC reset on the MSP430.                              */
/* Support Dev:                                                               */
/*              USB                                                           */
/* Parameters:                                                                */
/*              void                                                          */
/* Returns:                                                                   */
/*              bool  : TRUE if successfully                                  */
/******************************************************************************/
/*
BYTE BSL_Script_Interface::resetDevice(void)
{
  BYTE status = 0;

  DataBlock wdtViolationData = {								
    {(char)0x00,(char)0x00},
    2,
    0x015C
  };

  switch(mode)
  {
    case MODE_ROM:
    case MODE_5438:
    case MODE_FLASH:
      break;
    case MODE_USB:
      // Force a password violation into WDT
      status = USB_RX_DataBlock(wdtViolationData);
      break;
    default:
      break;
  }
  return status;
}*/

/******************************************************************************/
/* Function:    bool BSL_Script_Interface::verifyMemory(CString* RX_Data)     */
/* Description: Verify Flash memory based on a preparsed string               */
/* Parameters:                                                                */
/*              CString* RX_Data                                              */
/* Returns:                                                                   */
/*              bool  : TRUE if memory is verified successfully               */
/******************************************************************************/
/*
bool BSL_Script_Interface::verifyMemory(CString* RX_Data)
{
  DataBlock parsedData;
  DataBlock readData;

  bool status = BSL_SUCCESS;
  CString tempRX_Data = *RX_Data;

  int addressMe[200];
  int i = 0;

  currentAddr = NO_DATA_READ;

  while( !(currentAddr == EOF) )
  {
    parsedData = readTI_TextHex(bufferSize, &tempRX_Data); // TODO: PROBLEM
    addressMe[i++] = parsedData.startAddr;
    
    switch( mode )
	  {
	    case MODE_ROM:
	      break;
	    case MODE_5438:
	    case MODE_FLASH:
	      break;
      case MODE_USB:
        // Read Data
        readData = TX_DataBlock(parsedData.startAddr, parsedData.numberOfBytes);

        // verify memory
        for(int i = 0; i < parsedData.numberOfBytes; i++)
        {
          if(parsedData.data[i] != readData.data[i])
          {
            status = BSL_FAILED;
            // Exit immediately on a failed verify
            return status;
          }
        }
        break;
      default:
        break;
	  }
  }

  return status;
}
*/

/******************************************************************************/
/* Function:    bool BSL_Script_Interface::verifyMemory(CString rxFilename)   */
/* Description: Verify Flash memory based on a filename TI-HEX file           */
/* Parameters:                                                                */
/*              CString rxFilename                                            */
/* Returns:                                                                   */
/*              bool  : TRUE if memory is verified successfully               */
/******************************************************************************/
/*
bool BSL_Script_Interface::verifyMemory(CString rxFilename)
{
  bool status = BSL_SUCCESS;

  char filename[300] = {0};
  if(rxFilename.GetLength() < 300)
  {
    for(int i = 0 ; i < rxFilename.GetLength() ; i++)
    {
      filename[i] = rxFilename[i];
    }

    if(openTI_TextForRead( filename ) == OPERATION_SUCCESSFUL)
    {
      DataBlock parsedData;
      DataBlock readData;

      while( moreDataToRead() )
      {
        parsedData = readTI_TextFile(48);
	      switch( mode )
	      {
          case MODE_USB:
            // Read Data
            readData = TX_DataBlock(parsedData.startAddr, parsedData.numberOfBytes);

            // verify memory
            for(int i = 0; i < parsedData.numberOfBytes; i++)
            {
              if(parsedData.data[i] != readData.data[i])
              {
                status = BSL_FAILED;
                // Exit immediately on a failed verify
                return status;
              }
            }
            break;
          default:
            break;
	      }
      }
    }
    else
    {
      status = BSL_FAILED;
    }
  }
  else
  {
    // filename too large
    status = BSL_FAILED;
  }

  return status;
}*/