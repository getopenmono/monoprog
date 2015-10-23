@echo off

call configuration.bat
call setup.bat

msbuild monoprog.vcxproj
cucumber -f progress %*
