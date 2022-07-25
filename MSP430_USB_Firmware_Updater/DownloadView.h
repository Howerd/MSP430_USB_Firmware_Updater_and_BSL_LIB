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
#pragma once

#include "BSL430_lib.h"
#include "CRC.h"
#include <windows.h>
#include <Winuser.h>
#include <Dbt.h>
#include "Firmware.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Data::SqlTypes;
using namespace System::Reflection;
using namespace System::Resources;
using namespace System::Diagnostics;

namespace MSP430_USB_Firmware_Updater {

  /// <summary>
  /// Summary for DownloadView
  ///
  /// WARNING: If you change the name of this class, you will need to change the
  ///          'Resource File Name' property for the managed resource compiler tool
  ///          associated with all .resx files this class depends on.  Otherwise,
  ///          the designers will not be able to interact properly with localized
  ///          resources associated with this form.
  /// </summary>
  public ref class DownloadView : public System::Windows::Forms::Form
  {


  public:
    DownloadView(void)
    {
      InitializeComponent();
      InitializeVidPidFirmware();

      // Construct Software version for title
      Assembly^ myAsm = Assembly::GetExecutingAssembly();
      AssemblyName^ assemName = myAsm->GetName();
      int majorRevision = assemName->Version->Major;
      int minorRevision = assemName->Version->Minor;
      int buildNumber = assemName->Version->Build;
      this->Text = this->Text + majorRevision + "." + minorRevision + "." + buildNumber;

      InitializeBackgroundWorker();
    }
  
  protected: static String^ myVID;
  protected: static String^ myPID;
  protected: static String^ myDeviceIndex = "0";
  protected: static DWORD numberOfUSBDevicesConnected = 0;
  protected: static bool scanDeviceAgain = false;

  protected: static DWORD startProgramTime;
  protected: static DWORD stopProgramTime;
  private: System::Windows::Forms::GroupBox^ groupBox2;
  private: System::Windows::Forms::GroupBox^ groupBox3;
  private: System::Windows::Forms::Button^ buttonAbout;
  private: System::Windows::Forms::TextBox^ textBox1;


  protected:
  protected: static DWORD totalProgramTime;

  private: void CustomSelectedFirmware(String ^ filenameString, BackgroundWorker^ worker, DoWorkEventArgs ^ e );
  private: BYTE RegisterUSBDevice(HWND hWnd);
  private: BYTE UnRegisterUSBDevice();
  private: bool DownloadStartUp(BackgroundWorker^ worker, DoWorkEventArgs ^ e , bool * massErased );
  private: bool TriggerForcedPUC(BackgroundWorker^ worker, DoWorkEventArgs ^ e );
  private: bool TriggerForcedBOR(BackgroundWorker^ worker, DoWorkEventArgs ^ e );
  private: UINT EraseDataSegment( BYTE * dataArray );
  private: UINT EraseDataSegment_File( char * fileName );
  private: UINT Calculate_CRC( DataBlock data );
  private: UINT CRC_Check(DWORD addr , DWORD length , DWORD crcValue);
  private: UINT CRC_Check( BYTE * dataArray );
  private: UINT CRC_Check_File( char *  filename );
  private: String^ ResetVectorExtractor(String^ str , BYTE * newPassword , DataBlock * dataPassword);

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~DownloadView()
    {
      // Unregister USB devices before quiting program.
      UnRegisterUSBDevice();
      if (components)
      {
        delete components;
      }
    }

  private: void StringToChar(String^ str , BYTE * character);
  private: System::Windows::Forms::Button^  btnClose;
  private: System::Windows::Forms::GroupBox^  groupBox1;
  private: System::Windows::Forms::RadioButton^  radioButton3;
  private: System::Windows::Forms::RadioButton^  radioButton2;
  private: System::Windows::Forms::RadioButton^  radioButton1;
  private: System::Windows::Forms::RadioButton^  radioButton4;
  private: System::Windows::Forms::Button^  btnUpgrade;

  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Label^  label2;

  private: System::Windows::Forms::ProgressBar^  progressBar;

  private: System::Windows::Forms::TextBox^  filenameTextBox;

  private: System::Windows::Forms::Button^  btnBrowse;
  private: System::Windows::Forms::OpenFileDialog^  openFileDialogBrowse;
  private: System::Windows::Forms::TextBox^  statusBox;
  private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
  private: System::ComponentModel::BackgroundWorker^  backgroundWorkerCheckDevice;
  private: System::Windows::Forms::Label^  labelNumOfDevices;

  protected: 

  protected: 

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
        System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(DownloadView::typeid));
        this->btnClose = (gcnew System::Windows::Forms::Button());
        this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
        this->labelNumOfDevices = (gcnew System::Windows::Forms::Label());
        this->btnBrowse = (gcnew System::Windows::Forms::Button());
        this->filenameTextBox = (gcnew System::Windows::Forms::TextBox());
        this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
        this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
        this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
        this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
        this->btnUpgrade = (gcnew System::Windows::Forms::Button());
        this->label1 = (gcnew System::Windows::Forms::Label());
        this->label2 = (gcnew System::Windows::Forms::Label());
        this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
        this->openFileDialogBrowse = (gcnew System::Windows::Forms::OpenFileDialog());
        this->statusBox = (gcnew System::Windows::Forms::TextBox());
        this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
        this->backgroundWorkerCheckDevice = (gcnew System::ComponentModel::BackgroundWorker());
        this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
        this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
        this->buttonAbout = (gcnew System::Windows::Forms::Button());
        this->textBox1 = (gcnew System::Windows::Forms::TextBox());
        this->groupBox1->SuspendLayout();
        this->groupBox2->SuspendLayout();
        this->groupBox3->SuspendLayout();
        this->SuspendLayout();
        // 
        // btnClose
        // 
        this->btnClose->Location = System::Drawing::Point(344, 400);
        this->btnClose->Name = L"btnClose";
        this->btnClose->Size = System::Drawing::Size(141, 39);
        this->btnClose->TabIndex = 0;
        this->btnClose->Text = L"&Close";
        this->btnClose->UseVisualStyleBackColor = true;
        this->btnClose->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &DownloadView::btnClose_MouseClick);
        // 
        // groupBox1
        // 
        this->groupBox1->Controls->Add(this->btnBrowse);
        this->groupBox1->Controls->Add(this->filenameTextBox);
        this->groupBox1->Controls->Add(this->radioButton4);
        this->groupBox1->Location = System::Drawing::Point(12, 12);
        this->groupBox1->Name = L"groupBox1";
        this->groupBox1->Size = System::Drawing::Size(473, 86);
        this->groupBox1->TabIndex = 1;
        this->groupBox1->TabStop = false;
        this->groupBox1->Text = L"1. Select which firmware to download (optional)";
        // 
        // labelNumOfDevices
        // 
        this->labelNumOfDevices->AutoSize = true;
        this->labelNumOfDevices->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
            System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
        this->labelNumOfDevices->Location = System::Drawing::Point(45, 62);
        this->labelNumOfDevices->Name = L"labelNumOfDevices";
        this->labelNumOfDevices->Size = System::Drawing::Size(49, 13);
        this->labelNumOfDevices->TabIndex = 10;
        this->labelNumOfDevices->Text = L"              ";
        this->labelNumOfDevices->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        this->labelNumOfDevices->Click += gcnew System::EventHandler(this, &DownloadView::labelNumOfDevices_Click);
        // 
        // btnBrowse
        // 
        this->btnBrowse->Enabled = false;
        this->btnBrowse->Location = System::Drawing::Point(133, 26);
        this->btnBrowse->Name = L"btnBrowse";
        this->btnBrowse->Size = System::Drawing::Size(75, 23);
        this->btnBrowse->TabIndex = 4;
        this->btnBrowse->Text = L"&Browse";
        this->btnBrowse->UseVisualStyleBackColor = true;
        this->btnBrowse->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &DownloadView::btnBrowse_MouseClick);
        // 
        // filenameTextBox
        // 
        this->filenameTextBox->Location = System::Drawing::Point(6, 55);
        this->filenameTextBox->Name = L"filenameTextBox";
        this->filenameTextBox->ReadOnly = true;
        this->filenameTextBox->Size = System::Drawing::Size(456, 20);
        this->filenameTextBox->TabIndex = 3;
        this->filenameTextBox->Text = L"TARGET.TXT";
        // 
        // radioButton4
        // 
        this->radioButton4->AutoSize = true;
        this->radioButton4->Location = System::Drawing::Point(6, 29);
        this->radioButton4->Name = L"radioButton4";
        this->radioButton4->Size = System::Drawing::Size(100, 17);
        this->radioButton4->TabIndex = 2;
        this->radioButton4->TabStop = true;
        this->radioButton4->Text = L"Select Firmware";
        this->radioButton4->UseVisualStyleBackColor = true;
        this->radioButton4->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &DownloadView::selectedRadioBtn);
        // 
        // radioButton3
        // 
        this->radioButton3->Location = System::Drawing::Point(0, 0);
        this->radioButton3->Name = L"radioButton3";
        this->radioButton3->Size = System::Drawing::Size(104, 24);
        this->radioButton3->TabIndex = 0;
        // 
        // radioButton2
        // 
        this->radioButton2->Location = System::Drawing::Point(0, 0);
        this->radioButton2->Name = L"radioButton2";
        this->radioButton2->Size = System::Drawing::Size(104, 24);
        this->radioButton2->TabIndex = 0;
        // 
        // radioButton1
        // 
        this->radioButton1->Location = System::Drawing::Point(0, 0);
        this->radioButton1->Name = L"radioButton1";
        this->radioButton1->Size = System::Drawing::Size(104, 24);
        this->radioButton1->TabIndex = 0;
        // 
        // btnUpgrade
        // 
        this->btnUpgrade->BackColor = System::Drawing::SystemColors::ControlLightLight;
        this->btnUpgrade->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
        this->btnUpgrade->Enabled = false;
        this->btnUpgrade->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)));
        this->btnUpgrade->FlatAppearance->BorderSize = 8;
        this->btnUpgrade->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Yellow;
        this->btnUpgrade->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
        this->btnUpgrade->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->btnUpgrade->Location = System::Drawing::Point(343, 104);
        this->btnUpgrade->Name = L"btnUpgrade";
        this->btnUpgrade->Size = System::Drawing::Size(142, 63);
        this->btnUpgrade->TabIndex = 3;
        this->btnUpgrade->Text = L"Upgrade Firmware";
        this->btnUpgrade->UseVisualStyleBackColor = false;
        this->btnUpgrade->Click += gcnew System::EventHandler(this, &DownloadView::btnUpgrade_Click);
        this->btnUpgrade->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &DownloadView::btnUpgrade_MouseClick);
        // 
        // label1
        // 
        this->label1->AutoSize = true;
        this->label1->Location = System::Drawing::Point(15, 21);
        this->label1->Name = L"label1";
        this->label1->Size = System::Drawing::Size(127, 39);
        this->label1->TabIndex = 4;
        this->label1->Text = L"Hold down the BSL keys\r\nor Switch and connect the \r\ndevice's USB cable";
        this->label1->Click += gcnew System::EventHandler(this, &DownloadView::label1_Click_1);
        // 
        // label2
        // 
        this->label2->AutoSize = true;
        this->label2->Location = System::Drawing::Point(6, 21);
        this->label2->Name = L"label2";
        this->label2->Size = System::Drawing::Size(133, 26);
        this->label2->TabIndex = 5;
        this->label2->Text = L"Hold the On key (\'|\') and\r\nclick on Upgrade Firmware";
        // 
        // progressBar
        // 
        this->progressBar->Location = System::Drawing::Point(12, 198);
        this->progressBar->Name = L"progressBar";
        this->progressBar->Size = System::Drawing::Size(473, 16);
        this->progressBar->Step = 5;
        this->progressBar->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
        this->progressBar->TabIndex = 7;
        // 
        // openFileDialogBrowse
        // 
        this->openFileDialogBrowse->FileName = L"openFileDialogBrowse";
        this->openFileDialogBrowse->Filter = L"*.txt *.bin|*.txt;*.bin|All files (*.*)|*.*";
        // 
        // statusBox
        // 
        this->statusBox->BackColor = System::Drawing::SystemColors::Window;
        this->statusBox->Location = System::Drawing::Point(12, 231);
        this->statusBox->Multiline = true;
        this->statusBox->Name = L"statusBox";
        this->statusBox->ReadOnly = true;
        this->statusBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
        this->statusBox->Size = System::Drawing::Size(314, 208);
        this->statusBox->TabIndex = 9;
        // 
        // backgroundWorker1
        // 
        this->backgroundWorker1->WorkerReportsProgress = true;
        // 
        // groupBox2
        // 
        this->groupBox2->Controls->Add(this->labelNumOfDevices);
        this->groupBox2->Controls->Add(this->label1);
        this->groupBox2->Location = System::Drawing::Point(12, 104);
        this->groupBox2->Name = L"groupBox2";
        this->groupBox2->Size = System::Drawing::Size(153, 79);
        this->groupBox2->TabIndex = 11;
        this->groupBox2->TabStop = false;
        this->groupBox2->Text = L"2. Set BSL mode";
        this->groupBox2->Enter += gcnew System::EventHandler(this, &DownloadView::groupBox2_Enter);
        // 
        // groupBox3
        // 
        this->groupBox3->Controls->Add(this->label2);
        this->groupBox3->Location = System::Drawing::Point(176, 104);
        this->groupBox3->Name = L"groupBox3";
        this->groupBox3->Size = System::Drawing::Size(150, 79);
        this->groupBox3->TabIndex = 12;
        this->groupBox3->TabStop = false;
        this->groupBox3->Text = L"3. Upgrade";
        // 
        // buttonAbout
        // 
        this->buttonAbout->FlatAppearance->BorderColor = System::Drawing::Color::White;
        this->buttonAbout->FlatAppearance->BorderSize = 0;
        this->buttonAbout->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
        this->buttonAbout->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"buttonAbout.Image")));
        this->buttonAbout->ImageAlign = System::Drawing::ContentAlignment::BottomCenter;
        this->buttonAbout->Location = System::Drawing::Point(344, 231);
        this->buttonAbout->Name = L"buttonAbout";
        this->buttonAbout->Size = System::Drawing::Size(141, 163);
        this->buttonAbout->TabIndex = 13;
        this->buttonAbout->Text = L"Help            About ";
        this->buttonAbout->TextAlign = System::Drawing::ContentAlignment::BottomRight;
        this->buttonAbout->UseVisualStyleBackColor = true;
        this->buttonAbout->Click += gcnew System::EventHandler(this, &DownloadView::button1_Click_1);
        // 
        // textBox1
        // 
        this->textBox1->BackColor = System::Drawing::Color::Yellow;
        this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
        this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->textBox1->Location = System::Drawing::Point(343, 166);
        this->textBox1->Name = L"textBox1";
        this->textBox1->Size = System::Drawing::Size(142, 19);
        this->textBox1->TabIndex = 5;
        this->textBox1->Text = L"Found BSL device";
        this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
        this->textBox1->Visible = false;
        // 
        // DownloadView
        // 
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->BackColor = System::Drawing::SystemColors::ControlLightLight;
        this->ClientSize = System::Drawing::Size(497, 451);
        this->Controls->Add(this->textBox1);
        this->Controls->Add(this->buttonAbout);
        this->Controls->Add(this->groupBox3);
        this->Controls->Add(this->statusBox);
        this->Controls->Add(this->progressBar);
        this->Controls->Add(this->btnUpgrade);
        this->Controls->Add(this->groupBox1);
        this->Controls->Add(this->btnClose);
        this->Controls->Add(this->groupBox2);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
        this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
        this->MaximizeBox = false;
        this->Name = L"DownloadView";
        this->Text = L"MSP430 USB Firmware Upgrade Program";
        this->Load += gcnew System::EventHandler(this, &DownloadView::DownloadView_Load);
        this->groupBox1->ResumeLayout(false);
        this->groupBox1->PerformLayout();
        this->groupBox2->ResumeLayout(false);
        this->groupBox2->PerformLayout();
        this->groupBox3->ResumeLayout(false);
        this->groupBox3->PerformLayout();
        this->ResumeLayout(false);
        this->PerformLayout();

    }

    void InitializeVidPidFirmware(void)
    {
      System::ComponentModel::ComponentResourceManager^  firmwareResources = (gcnew System::ComponentModel::ComponentResourceManager(Firmware::typeid));

      this->myVID = firmwareResources->GetString(L"VID");
      this->myVID = padHexStringValue(this->myVID);

      this->myPID = firmwareResources->GetString(L"PID");
      this->myPID = padHexStringValue(this->myPID);
    }

    String^ padHexStringValue(String^ hexString)
    {
      if(hexString->Length < 4)
      {
        hexString = hexString->Insert(2,"000");
      }
      else if(hexString->Length < 5)
      {
        hexString = hexString->Insert(2,"00");
      }
      else if(hexString->Length < 6)
      {
        hexString = hexString->Insert(2,"0");
      }
      return hexString;
    }

    void InitializeBackgroundWorker(void)
    {
      backgroundWorker1->ProgressChanged += gcnew ProgressChangedEventHandler( this, &DownloadView::backgroundWorker1_ProgressChanged );
      backgroundWorker1->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &DownloadView::backgroundWorker1_RunWorkerCompleted );
      backgroundWorker1->DoWork += gcnew DoWorkEventHandler( this, &DownloadView::backgroundWorker1_DownloadFirmware);

      backgroundWorkerCheckDevice->DoWork += gcnew DoWorkEventHandler( this, &DownloadView::backgroundWorkerCheckDevice_DoWork);
      backgroundWorkerCheckDevice->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &DownloadView::backgroundWorkerCheckDevice_RunWorkerCompleted );
    }

