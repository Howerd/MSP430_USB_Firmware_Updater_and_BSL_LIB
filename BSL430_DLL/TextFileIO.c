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

#include "TextFileIO.h"

FILE* fileHandle;
int fileType;       // 0 = not set, 1 = raw *.img binary file, assumed to start at 0x4400, 2 = Texas Instruments *.TXT file
unsigned int currentAddr = NO_DATA_READ;

/*******************************************************************************
 *Function:    openTI_TextForRead
 *Description: Opens a TXT file for reading
 *Parameters:
 *             char *filename        The string containing the file name
 *Returns:
 *             SUCCESSFUL_OPERATION  The file is open for reading
 *             ERROR_OPENING_FILE    Some error occured during file open
 *******************************************************************************/
int openTI_TextForRead( char *filename )
{
    errno_t err;
    currentAddr = NO_DATA_READ;
    if ((err = fopen_s(&fileHandle, filename, "rb")) != 0)
    // if( (fileHandle = fopen( filename, "rb" )) == 0 )  // deprecated
    {
        return ERROR_OPENING_FILE;
    }
    else
    {
        char fileExtension[3] = { "BIN" };
        int length = strlen(filename);
        int position = length - 3;
        if (position < 0) position = 0;

        // check the file type
        fileType = 2;   // assume a TXT file
        char TexasExtension[3] = { "TXT" };
        for (int i = 0 ; i < 3 ; i++)
        {
            fileExtension[i] = ( filename[(position + i)] & 0xDF ); // convert file extension to upper case ".TXT"
            if (fileExtension[i] != TexasExtension[i])
            {
                fileType = 1;   // assume a raw binary file
                break;
            }
        }
        return OPERATION_SUCCESSFUL;
    }
}

/*******************************************************************************
 *Function:    openTI_TextForWrite
 *Description: Opens a TXT file for writing with append
 *Parameters:
 *             char *filename        The string containing the file name
 *Returns:
 *             SUCCESSFUL_OPERATION  The file is open for reading
 *             ERROR_OPENING_FILE    Some error occured during file open
 *******************************************************************************/
int openTI_TextForWrite( char *filename )
{
    errno_t err;
    currentAddr = NO_DATA_READ;
    if ((err = fopen_s(&fileHandle, filename, "a+")) != 0)
    // if( (fileHandle = fopen( filename, "a+" )) == 0 )  // deprecated
    {
        return ERROR_OPENING_FILE;
    }
    else
    {
        return OPERATION_SUCCESSFUL;
    }
}

/*******************************************************************************
 *Function:    endTI_TextWrite
 *Description: Writes the final 'q' to a TI TEXT file and closes it
 *Parameters:
 *             none
 *Returns:
 *             none
 *******************************************************************************/
void endTI_TextWrite()
{
    fprintf(fileHandle,"q\n");
    closeTI_Text();
}

/*******************************************************************************
 *Function:    closeTI_Text
 *Description: closes access to a TI TEXT file
 *Parameters:
 *             none
 *Returns:
 *             none
 *******************************************************************************/
void closeTI_Text()
{
    fclose( fileHandle );
}

/*******************************************************************************
 *Function:    moreDataToRead
 *Description: checks whether an end-of-file was hit during read
 *Parameters:
 *             none
 *Returns:
 *             1                     if an EOF has not been hit
 *             0                     if an EOF has been hit
 *******************************************************************************/
int moreDataToRead()
{
    return !(currentAddr == TXT_EOF);
}

void initializeDataToRead()
{
    currentAddr = NO_DATA_READ;
}
/*******************************************************************************
 *Function:    writeTI_TextFile
 *Description: writes a block of data in TI TEXT format to the current file
 *Parameters:
 *             DataBlock data        The DataBlock structure to write
 *Returns:
 *             none
 *******************************************************************************/
/*
void writeTI_TextFile( DataBlock data )
{

unsigned int bytesWritten = 0;
if( (currentAddr == NO_DATA_READ) || (currentAddr != data.startAddr) )
{
fprintf(fileHandle, "@%05X\n", data.startAddr);
currentAddr = data.startAddr;
}
for( bytesWritten = 0; bytesWritten < data.numberOfBytes; bytesWritten++,currentAddr++ )
{
fprintf(fileHandle, "%02X", data.data[bytesWritten]);
if( ((bytesWritten+1)%16 == 0) || (bytesWritten+1 == data.numberOfBytes) )
{
fprintf(fileHandle, "\n");
} // if
else
{
fprintf(fileHandle, " ");
}
}
}
 */
/*******************************************************************************
 *Function:    writeTI_TextFile
 *Description: writes a block of data in TI TEXT format to the current file
 *Parameters:
 *             int length                 The address of bytes
 *             unsigned char *data        The array to write
 *             int length                 The amount of bytes
 *Returns:
 *             none
 *******************************************************************************/
