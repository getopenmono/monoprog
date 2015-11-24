@echo off

call configuration.bat
call setup.bat

msbuild monoprog.vcxproj /property:Configuration=Debug

msbuild monoprog.vcxproj /property:Configuration=Release
cucumber -f progress %*
