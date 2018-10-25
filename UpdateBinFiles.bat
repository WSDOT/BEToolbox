REM - Script to prepare for Release

SET BINTARGET=bin


copy /Y \ARP\PGSuper\RegFreeCOM\Win32\Release\BEToolbox.dll	%BINTARGET%\Win32\
copy /Y \ARP\PGSuper\RegFreeCOM\x64\Release\BEToolbox.dll	%BINTARGET%\x64\
copy /Y \ARP\PGSuper\RegFreeCOM\BEToolbox.chm	              	%BINTARGET%\

copy /Y Images\*.png						%BINTARGET%\Images\