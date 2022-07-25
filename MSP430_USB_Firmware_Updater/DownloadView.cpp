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
#include "StdAfx.h"
#include "DownloadView.h"
#include <stdio.h>
#include <string.h>
#include "BSL_Definitions.h"

unsigned char * bslVersion;

/*******************************************************************************
 *Function:    RegisterUSBDevice
 *Description: This function registers a HID USB device to Windows.
 *Parameters:
 *             HWND hWnd
 *
 *Returns:
 *             BYTE              BSL_SUCCESS           - Success
 *                               BSL_USB_HANDLE_ERROR  - HID device handle error
 *
 *******************************************************************************/
BYTE MSP430_USB_Firmware_Updater::DownloadView::RegisterUSBDevice(HWND hWnd)
{
    return BSL_RegisterUSBDeviceNotification(hWnd);
}

/*******************************************************************************
 *Function:    UnRegisterUSBDevice
 *Description: This function unregisters a HID USB device to Windows.
 *Parameters:
 *             none
 *
 *Returns:
 *             BYTE              BSL_SUCCESS           - Success
 *                               BSL_USB_HANDLE_ERROR  - HID device handle error
 *
 *******************************************************************************/
BYTE MSP430_USB_Firmware_Updater::DownloadView::UnRegisterUSBDevice()
{
    return BSL_UnRegisterUSBDeviceNotification();
}

/*******************************************************************************
 *Function:    DownloadStartUp
 *Description: Basic startup of sequence of download a BSL firmware.
 *Parameters:
 *             BackgroundWorker^ worker  Any progress that will be reported back
 *                                       to GUI during execution
 *
 *             DoWorkEventArgs ^ e       Any events to be passed back once thread
 *                                       finish executing
 *
 *Returns:
 *             bool                      true  - Succesfully started BSL
 *                                       false - Falsed to start BSL
 *
 *******************************************************************************/
bool MSP430_USB_Firmware_Updater::DownloadView::DownloadStartUp(BackgroundWorker^ worker, DoWorkEventArgs ^ e , bool * massErased )
{
    System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Firmware::typeid));

    BYTE reply;
    DataBlock pass;
    DataBlock blankPass;

    String^ bslVidPidIndexString = "PID="+myPID+",VID="+myVID+",DEVICEINDEX="+myDeviceIndex;
    BYTE bslVidPidIndex[100] = {0}; // bslVidPidIndex should look like this
    // PID=0x200,VID=0x2047,DEVICEINDEX=0
    StringToChar(bslVidPidIndexString,bslVidPidIndex);

    // Initialize USB HID BSL
    BSL_setFamily( FAMILY_FLASH );
    BSL_setCom( COM_USB );

#ifdef DEBUG_VERBOSE
      // Activate Verbose mode
      BSL_SetVerbose(true);
