@echo off

call setup.bat
msbuild unittests.vcxproj
pushd build\debug
unittests.exe -silent %*
popd
