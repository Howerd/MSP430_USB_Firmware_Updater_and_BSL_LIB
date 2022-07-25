MSP430F5529 USB BSL Firmware Upgrade Program

Link to the software download folder : https://www.inventio.co.uk/msp430_usb_prog.html

The Texas Instruments MSP430F5529 and family of microprocessors have a built-in BootStrap Loader that allows 
the application firmware running on the chip to be upgraded, using just a USB cable.

The MSP430_USB_Firmware_Updater.exe program links to the Texas Instruments BSL430.lib to connect to the 
USB HID endpoint in the MSP430F5529 chip, and upload firmware to it. 
It was based on the Texas Instruments MSP430_USB_Firmware_Updater_Example software, 
which seems no longer to be supported or supplied by Texas Instruments the original readme.txt is copied below.

Note : there was a bug in the TI supplied BSL430.dll/lib files that did not allow multiple “@xxxx” 
address sections - this is now fixed. The TXT file is parsed and converted to a binary file.

The MSP430_USB_Firmware_Updater.exe program, source code and Microsoft Visual Studio 2019 project and solution 
files are available here https://www.inventio.co.uk/msp430_usb_prog.html.

The TARGET.TXT file is produced by SwiftX from Forth, Inc. (https://www.forth.com/embedded/), and is the "distress" 
program for the Texas Instruments MSP-TS430PN80USB development board. 
Folder : ...\SwiftX\samples\distress\launchpad\exp320f5529lp\project.swx .
The LED on the board should flash SOS in Morse code.

Disclaimer : the software mentioned above is supplied as is, comes with absolutely no warranty, therefore you 
may only use it at your own risk.
License : the software mentioned above is supplied under the same license as the Texas Instruments software 
that it was based on.

If you find any bugs please let me know and I will try to fix them as quickly as possible.

Enjoy!
Howerd Oakford    www.inventio.co.uk   howerd@inventio.co.uk

2021 Dec 17


/*==========================================================================*\
|                                                                            |
|                                                                            |
| GUI Bootstrap Loader communication Demo Application                        |
|                                                                            |
|                                                                            |
|----------------------------------------------------------------------------|
| Developed using:      Microsoft Visual C++ Version 8.0                     |
|----------------------------------------------------------------------------|
| Author:               W. Goh                                               |
| Initial Version:      12-14-09                                             |
| Last Change:          07-13-12                                             |
|----------------------------------------------------------------------------|
| Version history:                                                           |
| 1.0.0 12/14/09 W. Goh	  Initial version, internal release                  |
| 1.0.1 12/21/09 W. Goh   Relocated firmware strings to a dedicated resource |
| 1.0.2 01/12/10 W. Goh   Added automatic CRC check features                 |
|                         Added automatic erase memory segment features      |
| 1.0.3 01/13/10 W. Goh   Fix Custom firmware in which it failed to check    |
|                         empty strings properly                             |
| 1.0.4 01/23/10 W. Goh   Added HID Firmware                                 |
|                         Auto extracts reset vector to be transmitted last  |
| 1.0.5 02/10/10 W. Goh   Fix erase segment timeout                          |
| 1.0.6 02/28/10 W. Goh   Fix HID progress bar download                      |
| 1.0.7 03/22/10 W. Goh   Fix erase segment execution speed                  |
| 1.0.8 04/14/10 W. Goh   Fix device detection when quickly disconnecting &  |
|                         reconnecting device.                               |
|                         Added functionality to modify RAM_BSL for custom   |
|                         VID/PID.                                           |
| 1.0.9 04/16/10 W. Goh   Fix erase segment bug where it missed 1 segment    |
| 1.1.0 04/27/10 W. Goh   Attaching license file                             |
| 1.1.1 07/09/10 W. Goh   Added various build support for VS Full and Express|
|                         Editions.                                          |
| 1.1.2 10/13/10 W. Goh   Integrated the lastest BSL scripter code.          |
|                         Added clock tick counts.                           |
|                         Used RAM_BSL v00.05.04.34                          |
| 1.1.3 10/22/10 W. Goh   Fixed library path under project settings to handle|
|                         path names with spaces.                            |
| 1.1.4 03/22/11 W. Goh   Fixed memory verification error.                   |
| 1.2.0 06/01/12 W. Goh   Ported project to support VS2010. Requires         |
|                         .NET Framework v4.0 and above. It also requires    |
|                         msvcr100.dll file to execute.                      |
| 1.2.1 07/13/12 W. Goh   Made MSP430_USB_Firmware_Updater as the default start-up project   |
| 1.3.0 08/08/13 W. Goh   Updated RAM_BSL to v00.07.87.37                    |
|                         Fixed BOR trigger sequence                         |
| 1.3.1 04/07/14 A. Bhat  Updated RAM_BSl to v00.07.08.38                    |
|----------------------------------------------------------------------------|
| Designed 2009 by Texas Instruments                                         |
*==========================================================================*/