@echo off
pushd %~dp0\..\..\
call Hazel\vendor\premake\bin\premake5.exe --os=emscripten "--scripts=Hazel/vendor/premake/scripts" gmake2
popd
PAUSE