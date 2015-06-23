@echo off

call setup.bat
msbuild monoprog.vcxproj
cucumber -f progress %*
