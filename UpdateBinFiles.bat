REM - Script to prepare for Release

SET BINTARGET=bin
SET REGFREECOM=%ARPDIR%\BridgeLink\RegFreeCOM


xcopy /Y /d %REGFREECOM%\x64\Release\BEToolbox.dll		%BINTARGET%\x64\
xcopy /Y /d %REGFREECOM%\x64\Release\BEToolboxXML.dll		%BINTARGET%\x64\
xcopy /Y /d Images\*.png					%BINTARGET%\Images\
xcopy /Y /d USGS\2014\*.bin                                     %BINTARGET%\