REM - Script to prepare for Release

SET BINTARGET=bin
SET REGFREECOM=\ARP\BridgeLink\RegFreeCOM


xcopy /Y /d %REGFREECOM%\Win32\Release\BEToolbox.dll		%BINTARGET%\Win32\
xcopy /Y /d %REGFREECOM%\x64\Release\BEToolbox.dll		%BINTARGET%\x64\
xcopy /Y /d %REGFREECOM%\Win32\Release\BEToolboxXML.dll		%BINTARGET%\Win32\
xcopy /Y /d %REGFREECOM%\x64\Release\BEToolboxXML.dll		%BINTARGET%\x64\
xcopy /Y /d Images\*.png					%BINTARGET%\Images\