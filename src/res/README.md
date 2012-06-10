Project resource file (.rc) README
==================================

To compile res.rc into RemoteProgrammer, follow these steps:
------------------------------------------------------------
* Open Project properties
* Select **C/C++ Build - Settings**
* Select the **Build Steps** tab
 * Add `windres ../src/res/res.rc -o src/res.o` as Pre-build command
* Select **MinGW C++ Linker - Miscellaneous**
 * Add res.o to **Other objects**, should look like **"${workspace_loc:/${ProjName}/Release/src/res.o}"**
 
Attention:
----------
* windres will complain if the target folder does not exist (at first compile)
* Eclipse will complain if $buildConfig/src/res.o does not exist (at first compile)
* => Before first compile, you should create a dummy $buildConfig/src/res.o file to satisfy eclipse