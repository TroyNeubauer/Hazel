@echo off
pushd %~dp0\..\..\
call vendor\premake\bin\premake5.exe --os=emscripten "--scripts=Hazel/vendor/premake/scripts" gmake2
popd