#endif

    if(BSL_initialize_BSL((BYTE *) bslVidPidIndex) == BSL_DEVICE_NOT_FOUND) // Allows explicit declaration of a different
    {                                                    // VID or PID
        worker->ReportProgress(0,"USB device was unplugged!\r\n");
        e->Result = false;                      // Device programming failed, report back.
        return false;                           // End programming early.
    }

    // Get Password text
    String^ passwordStr = resources->GetString(L"password.Text");
    BYTE password[200];
    StringToChar(passwordStr,password);
    pass = BSL_readTI_Text((char *) password,32);
    blankPass = BSL_getDefaultPass();         // Get Blank password

    worker->ReportProgress(5,"");

    // Send Password... Causes mass erase if password is wrong!
    reply = BSL_RX_Password(pass);
    reply = true;   // Howerd Oakford 2021 Dec 13 force a mass erase every time
    if(reply)
    {
        // Wrong password provided.
        // Memory is being mass erased. Setting a 1sec delay.
        *massErased = true;
        worker->ReportProgress(5,"Using Mass erase command\r\n");
        Sleep(2000);
        // Memory is blank now. Re-send blank password to access device.
        reply = BSL_RX_Password(blankPass);
        if(reply)
        {
            worker->ReportProgress(10,"Password FAILED!\r\n");
            e->Result = false;
            return false;
        }
    }

    worker->ReportProgress(10,"Password Sent Successfully\r\n");

    // Get RAM_BSL Data
    BYTE RAM_BSL[20000];
    String^ ramBSLString = resources->GetString(L"RAM_BSL.Text");
    StringToChar(ramBSLString,RAM_BSL);

    // Send RAM BSL
    worker->ReportProgress(20,"Sending RAM BSL v" + resources->GetString(L"RAM_BSL_VERSION") + "\r\n");
    BSL_RX_TXT((char *) RAM_BSL, 1);
    BSL_LoadPC(0x2504);
    worker->ReportProgress(30,"Done RAM BSL v" + resources->GetString(L"RAM_BSL_VERSION") + "\r\n");

    // Close BSL connection
    BSL_close_BSL();

    worker->ReportProgress(35,"");
    Sleep(1000);
    worker->ReportProgress(40,"");
    Sleep(1000);
    worker->ReportProgress(45,"");
    Sleep(1000);

    startProgramTime = GetTickCount();

    // Re-initialize USB BSL
    BSL_setFamily( FAMILY_FLASH );
    BSL_setCom( COM_USB );
    if(BSL_initialize_BSL((BYTE *) bslVidPidIndex) == BSL_DEVICE_NOT_FOUND)
    {
        worker->ReportProgress(50,"USB device was unplugged!\r\n");
        e->Result = false;
        return false;
    }

    //Resend password depending if device has mass erased
    if(*massErased == true)
    {
        // Since device is assumed blank now, resend a blank password
        reply = BSL_RX_Password(blankPass);
    }
    else
    {
        reply = BSL_RX_Password(pass);
    }

    // Let's make sure the new RAM BSL has initiated
    // Query for BSL version
    bslVersion = BSL_TX_BSL_Version_String();

    // Get CI from device
    String^ CI("" + System::Convert::ToChar(bslVersion[16]) + System::Convert::ToChar(bslVersion[17]));
    // Get API from device
    String^ API("" + System::Convert::ToChar(bslVersion[25]) + System::Convert::ToChar(bslVersion[26]));
    // Get PI from device
    String^ PI("" + System::Convert::ToChar(bslVersion[33]) + System::Convert::ToChar(bslVersion[34]));

    // Get current version of RAM BSL
    String^ ramBslVersion = resources->GetString(L"RAM_BSL_VERSION");
    // Construct RAM BSL version that was read from device. Debug purposes
    String^ checkRamBslVersion("00." + CI + "." + API + "." + PI);

    if ((!strcmp((const char *) bslVersion, "ERROR")) ||
        (System::String::Compare(ramBslVersion, checkRamBslVersion)))
    {
        // If bslVersion returned an error or RAM_BSL does not match the current version, return from start-up
        worker->ReportProgress(50,"Failed to read data from RAM BSL\r\n");
        e->Result = false;
        return false;
    }

    // Password failed after sending RAM_BSL. Trap it here.
    if(reply)
    {
        worker->ReportProgress(50,"Password FAILED after sending RAM BSL!\r\n");
        e->Result = false;
        return false;
    }

    // Erase reset vector first
    reply = BSL_eraseSegment(0xFFFF);

    if (reply != BSL_ACK)
    {
        // Not properly started.
        worker->ReportProgress(50,"Failed to erase reset vector\r\n");
    }

    return true;
}

/*******************************************************************************
 *Function:    TriggerForcedPUC
 *Description: RESETS the MSP430 by forcing a PUC.
 *Parameters:
 *             BackgroundWorker^ worker  Any progress that will be reported back
 *                                       to GUI during execution
 *
 *             DoWorkEventArgs ^ e       Any events to be passed back once thread
 *                                       finish executing
 *
 *Returns:
 *             bool                      true  - Device RESET successfully
 *                                       false - Device FAILED to RESET
 *
 *******************************************************************************/
bool MSP430_USB_Firmware_Updater::DownloadView::TriggerForcedPUC(BackgroundWorker^ worker, DoWorkEventArgs ^ e )
{
    System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Firmware::typeid));

    // Force a PUC
    String^ forcedPUCStr = resources->GetString(L"Forced_PUC.Text");
    BYTE forcedPUC[100];
    StringToChar(forcedPUCStr,forcedPUC);
    worker->ReportProgress(90,"Resetting Device...\r\nStarting application\r\n");
    if( BSL_RX_TXT((char *) forcedPUC, 1) == BSL_ACK )
    {
        String^ ramBslVersion = resources->GetString(L"RAM_BSL_VERSION");
        // Version 00.06.05.34 and above requires another dummy command to trigger the real PUC.
        if (ramBslVersion->Contains("00.06.05.34") || ramBslVersion->Contains("00.07.08.38"))
        {
            // Send another PUC to trigger a real PUC.
            StringToChar(forcedPUCStr,forcedPUC);
            BSL_RX_TXT((char *) forcedPUC, 1);
        }
        return true;
    }
    else
    {
        return false;
    }
}

