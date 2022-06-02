cd %ARPDIR%\BEToolbox\Documentation

rmdir /S /Q doc
doxygen Doxygen.dox

%ARPDIR%\BridgeLink\RegFreeCOM\x64\Debug\MakeDocMap BEToolbox
rmdir /S /Q %ARPDIR%\BridgeLink\Docs\BEToolbox\%1\
mkdir %ARPDIR%\BridgeLink\Docs\BEToolbox\%1\
copy %ARPDIR%\BEToolbox\Documentation\doc\html\* %ARPDIR%\BridgeLink\Docs\BEToolbox\%1\
copy %ARPDIR%\BEToolbox\Documentation\BEToolbox.dm %ARPDIR%\BridgeLink\Docs\BEToolbox\%1\



