@echo off

call configuration.bat

call setup.bat
msbuild unittests.vcxproj
pushd %BUILDDIR%\debug
unittests.exe -silent %*
popd