/*******************************************************************************
 *Function:    TriggerForcedBOR
 *Description: RESETS the MSP430 by forcing a BOR.
 *Parameters:
 *             BackgroundWorker^ worker  Any progress that will be reported back
 *                                       to GUI during execution
 *
 *             DoWorkEventArgs ^ e       Any events to be passed back once thread
 *                                       finish executing
 *
 *Returns:
 *             bool                      true  - Device RESET successfully
 *                                       false - Device FAILED to RESET
 *
 *******************************************************************************/
bool MSP430_USB_Firmware_Updater::DownloadView::TriggerForcedBOR(BackgroundWorker^ worker, DoWorkEventArgs ^ e )
{
    System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Firmware::typeid));

    // Force a PUC
    String^ forcedBORStr = resources->GetString(L"Forced_BOR.Text");
    BYTE forcedBOR[100];
    StringToChar(forcedBORStr,forcedBOR);
    worker->ReportProgress(90,"Resetting Device...\r\nStarting application\r\n");
    if( BSL_RX_TXT((char *) forcedBOR, 1) == BSL_ACK )
    {
        String^ ramBslVersion = resources->GetString(L"RAM_BSL_VERSION");
        // Version 00.06.05.34 and above requires another dummy command to trigger the real BOR.
        if (ramBslVersion->Contains("00.06.05.34") || ramBslVersion->Contains("00.07.08.38"))
        {
            // Send another BOR to trigger a real BOR.
            StringToChar(forcedBORStr,forcedBOR);
            BSL_RX_TXT((char *) forcedBOR, 1);
        }
        return true;
    }
    else
    {
        return false;
    }
}

/*
 * This function downloads any selected TI Hex file.
 * It has to be executed inside a BackgroundWorker thread.
 *
 * @Inputs  -  BackgroundWorker^ worker, DoWorkEventArgs ^ e
 *
 * @Outputs -  none
 *
 */
void MSP430_USB_Firmware_Updater::DownloadView::CustomSelectedFirmware(String ^ filenameString, BackgroundWorker^ worker, DoWorkEventArgs ^ e )
{
    System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Firmware::typeid));
    bool memoryMassErased = false;            // Tracks if memory has been massed erased to prevent


    // Transmit file based on filename
    BYTE filename[200] = { 0 };
    StringToChar(filenameString, filename);

    int fileStatus = GetFileAttributes((char*)filename);

    if (fileStatus == INVALID_FILE_ATTRIBUTES)
    {
        // no file found
        
        worker->ReportProgress(00, "Could not find file : " + filenameString + "\r\n");
        e->Result = true;
        // only erase the chip if have found the file to program it with...
        worker->ReportProgress(00, "Please click on 'Select Firmware' in Step 1'\r\n");
        worker->ReportProgress(00, "   and Browse for a TXT or BIN file.'\r\n");
        return;
    }
    
    // another segment erase.
    // Howerd Oakford 2021 Dec 13 DownloadStartUp always returns true, because we force a mass erase every time
    if(!DownloadStartUp(worker,e,&memoryMassErased))
    {
        // Close BSL connection
        BSL_close_BSL();

        // Failed the first time. Try again
        worker->ReportProgress(50,"Unsuccessful in starting the BSL. Restarting. \r\n");
        if(!DownloadStartUp(worker,e,&memoryMassErased))
        {
            // Quit after 2 tries
            e->Result = false;
            return;
        }
    }

    // 
    // Howerd Oakford 2021 Dec 13 Note : we force a mass erase every time
    // Erases necessary memory segments
    // If memory is not massed erased, erase individual memory segments
    if (!memoryMassErased)
    {
        worker->ReportProgress(60, "Erasing memory segments \r\n");
        EraseDataSegment_File((char*)filename);
    }

    worker->ReportProgress(70, "Sending " + filenameString + "\r\n");
    BSL_RX_TXT_File((char*)filename, 1);      // the 1 indicates fast mode
    worker->ReportProgress(75, "Firmware Sent\r\n");

    // Run CRC check
    worker->ReportProgress(80,"Verifying memory\r\n");
    if( CRC_Check_File((char *) filename) )
    {
        // Download error
        worker->ReportProgress(85,"Memory verification error\r\n");
        e->Result = false;
        return;
    }
    else
    {
        // Memory verified successfully
        worker->ReportProgress(85,"Memory successfully verified\r\n");
    }
    stopProgramTime = GetTickCount();
    totalProgramTime = (stopProgramTime - startProgramTime) / 100;

    worker->ReportProgress(90,"Total programming time is " + totalProgramTime/10  + "." + totalProgramTime % 10 + " s \r\n");

    // Force a BOR
    TriggerForcedBOR(worker, e);

    worker->ReportProgress(100,"Done.");
    e->Result = true;
}

