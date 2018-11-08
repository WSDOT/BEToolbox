cd \ARP\BEToolbox\Documentation

rmdir /S /Q doc
doxygen Doxygen.dox

\ARP\BridgeLink\RegFreeCOM\x64\Debug\MakeDocMap BEToolbox
rmdir /S /Q \ARP\BridgeLink\Docs\BEToolbox\%1\
mkdir \ARP\BridgeLink\Docs\BEToolbox\%1\
copy \ARP\BEToolbox\Documentation\doc\html\* \ARP\BridgeLink\Docs\BEToolbox\%1\
copy \ARP\BEToolbox\Documentation\BEToolbox.dm \ARP\BridgeLink\Docs\BEToolbox\%1\



