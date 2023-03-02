cd ..
call vendor\premake\premake5.exe vs2019
cd Editor
call ..\vendor\premake\premake5.exe vs2019
cd..
cd WiwaGame
call ..\vendor\premake\premake5.exe vs2019
cd..
PAUSE