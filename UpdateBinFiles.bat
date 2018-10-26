REM - Script to prepare for Release

SET BINTARGET=bin
SET REGFREECOM=\ARP\BridgeLink\RegFreeCOM


copy /Y %REGFREECOM%\Win32\Release\BEToolbox.dll	%BINTARGET%\Win32\
copy /Y %REGFREECOM%\x64\Release\BEToolbox.dll		%BINTARGET%\x64\
copy /Y \ARP\BridgeLink\BEToolbox.chm	              	%BINTARGET%\

copy /Y Images\*.png					%BINTARGET%\Images\