/*******************************************************************************
 *Function:    StringToChar
 *Description: This function parsses String^ to character array
 *Parameters:
 *             String^ str               String value to be parssed
 *             BYTE * character          pointer to character array to be constructed
 *
 *Returns:
 *             none
 *
 *******************************************************************************/
void MSP430_USB_Firmware_Updater::DownloadView::StringToChar(String^ str, BYTE * character)
{
    for(int i = 0 ; i < str->Length ; i++)
    {
        character[i] = (BYTE) str[i];
    }
}

/*******************************************************************************
 *Function:    EraseDataSegment
 *Description: Erases only specific memory segments based on a firmware text requirement
 *Parameters:
 *             BYTE * dataArray          Firmware array of characters
 *
 *Returns:
 *             UINT                      BSL_SUCCESS
 *
 *******************************************************************************/
UINT MSP430_USB_Firmware_Updater::DownloadView::EraseDataSegment( BYTE * dataArray )
{
    char * dataArrayLine;
    char * next_newDataArray;
    char * newDataArray = 0;
    unsigned long currentAddr = NO_DATA_READ;
    unsigned long startAddr;
    UINT BSL_bufferSize = 512;
    unsigned char bslStatus;

    newDataArray = (char *) dataArray;

    if( currentAddr == NO_DATA_READ )
    {
        sscanf_s( &newDataArray[1], "%x\n", &currentAddr );
        newDataArray = strstr((char* ) dataArray, (char* )"\n");
    }

    startAddr = currentAddr;

    // Erase the first segment
    bslStatus = BSL_eraseSegment(currentAddr);
    if (bslStatus != BSL_ACK)
    {
        return BSL_FAILED;
    }

    // Establish string to get first token line
    dataArrayLine = strtok_s( newDataArray, (char* )"\n" , &next_newDataArray);

    while( currentAddr != EOF )
    {
        unsigned int bytesRead = 0;
        int status = 1;
        startAddr = currentAddr;

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
                // Erase current address before changing to the next address
                bslStatus = BSL_eraseSegment(currentAddr);
                if (bslStatus != BSL_ACK)
                {
                    return BSL_FAILED;
                }
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
                    bytesRead++;
                    currentAddr++;
                }
            }

            dataArrayLine = strtok_s( NULL, (char* )"\n"  , &next_newDataArray);
            if(dataArrayLine == NULL)
            {
                currentAddr = EOF;
                status = 0;
            }
        }
        while( (status != 0) && (bytesRead < BSL_bufferSize) );

        // Erase segment after every 512 bytes
        if(currentAddr != EOF)
        {
            bslStatus = BSL_eraseSegment(currentAddr);
            if (bslStatus != BSL_ACK)
            {
                return BSL_FAILED;
            }
        }
    }

    return BSL_SUCCESS;
}

/*******************************************************************************
 *Function:    EraseDataSegment_File
 *Description: Erases only specific memory segments based on a firmware text requirement
 *Parameters:
 *             char * fileName           Filename containing TI Hex
 *
 *Returns:
 *             UINT                      BSL_SUCCESS
 *                                       ERROR_OPENING_FILE
 *
 *******************************************************************************/
UINT MSP430_USB_Firmware_Updater::DownloadView::EraseDataSegment_File( char * fileName )
{
    DataBlock data;
    INT BSL_bufferSize = 512;

    // Open file based on string
    if( BSL_openTI_TextForRead(fileName) == ERROR_OPENING_FILE )
    {
        return ERROR_OPENING_FILE;
    }

    // copy line by line until EOF to dataArray
    while( BSL_moreDataToRead_TextFile() )
    {
        data = BSL_readTI_TextFile(BSL_bufferSize);

        // Erase segment after every 512 bytes
        BSL_eraseSegment(data.startAddr);
    }

    BSL_closeTI_Text();
    return BSL_SUCCESS;
}

/*******************************************************************************
 *Function:    Calculate_CRC
 *Description: Calculates CRC value based on a DataBlock
 *Parameters:
 *             DataBlock data            DataBlock to be calculated
 *
 *Returns:
 *             UINT                      calculated CRC value
 *
 *******************************************************************************/
