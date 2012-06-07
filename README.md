RemoteProgrammer
================

An application to program Microchip PIC microcontrollers using the ICD3 programmer over the network.  
The executable is stored in the Release/ folder. Compiled for Windows 7, 64bit.

Screenshots
-----------
![RemoteProgrammer](RemoteProgrammer/raw/master/screenshots/1.png "RemoteProgrammer")

Usage
-----
Start RemoteProgrammer and use netcat (or another socket client) to push a hex file into your controller:

    cat program.hex | nc 192.168.0.2 3000

Todo
----
* There is currently no way to customize the server port or ICD3CMD options at runtime.  
If you're running a different chip than a PIC32MX795F512H, you have to change the parameter in the sources and recompile the programm.

Dependencies
------------

### Runtime
* MPLAB IDE (with ICD3 support)

### Compile
* wxWidgets (>= 2.8.12)

Disclaimer
----------

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.