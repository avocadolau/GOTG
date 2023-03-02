cd ..
call vendor\premake\premake5.exe vs2019
cd Editor
call ..\vendor\premake\premake5.exe vs2019
cd game
call ..\..\vendor\premake\premake5.exe vs2019
cd..
cd..
PAUSE