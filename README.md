RemoteProgrammer
================

An application to program Microchip PIC microcontrollers using the ICD3 programmer over the network.  
The executable is stored in the Release/ folder. Compiled for Windows 7, 32bit.

Screenshots
-----------
![RemoteProgrammer](RemoteProgrammer/raw/master/screenshots/1.png "RemoteProgrammer")

Usage
-----
Start RemoteProgrammer and use netcat (or another socket client) to push a hex file into your controller:

    cat program.hex | nc 192.168.0.2 3000

Todo
----
* To customize ICD3CMD options, search for **ICD3CMD_OPTIONS** in windows registry. A user interface will be added soon.
* There is currently no way to customize the server port at runtime. You have to change it in the sources and recompile the programm.

Dependencies
------------

### Runtime
* MPLAB IDE (with ICD3 support)

### Compile
* wxWidgets (>= 2.8.12)


C++ Windows Development with Eclipse, MinGW and wxWidgets
---------------------------------------------------------

The following "howto" describes the development setup which was used for building RemoteProgrammer.

## Required software
* A windows environment (tested with **Windows 7 - 32 and 64 bit** inside [VirtualBox](https://www.virtualbox.org/), Windows XP may also work, haven't tried it yet)
* [Eclipse IDE for C/C++ Developers](http://eclipse.org/downloads/) (tested with Indigo 3.7.2)
* [MinGW](http://www.mingw.org/wiki/MinGW) (tested with [mingw-get-inst-20120426.exe](http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/mingw-get-inst-20120426/))
* [MSYS](http://www.mingw.org/wiki/MSYS) (the MinGW installer will install it)
* [wxWidgets source](http://www.wxwidgets.org/downloads/) (tested with [wxAll 2.8.12](http://prdownloads.sourceforge.net/wxwindows/wxWidgets-2.8.12.zip))

## Eclipse setup
* Download **Eclipse IDE for C/C++ Developers** (see link above)
* Extract the archive to some location
* Eclipse requires [Java](www.java.com/download/) to be installed on your computer, but this is mostly the case

## MinGW / MSYS setup
* Download the **MinGW Installer** (see link above) and run it
* Select "**Download latest repository catalogues**" if you get asked whether you want to use the pre-packaged or the latest catalogues
* Install to **C:\MinGW**
* On components selection step, you should at least choose
 * C Compiler
 * C++ Compiler
 * MSYS Basic System
 * MinGW Developer Toolkit
* After you hit install, the installer takes a few minutes to download and install the libraries and tools

## wxWidgets setup
* Download the **wxWidgets source** (see link above)
* Extract the archive to **C:\MinGW** (so that it becomes C:\MinGW\wxWidgets-2.8.12)
* Open the **MinGW Shell** (Start Menu - MinGW - MinGW Shell)
* Type `cd /mingw/wxWidgets-2.8.12`
* Type `./configure --prefix=/mingw --disable-shared`
* Type `make && make install`
* These last two commands may take upwards of ten minutes each to execute
* Once completed, you should be ready to compile wxWidgets C++ projects

## wxWidgets project setup
#### Minimal wxWidgets application
* Start Eclipse by executing eclipse.exe
* If this is the first run, you will get a "Welcome screen". Click the close button (the little "x") in the tab bar to close it
* Create a new C++ Project 
 * Select **File - New - C++ Project**
 * Choose a **Project name** (e.g. "sampleMinimal")
 * Choose **Executable/Empty Project** as Project type
 * Choose **MinGW GCC** as Toolchain
 * Click **Finish**
* You will now see the project in the **Project Explorer** on the left
 * Right click the project name, select **New - Source Folder**
 * Type "src" as **Folder name**
 * Click **Finish**
* Right click the "src" folder, select **New - Source File**
 * Type "minimal.cpp" as **Source File**
* Select **File - Open File..**
 * Browse to **C:\MinGW\wxWidgets-2.8.12\samples\minimal**
 * Select **minimal.cpp**
 * Copy the contents of the whole file ("**Edit - Select All**", "**Edit - Copy**")
 * Close the file (**File - Close**)
* Paste the code into your minimal.cpp ("**Edit - Select All**", "**Edit - Paste**")
* Save the file (**File - Save**)

##### Build properties
* Open **Project Properties** (Project - Properties)
* Select and expand **C/C++ Build** in the list on the left
* Choose **All configurations** as Configuration
* Select **C/C++ Build - Settings**
* Open the **Tool Settings** tab
* Select **GCC C++ Compiler - Miscellaneous**
 * Run the command `wx-config --cxxflags --static` in a MinGW Shell
 * Prepend the output to **Other flags**, so they should look like `-I/mingw/lib/wx/include/msw-ansi-release-static-2.8 -I/mingw/include/wx-2.8 -D__WXMSW__ -mthreads -c -fmessage-length=0`
* Select **MinGW C++ Linker**
 * In the **Command line pattern**, move the `${FLAGS}` entry from front to back so it should look like `${COMMAND} ${OUTPUT_FLAG} ${OUTPUT_PREFIX}${OUTPUT} ${INPUTS} ${FLAGS}`
* Select **MinGW C++ Linker - Miscellaneous**
 * Run the command `wx-config --libs --static` in a MinGW Shell
 * Append the output to **Linker flags**
 * Prepend "**-static-libstdc++ -static-libgcc**" to **Linker flags**
 * The **Linker flags** now should look like `-static-libstdc++ -static-libgcc -L/mingw/lib  -mthreads  -Wl,--subsystem,windows -mwindows /mingw/lib/libwx_msw_richtext-2.8.a /mingw/lib/libwx_msw_aui-2.8.a /mingw/lib/libwx_msw_xrc-2.8.a /mingw/lib/libwx_msw_qa-2.8.a /mingw/lib/libwx_msw_html-2.8.a /mingw/lib/libwx_msw_adv-2.8.a /mingw/lib/libwx_msw_core-2.8.a /mingw/lib/libwx_base_xml-2.8.a /mingw/lib/libwx_base_net-2.8.a /mingw/lib/libwx_base-2.8.a -lwxregex-2.8 -lwxexpat-2.8 -lwxtiff-2.8 -lwxjpeg-2.8 -lwxpng-2.8 -lwxzlib-2.8 -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 -lctl3d32 -ladvapi32 -lwsock32 -lgdi32`
 * Click **Apply**
* Click **OK**
* Eclipse should now be able to compile the program, select **Run - Run** and you should see the application running. If not, check if you made a mistake in a previous step.

##### General properties
Follow these steps to fix the "Unresolved inclusion" warnings and get auto-completion running
* Open **Project Properties** (Project - Properties)
* Expand **C/C++ General** in the list on the left
* Select **Indexer**
 * Tick **Enable project specific settings**
 * Tick **Index unused headers as C++ files**
 * Click **Apply**
* Select **Paths and Symbols** on the left
 * Choose **All configurations** as Configuration
 * In the **Includes** tab, select **GNU C++** as Language
 * Click **Add...** on the right
  * Click **File system...** and select **C:\MinGW\include\wx-2.8**
  * Click **OK**
 * Click **Add...** on the right
  * Click **File system...** and select **C:\MinGW\lib\wx\include\msw-ansi-release-static-2.8**
  * Click **OK**
* Click **Apply**
 * If dialog opens where you get asked whether you wish to rebuld the index, choose **Yes**
* Click **OK**

#### Stripping debug overhead from executables
* Open Project properties
* Select **C/C++ Build - Settings**
* Select the **Build Steps** tab
 * Select **Release** as Configuration
 * Add `strip <File>.exe` as Post-build command
* In case of RemoteProgrammer, this strips the executable down from 4,1MB to 2,5MB (64% smaller)

#### Compile resource information into the program
* See [src/res/README.md](RemoteProgrammer/raw/master/src/res/README.md)

## References
* [wiki.wxwidgets.org](http://wiki.wxwidgets.org)

Disclaimer
----------

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.