#pragma endregion

  private: System::Void btnClose_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
        int temp = this->OwnedForms->Length;
        this->Close();
    }

   public: void DoTheDownloadWindow(void) {
        System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(DownloadView::typeid));

        btnUpgrade->Enabled = false;
        statusBox->Text = "Starting\r\n";
        backgroundWorker1->RunWorkerAsync();
    }

  private: System::Void btnUpgrade_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {            
        DoTheDownloadWindow();
    }

// This event handler is where the actual, potentially time-consuming work is done.
void backgroundWorker1_DownloadFirmware( Object^ sender, DoWorkEventArgs^ e )
{
  // Get the BackgroundWorker that raised this event.
    BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);

    //String^ test = filenameTextBox->Text->;
    if(!filenameTextBox->Text->Equals(""))
    {
        // Download custom firmware from a TI Hex file
        CustomSelectedFirmware(filenameTextBox->Text, worker, e);
    }
    else
    {
        CustomSelectedFirmware("TARGET.TXT", worker, e);
    //      worker->ReportProgress(0,"No file specified\r\n");
    //      e->Result = false;
    }
//  else
//  {
//    worker->ReportProgress(0,"Please Select the firmware first\r\n");
//    e->Result = false;
//  }
}

// This event handler updates the progress bar.
void backgroundWorker1_ProgressChanged( Object^ /*sender*/, ProgressChangedEventArgs^ e )
{
  this->progressBar->Value = e->ProgressPercentage;
  if(e->UserState->ToString() != "")
  {
    this->statusBox->AppendText(e->UserState->ToString());
  }
}

