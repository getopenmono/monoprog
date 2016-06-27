@echo off

call ..\..\configuration.bat

set BUILDDIR=..\..\%BUILDDIR%
set RELEASEDIR=..\..\%RELEASEDIR%

windeployqt %RELEASEDIR%/%EXE%

makensis monoprog.nsi
