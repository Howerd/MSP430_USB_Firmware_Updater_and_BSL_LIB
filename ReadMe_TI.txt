/*==========================================================================*\
|                                                                            |
|                                                                            |
| MSP430 Field Firmware Updater                                              |
|                                                                            |
|                                                                            |
|----------------------------------------------------------------------------|
| Developed using:      Microsoft Visual Studio 2019                         |
|----------------------------------------------------------------------------|
| Author:               W. Goh                                               |
|----------------------------------------------------------------------------|
|                                                                            |
|  There are 2 solution workspace attached:                                  |
|    1. MSP430_USB_Firmware_Updater.sln - this project recompiles the USB Field Firmware code|
|       This project does not require Microsoft WDK to recompile.            |
|    2. MSP430_USB_Firmware_Updater_wBSL430DLL.sln - this project recompiles both BSL430 DLL |
|       and USB Field Firmware code. This project requires Microsoft WDK     |
|       to recompile. See BSL430_DLL_README.txt inside BSL430_DLL folder.    |
|    3. Each workspace supports VS Express and Full edition. Select either   |
|       Express or Full Edition under Configuration Manager for the build.   |
|       The difference between 2 builds is that Full version will compiled   |
|       with the resource file which contains file version and manufacturer's|
|       information                                                          |
|----------------------------------------------------------------------------|
| Designed 2009 by Texas Instruments                                         |
*==========================================================================*/