void writeTI_TextFile( int addr, unsigned char *data, int length )
{
    int i;
    fprintf(fileHandle, "@%05X", addr);
    for( i = 0; i < length; i++)
    {
        if( i%16 == 0 )
        {
            fprintf(fileHandle, "\n");
        }
        else
        {
            fprintf(fileHandle, " ");
        }

        fprintf(fileHandle, "%02X", data[i]);
        /*
        if( i == 0 )
        {
        fprintf(fileHandle, " ");
    }
        else if( (i%16 == 0) || (i == length-1) || (i == 15))
        {
        fprintf(fileHandle, "\n");
    }
        else
        {
        fprintf(fileHandle, " ");
    }
         */
    }
    fprintf(fileHandle, "\n");

}

#ifdef use_old_readTI_TextFile

/*******************************************************************************
 *Function:    readTI_TextFile
 *Description: reads the given number of bytes from a TI TEXT file
 *Parameters:
 *             int bytesToRead       The maximum number of bytes to read from the file
 *Returns:
 *             A Datablock structure with the requested bytes
 *******************************************************************************/
DataBlock readTI_TextFile(int bytesToRead)
{
    DataBlock returnBlock;
    int bytesRead = 0;
    char string[50];
    char* status;

    if( currentAddr == NO_DATA_READ )
    {
        fgets( string, sizeof string, fileHandle );
        sscanf_s(&string[1], "%x\n", &currentAddr);
    }
    returnBlock.startAddr = currentAddr;

    returnBlock.eof = 0;    // not at the end of the files yet
    do
    {
        int stringLength=0;
        int stringPosition=0;
        status = fgets( string, sizeof(string), fileHandle );
        stringLength = strlen( string );
        if(status == NULL)
        {
            currentAddr = EOF;
            returnBlock.eof = 1;
        }
        else if( string[0] == '@' )
        {
            sscanf_s(&string[1], "%x\n", &currentAddr);
            status = 0;
        }
        else if ( string[0] == 'q' || string[0] == 'Q' )
        {
            status = 0;
            currentAddr = EOF;
            returnBlock.eof = 1;
        }
        else
        {
            for( stringPosition = 0; stringPosition < (stringLength-3); stringPosition+=3 )
            {
                sscanf_s(&string[stringPosition], "%hhx" , &returnBlock.data[bytesRead] );
                bytesRead++;
                currentAddr++;
            }
        }
    }
    while( (status != 0) && (bytesRead < bytesToRead) );
    returnBlock.numberOfBytes = bytesRead;
    return returnBlock;
}

#else // use_old_readTI_TextFile 

// the new version : Howerd Oakford 2021 Dec 17

///////////////////////////////////////////////////////////////////////////////
// Support functions for readTI_TextFile()
///////////////////////////////////////////////////////////////////////////////

#define SIZE_OF_BinaryImage (0x20000)       // up to 256K bytes (The MSP430F5529 has 128K bytes of FLASH) 
#define DEFAULT_START_OF_FLASH ( 0x4400 )   // for the MSP430F5529

unsigned char BinaryImage[ SIZE_OF_BinaryImage ];
int BinaryImageLoaded = 0;
int BinaryImageIndex = DEFAULT_START_OF_FLASH;      // an index into the BinaryImage array

// #define SIZE_OF_string (0x400)       // must be a power of 2
 
// get the next character from the file 
char get_next_char (void)
{
    char myChar;

    myChar = fgetc(fileHandle);
    return(myChar);
}

// convert an ASCII hexadecimal character (0 to 9, a to f or A to F) to its digit value
int to_digit ( char myChar )
{   
    int myValue;

    myValue = ( myChar - '0' ) & 0xDF ; // convert (a to f) to (A to F)
    if( myValue > 0x09 )  myValue -= 0x07;  // convert A to F to their digit value 

    return( myValue );
}

// get the next n character BigEndian hex number from the file 
int get_next_hex_value (int numberOfDigits)
{
    int myValue = 0;
    int i;

    for( i = 0 ; i < numberOfDigits ; i++ )
    {
    
        myValue  = myValue << 4;                // shift the 4 bit value up (BigEndian)
        myValue |= to_digit( get_next_char() );
    }

    return( myValue );
}

// put theByte into the next position in the BinaryImage array
void store_to_BinaryImage_8 ( unsigned char theByte )
{
    BinaryImage[ ( BinaryImageIndex - DEFAULT_START_OF_FLASH ) ] = theByte;
    BinaryImageIndex++;
}

