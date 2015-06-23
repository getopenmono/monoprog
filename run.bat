@echo off

call setup.bat
msbuild monoprog.vcxproj
build\debug\monoprog.exe %*
