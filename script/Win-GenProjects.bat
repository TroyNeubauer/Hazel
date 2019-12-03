@echo off
pushd %~dp0\..\
call vendor\bin\premake\premake5.exe --compiler=msc vs2017
popd
PAUSE