// get all data from the Texas Instruments TXT file into the BinaryImage array 
void load_TI_TextFile (void)    
{
    int bytesRead;
    int required_number_of_linefeeds;
    int i;
    int timeToleave;
    unsigned int FileCRC32;
 
    bytesRead = 0;

    i = 0;
    BinaryImageIndex = DEFAULT_START_OF_FLASH;  // the default for the MSP430F5529
    timeToleave = 0;
    while( timeToleave == 0 )
    {
        char myChar = get_next_char();       
        switch( myChar )
        {
            case '@' : 
            {
                // '@' gives the start address for the following lines in the TARGET.TXT file
                BinaryImageIndex = get_next_hex_value( 4 );
                if (currentAddr == NO_DATA_READ)
                {
                    currentAddr = BinaryImageIndex;
                }
                // update the start address in the returnBlock, 
                // returnBlock.startAddr = currentAddr;
                break; 
            }

            case ' ' :  // space
            case 0x0D : // carriage return
            {
                // crs and spaces are allowed, but have no effect
                // do nothing - we ignore spaces and cr's
                break;
            }

            case 0x0A : // linefeed
            {
                // no action required, ignore linefeeds
                break;
            }

            case 'q' : 
            case 'Q' : 
            {
                timeToleave = 1;
                break;
            }
            
            case '0':
            case '1':            
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            {
                int myDataByte;
                myDataByte  = ( to_digit( myChar ) ) << 4 ;    // the high 4 bits of the value
                myDataByte |= to_digit( get_next_char() );     // the low  4 bits of the value
                store_to_BinaryImage_8( myDataByte );  
                
                if (bytesRead >= SIZE_OF_BinaryImage)
                {
                    timeToleave = 1;
                }
                else
                {
                    bytesRead++;
                }
                break;
            }

            default :
            {
                timeToleave = 1;
                break;
            }
        }
        i++;
        if (i > (SIZE_OF_BinaryImage * 4) )   // 4 * is large enough to hold the data
        {
            // just in case we do not find a 'q'
            timeToleave = 1;
        }
        if (timeToleave) break;
    }          
}

int BinaryImageArraySize;

/*******************************************************************************
 *Function:    readTI_TextFile
 *Description: reads the given number of bytes from a TI TEXT file
 *Parameters:
 *             int bytesToRead       The maximum number of bytes to read from the file
 *Returns:
 *             A Datablock structure with the requested bytes
 *******************************************************************************/

// return a DataBlock containing the given number of bytes at the 
// currentAddr offset in the BinaryImage
// the TARGET.TXT of TARGET.BIN file has been is parsed or copied, as required
// Note that this function requires that the file has already been opened, 
// and will later be closed.
DataBlock readTI_TextFile (int bytesToRead)
{
    DataBlock returnBlock;
    int i;
    unsigned int FileCRC32;

    if (currentAddr == NO_DATA_READ)    // if no file has been loaded yet
    {      
        // fill the binary image with FF's
        memset( BinaryImage, 0xFF, SIZE_OF_BinaryImage );

        BinaryImageArraySize = 0;   // start off with no data in the BinaryImage array
        
        if (fileType == 2)  // fileType is set when the file is opened
        {
            // a Texas Instruments TXT format file
            load_TI_TextFile();     // also sets currentAddr to the first @XXXX that it finds in the TXT file
        }
        else
        {
            // any other extension is assumed to be a raw binary file
            // so just load it into the array
            for (i = 0 ; i < (SIZE_OF_BinaryImage) ; i++ )
            {
                BinaryImage[i] = get_next_char();  // Note : fgets does not seem to work...
            }
            currentAddr = DEFAULT_START_OF_FLASH;
        }
        BinaryImageLoaded = 1;

        crcInit();
        FileCRC32 = crcFast(BinaryImage, SIZE_OF_BinaryImage);

        // find the size of the data that needs to be FLASHed
        BinaryImageArraySize = SIZE_OF_BinaryImage;  // start at the maximum
        for (i = 0; i < SIZE_OF_BinaryImage; i++)
        {
            if (BinaryImage[((SIZE_OF_BinaryImage - 1) - i)] == 0xFF)
            {
                BinaryImageArraySize--;
            }
            else
            {
                break;
            }
        }

        #ifdef WRITE_BINARY_TEST_FILE
        // for testing only
        errno_t err;
        FILE* myfileHandle;
        err = fopen_s(&myfileHandle, "TARGET_new.bin", "wb");
        if (err == 0)
        {
            fwrite(BinaryImage, sizeof(char), BinaryImageArraySize, myfileHandle);
        }
        fclose(myfileHandle);
        #endif
    }
    
    returnBlock.startAddr = currentAddr;
    returnBlock.numberOfBytes = bytesToRead;
    returnBlock.eof = 0;        // mark as not the End of "File"

    if ( (currentAddr + bytesToRead) >= (BinaryImageArraySize + DEFAULT_START_OF_FLASH ) )
    {
        returnBlock.eof = 1;        // mark the End of "File"
        BinaryImageLoaded = 0;      // so we load the file again next time
    }

    memcpy( returnBlock.data, &BinaryImage[ ( currentAddr - DEFAULT_START_OF_FLASH ) ], bytesToRead );

    currentAddr += bytesToRead;     // update the global address pointer for next time we are called

    return returnBlock;    
}

#endif  // use_old_readTI_TextFile