UINT MSP430_USB_Firmware_Updater::DownloadView::Calculate_CRC( DataBlock data )
{
    CRC^ crcObject = gcnew CRC(0);
    crcObject->CrcInit(0xFFFF);

    for( UINT i = 0; i < data.numberOfBytes ; i++ )
    {
        crcObject->CrcInput(data.data[i]);
    }
    return crcObject->GetCrcValue();
}

/*******************************************************************************
 *Function:    CRC_Check_File
 *Description: Checks if CRC from a file matches CRC from MSP430
 *Parameters:
 *             char * fileName           Filename containing TI Hex
 *
 *Returns:
 *             UINT                      BSL_SUCCESS
 *                                       BSL_FAILED
 *
 *******************************************************************************/
UINT MSP430_USB_Firmware_Updater::DownloadView::CRC_Check_File( char * fileName )
{
    DataBlock data;
    int timeToLeave;
    int result = BSL_SUCCESS;

    CRC^ crcObject = gcnew CRC(0);
    INT BSL_bufferSize = DATA_BLOCK_SIZE;

    UINT actualCrcValue;
    UINT calculatedCrcValue;

    // Open file based on string
    if( BSL_openTI_TextForRead(fileName) == ERROR_OPENING_FILE )
    {
        return ERROR_OPENING_FILE;
    }

    timeToLeave = 0;
    // copy line by line until EOF to dataArray
    while (timeToLeave == 0)
    {
        data = BSL_readTI_TextFile(BSL_bufferSize);
        
        actualCrcValue = CRC_Check(data.startAddr , data.numberOfBytes , NULL);

        calculatedCrcValue = Calculate_CRC(data);

        // test this after we have processed the block marked with EOF
        if (data.eof == 1)
        {
            timeToLeave = 1;
        }

        if(actualCrcValue != calculatedCrcValue)
        {
            result = BSL_FAILED;
        }
    }

    BSL_closeTI_Text();
    return BSL_SUCCESS;
}

/*******************************************************************************
 *Function:    CRC_Check
 *Description: Checks CRC based on an array of data TI Hex
 *Parameters:
 *             BYTE * dataArray          Array of data containing TI Hex
 *
 *Returns:
 *             UINT                      BSL_SUCCESS
 *                                       BSL_FAILED
 *
 *******************************************************************************/
UINT MSP430_USB_Firmware_Updater::DownloadView::CRC_Check( BYTE * dataArray )
{
    DataBlock data;
    char * dataArrayLine;
    char * next_newDataArray;
    char * newDataArray = 0;
    unsigned long currentAddr = NO_DATA_READ;
    UINT BSL_bufferSize = 5000;
    UINT actualCrcValue;
    UINT calculatedCrcValue;

    newDataArray = (char *) dataArray;

    if( currentAddr == NO_DATA_READ )
    {
        sscanf_s( &newDataArray[1], "%x\n", &currentAddr );
        newDataArray = strstr((char* ) dataArray, (char* )"\n");
    }
    data.startAddr = currentAddr;

    // Establish string to get first token line
    dataArrayLine = strtok_s( newDataArray, (char* )"\n" , &next_newDataArray);

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
                    sscanf_s( &dataArrayLine[stringPosition], "%hhx", &data.data[bytesRead] );
                    bytesRead++;
                    currentAddr++;
                }
            }

            dataArrayLine = strtok_s( NULL, (char* )"\n"  , &next_newDataArray);
            if(dataArrayLine == NULL)
            {
                currentAddr = EOF;
                status = 0;
            }
        }
        while( (status != 0) && (bytesRead < BSL_bufferSize) );
        data.numberOfBytes = bytesRead;

        // Get the actual CRC value from device
        actualCrcValue = CRC_Check(data.startAddr , data.numberOfBytes , NULL);
        calculatedCrcValue = Calculate_CRC(data);
        if(actualCrcValue != calculatedCrcValue)
        {
            return BSL_FAILED;
        }
    }

    return BSL_SUCCESS;
}

/*******************************************************************************
 *Function:    CRC_Check
 *Description: Checks CRC based on an array of data TI Hex
 *Parameters:
 *             DWORD addr                Address to start checking CRC
 *             DWORD length              Length to check for CRC
 *             DWORD crcValue            CRC value to compare too or NULL to
 *                                       return a CRC value
 *
 *Returns:
 *             UINT                      (0xXXXX) CRC value from device
 *                                       CRC_SUCCESS
 *                                       CRC_FAILED
 *
 *******************************************************************************/
