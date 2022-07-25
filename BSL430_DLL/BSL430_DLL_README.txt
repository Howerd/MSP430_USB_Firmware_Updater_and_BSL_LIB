/*==========================================================================*\
|                                                                            |
|                                                                            |
| GUI Bootstrap Loader communication Demo Application                        |
|                                                                            |
|                                                                            |
|----------------------------------------------------------------------------|
| Developed using:      Microsoft Visual C++ Version 8.0                     |
|----------------------------------------------------------------------------|
| Usage:                                                                     |
|   1. Download & install the latest version of WDK from Microsoft.          |
|      http://www.microsoft.com/whdc/devtools/wdk/wdkpkg.mspx                |
|   2. Right click on HidDevice.c file --> Properties                        |
|   3. Navigate to C/C++ --> General --> Additional Include Directories      |
|   4. Include:                                                              |
|      a. c:\<WinDDK Install>\<Build Version>\inc\api\                       |
|      b. c:\<WinDDK Install>\<Build Version>\inc\crt\                       |
|   5. Navigate to Linker --> General --> Additional Include Directories     |
|   6. Include:                                                              |
|      a. c:\<WinDDK Install>\<Build Version>\lib\wxp\i386                   |
|                                                                            |
|----------------------------------------------------------------------------|
| Author:               W. Goh                                               |
| Initial Version:      02-20-07                                             |
| Last Change:          06-01-12                                             |
|----------------------------------------------------------------------------|
| Version history:                                                           |
| 0.0.1.0 02/20/07 W. Goh	  Internal version, no release                   |
| 0.9.0.0 03/20/09 W. Goh	  Adapted for using with MSP430F5529 HID stack   |
| 1.0.1.0 04/10/10 W. Goh	  Adapted for using with multiple USB devices    |
| 1.0.2.0 04/24/10 W. Goh	  Added 2 new functions to register and          |
|                             unregister USB device for auto notification    |
| 1.1.0.0 06/01/12 W. Goh	  Ported project to support VS2010.              |
|                                                                            |
|----------------------------------------------------------------------------|
| Designed 2009 by Texas Instruments                                         |
*==========================================================================*/