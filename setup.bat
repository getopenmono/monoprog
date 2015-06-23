@echo off

qmake -spec win32-msvc2010 -tp vc unittests.pro
qmake -spec win32-msvc2010 -tp vc monoprog.pro