UINT MSP430_USB_Firmware_Updater::DownloadView::CRC_Check(DWORD addr , DWORD length , DWORD crcValue)
{
    if( BSL_CRC_Check( addr, length) == BSL_ACK )
    {
        UINT crc;
        dataBuffer db = BSL_get_RX_Buffer();
        crc = db.data[1];
        crc |= db.data[2]<<8;

        if( crcValue == NULL )
        {
            // If crcValue == NULL, return the actual calculated CRC value from device
            return crc;
        }
        else if( crc == crcValue )
        {
            return CRC_SUCCESS;
        }
        else
        {
            return CRC_FAILED;
        }
    }
    else
    {
        return CRC_FAILED;
    }
}

String^ MSP430_USB_Firmware_Updater::DownloadView::ResetVectorExtractor(String^ str , BYTE * newPassword , DataBlock * dataPassword)
{
    unsigned long currentAddr = NO_DATA_READ;
    dataPassword->startAddr = 0xFFFF;

    String^ strCopy = "";
    String^ strLine;
    int stringPointer;
    int nextStringPointer;
    int sizeStringPointer;

    strcpy_s((char *) newPassword,100,(char *) "@FFFE\r\nFF FF\r\nq");

    if( currentAddr == NO_DATA_READ )
    {
        // Get the first token
        stringPointer = str->IndexOf("\n");
        sizeStringPointer = stringPointer - 2;

        // Get current address
        currentAddr = System::Convert::ToUInt32(str->Substring(1 , sizeStringPointer) ,16);
        strCopy = String::Concat(str->Substring(0,stringPointer+1));
    }

    // Establish string to get first token line
    nextStringPointer = str->IndexOf("\n",stringPointer+1);
    sizeStringPointer = nextStringPointer - stringPointer;
    strLine = str->Substring( stringPointer+1 , sizeStringPointer );

    do
    {
        int stringLength=0;
        int stringPosition=0;
        stringLength = strLine->Length;

        if( strLine[0] == '@' )
        {
            // Get current address
            sizeStringPointer = nextStringPointer - stringPointer;
            currentAddr = System::Convert::ToUInt32(strLine->Substring(1 , sizeStringPointer - 3) ,16);
        }
        else if ( strLine[0] == 'q' || strLine[0] == 'Q' )
        {
            currentAddr = EOF;
        }
        else
        {
            for( stringPosition = 0; stringPosition < (stringLength-2); stringPosition+=3 )
            {
                if(currentAddr == 0xFFFE)
                {
                    dataPassword->startAddr = currentAddr;

                    // Pad RESET vector values
                    newPassword[7] = (BYTE) strLine[stringPosition];
                    newPassword[8] = (BYTE) strLine[stringPosition+1];

                    dataPassword->data[0] = System::Convert::ToByte(strLine->Substring( stringPosition , 2 ) ,16);
                    // Remove character from string line
                    strLine = strLine->Remove(stringPosition , 2);
                    // Insert 0xFF character into string line
                    strLine = strLine->Insert(stringPosition , "FF");
                }
                else if(currentAddr == 0xFFFF)
                {
                    String^ test = strLine->Substring(stringPosition , 2);

                    // Pad RESET vector values
                    newPassword[10] = (BYTE) strLine[stringPosition];
                    newPassword[11] = (BYTE) strLine[stringPosition+1];

                    dataPassword->data[1] = System::Convert::ToByte(strLine->Substring( stringPosition , 2 ) ,16);
                    // Remove character from string line
                    strLine = strLine->Remove(stringPosition , 2);
                    // Insert 0xFF character into string line
                    strLine = strLine->Insert(stringPosition , "FF");
                }
                currentAddr++;
            }
        }

        strCopy = String::Concat(strCopy,strLine->Substring(0,sizeStringPointer));

        stringPointer = nextStringPointer;
        nextStringPointer = str->IndexOf("\n",stringPointer+1);
        sizeStringPointer = nextStringPointer - stringPointer;
        if(nextStringPointer == -1)
        {
            currentAddr = EOF;
            strCopy = String::Concat(strCopy,"q");
        }
        else
        {
            strLine = str->Substring( stringPointer+1 , sizeStringPointer );
        }
    }
    while( (currentAddr != EOF) );
    dataPassword->numberOfBytes = 2;

    return strCopy;
}
