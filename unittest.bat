@echo off

call configuration.bat
call setup.bat

msbuild unittests.vcxproj /property:Configuration=Debug
pushd %DEBUGDIR%
unittests.exe -silent %*
popd

msbuild unittests.vcxproj /property:Configuration=Release
pushd %RELEASEDIR%
unittests.exe -silent %*
popd
