@echo off

if not exist build mkdir build
pushd build

cl -DN_DEBUG=1 -FC -Fe:AutoBrightness.exe -nologo -O2 ..\src\win32_main.c -link -subsystem:windows

popd build