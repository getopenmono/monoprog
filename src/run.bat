@echo off

call configuration.bat

call setup.bat
msbuild monoprog.vcxproj
%BUILDDIR%\debug\monoprog.exe %*
