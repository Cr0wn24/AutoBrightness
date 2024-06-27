## AutoBrightness

This is just a small program to set the brightness of the primary monitor depending on the time of day. 

### Building

Currently only builds for Windows with the Visual Studio (MSVC) compiler. Run ``build.bat`` with Visual Studio's environment variables
enabled and a ``build`` folder will be created where the exectuable resides. Double click the exectubale and it will run in the background.

### Usage

Edit the times and brightness values in [src/config_entries.c](src/config_entries.c) to your preference. Start the program
and it will continue to run in the background. It is recommended to [add this program to your startup programs](https://support.microsoft.com/en-us/windows/add-an-app-to-run-automatically-at-startup-in-windows-10-150da165-dcd9-7230-517b-cf3c295d89dd) so you
                                                                                                                don't have to think about starting the program each time you restart your computer.
                                                                                                                