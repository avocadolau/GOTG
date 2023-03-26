@echo off
cd %~dp0
cd
set build_path="..\game\build"
set config_path=%build_path%\config
set mono_path=%build_path%\mono
set lib_path=%build_path%\library
set rsc_path=%build_path%\resources

if exist %build_path% rmdir /S /Q %build_path%

md %build_path%

copy "..\*.dll" %build_path%

md %config_path%

copy "..\config\application.json" %config_path%
copy "..\config\project.wiproject" %config_path%
copy "..\config\room_data.wiproject" %config_path%

md %mono_path%

xcopy /e /v /s "..\mono" %mono_path%

md %lib_path%

xcopy /e /v /s "..\library" %lib_path%

md %rsc_path%

xcopy /e /v /s "..\resources" %rsc_path%

call buildsln.bat "..\game\WiwaGame.sln" "Release"