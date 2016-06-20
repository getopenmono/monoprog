@echo off

call configuration.bat

rmdir /s /q "%BUILDDIR%"
del /q *.vcxproj *.vcxproj.filters *.sdf *.opensdf
rmdir /s /q ipch