// This event handler deals with the results of the
// background operation.
void backgroundWorker1_RunWorkerCompleted( Object^ /*sender*/, RunWorkerCompletedEventArgs^ e )
{
  if ( e->Error != nullptr )
  {
  
  }
  else if ( e->Cancelled )
  {
 
  }
  else
  {
    bool successOrFail = System::Convert::ToBoolean(e->Result->ToString());
    
    if(successOrFail)
    {
      // Re-enable back Upgrade button when downloading is complete successfully
      //btnUpgrade->Enabled = true;
    }
    else
    {
      backgroundWorkerCheckDevice->RunWorkerAsync();
    }
  }
}

// Event handler thread for Checking # of devices
void backgroundWorkerCheckDevice_DoWork( Object^ sender, DoWorkEventArgs^ e )
{
  // Get the BackgroundWorker that raised this event.
  BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);

  // Scan # of USB HID devices based on VID and PID
  DWORD numOfDevices = BSL_NumOfUSBDevices(System::Convert::ToUInt16(myVID->Replace("0x",""),16),
                                           System::Convert::ToUInt16(myPID->Replace("0x",""),16));

  // Set # of USB devices into results inside this thread (safely)
  e->Result = numOfDevices; 
}

// This event handler deals with the results of the
// background operation for checking the number of USB devices.
void backgroundWorkerCheckDevice_RunWorkerCompleted( Object^ /*sender*/, RunWorkerCompletedEventArgs^ e )
{
  if ( e->Error != nullptr )
  {
  
  }
  else if ( e->Cancelled )
  {
 
  }
  else
  {
    // Read passed value from backgroundWorkerCheckDevice thread
    numberOfUSBDevicesConnected = System::Convert::ToUInt32(e->Result->ToString());

    String^ structText;

    if(numberOfUSBDevicesConnected > 0)
    {
      // structText = ("Found " + numberOfUSBDevicesConnected + " device");
      structText = ("Step 3 --> ");

   //   if(numberOfUSBDevicesConnected > 1)
   //   {
   //     structText = ("Found " + numberOfUSBDevicesConnected + " devices");
   //   }

      // If there is a USB device connected && any radio buttons are selected, enable the Upgrade button
      // if((radioButton1->Checked) || (radioButton2->Checked) || (radioButton3->Checked) || (radioButton4->Checked))
      {
        btnUpgrade->Enabled = true;
        textBox1->Visible = true;
      }
    }
    else if(numberOfUSBDevicesConnected == 0)
    {
      structText = ("              ");     // ("No device connected");

      // If there are NO USB device connected, disable the Upgrade button
      btnUpgrade->Enabled = false;
      textBox1->Visible = false;
    }  

    // Update label
    labelNumOfDevices->Text = structText;
  }

  if(scanDeviceAgain)
  {
    scanDeviceAgain = false;
    backgroundWorkerCheckDevice->RunWorkerAsync(); // Run check number of devices thread
  }
}

