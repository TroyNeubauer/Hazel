@echo off
pushd %~dp0\..\..\
call Hazel\vendor\premake\bin\premake5.exe --os=windows --compiler=msc vs2019
popd
PAUSE