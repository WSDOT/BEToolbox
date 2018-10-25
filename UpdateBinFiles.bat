REM - Script to prepare BEToolbox for Release

SET BINTARGET=bin

copy "Samples\*.*"                         %BINTARGET%\Examples\
copy "Redist\FORTRAN DLLs\*.*"             %BINTARGET%\FortranDLLs\
copy "Redist\Program DLLs\*.*"             %BINTARGET%\ProgramDLLs\
copy "HelpFile\BEToolbox.chm"              %BINTARGET%\App\
copy "BETCore\objsafe.tlb"                 %BINTARGET%\SharedDLLs\
copy "License.rtf"                         %BINTARGET%