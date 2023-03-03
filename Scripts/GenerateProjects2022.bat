cd ..
call vendor\premake\premake5.exe vs2022
cd Editor
call ..\vendor\premake\premake5.exe vs2022
cd game
call ..\..\vendor\premake\premake5.exe vs2022
cd..
cd..
PAUSE