/*******************************************************************************
*Function:    OnHandleCreated
*Description: Function that overrides OnHandleCreated for the purpose of 
*             registering USB notification to windows
*Parameters: 
*             EventArgs^ e          Events arguments
*Returns:
*             void
*******************************************************************************/
protected: virtual void OnHandleCreated(System::EventArgs^ e) override
{
  // Runs the original constructor of OnHandleCreated
  Form::OnHandleCreated(e);

  // Register USB devices
  BYTE status = RegisterUSBDevice((HWND) Handle.ToPointer());

  if(status == 0x00)
  {
    // If registration is good, scan for # of USB devices
    backgroundWorkerCheckDevice->RunWorkerAsync();
  }
  else
  {
    labelNumOfDevices->Text = "ERROR: Unable to register device";
  }
}

/*******************************************************************************
*Function:    WndProc
*Description: Overrides windows form processor function
*Parameters: 
*             Message% m            Takes windows message
*Returns:
*             void
*******************************************************************************/
protected: virtual void WndProc(Message% m) override
{
  ParseMessages(m);
  Form::WndProc(m);	                        // pass message on to base form
}

/*******************************************************************************
*Function:    ParseMessages
*Description: Parses message from Windows to identify if device was removed or
*             inserted
*Parameters: 
*             Message% m            Takes windows message
*Returns:
*             void
*******************************************************************************/
private: void ParseMessages(Message% m)
{
  if (m.Msg == WM_DEVICECHANGE)	          // Received a device change message.
  {                                       // A USB device was either inserted or removed.
    switch (m.WParam.ToInt32())	          // Check the W parameter to see if a device was inserted or removed
    {
      case DBT_DEVICEARRIVAL:	            // Device was inserted
        if(!(backgroundWorker1->IsBusy) && !(backgroundWorkerCheckDevice->IsBusy))
        {
          backgroundWorkerCheckDevice->RunWorkerAsync(); // Run check number of devices thread
        }
        else
        {
          scanDeviceAgain = true;
        }
        break;
      case DBT_DEVICEREMOVECOMPLETE:	    // Device was removed
        if(!(backgroundWorker1->IsBusy) && !(backgroundWorkerCheckDevice->IsBusy))
        {
          backgroundWorkerCheckDevice->RunWorkerAsync(); // Run check number of devices thread
        }
        else
        {
          scanDeviceAgain = true;
        }
        break;
    }
  }
}


private: System::Void btnBrowse_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
  if ( openFileDialogBrowse->ShowDialog() == System::Windows::Forms::DialogResult::OK )
  {
    String^ selectedFilename = openFileDialogBrowse->FileName;
    filenameTextBox->Text = selectedFilename;
  }
}

private: System::Void selectedRadioBtn(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
  // Enable Upgrade button when any radio button is selected and more than 1 device is connected.
  if(numberOfUSBDevicesConnected > 0)
  {
    btnUpgrade->Enabled = true;
  }

  // if(radioButton4->Checked)
  {
    btnBrowse->Enabled = true;
  }
//  else
//  {
//    btnBrowse->Enabled = false;
//  }
}
private: System::Void DownloadView_Load(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void btnUpgrade_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label1_Click_1(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void groupBox2_Enter(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void labelNumOfDevices_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void richTextBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}       
private: System::Void webBrowser1_DocumentCompleted(System::Object^ sender, System::Windows::Forms::WebBrowserDocumentCompletedEventArgs^ e) {
}
private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
    System::Diagnostics::Process::Start("https://www.inventio.co.uk/msp430_usb_prog.html");
}
};
}
