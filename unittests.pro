QT += core
QT += testlib
QT -= gui

CONFIG += console
CONFIG += warn_on
CONFIG += testcase
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += unittest/test-main.cpp
SOURCES += src/ElfReader.cpp
HEADERS += unittest/autotest.h
HEADERS += unittest/test-dummy.h
HEADERS += unittest/test-output-collector.h
HEADERS += unittest/test-bootloader-packet.h
HEADERS += unittest/test-elf-reader.h
HEADERS += unittest/test-combined-memory.h
HEADERS += src/OutputCollector.h
HEADERS += src/ElfReader.h
HEADERS += src/IMemorySection.h
HEADERS += src/MemorySection.h
HEADERS += src/CombinedMemory.h

windows {
	Release:DESTDIR = build/release
	Release:OBJECTS_DIR = build/release/.obj
	Release:MOC_DIR = build/release/.moc
	Release:RCC_DIR = build/release/.rcc
	Release:UI_DIR = build/release/.ui
	
	Debug:DESTDIR = build/debug
	Debug:OBJECTS_DIR = build/debug/.obj
	Debug:MOC_DIR = build/debug/.moc
	Debug:RCC_DIR = build/debug/.rcc
	Debug:UI_DIR = build/debug/.ui
}

INCLUDEPATH += src
INCLUDEPATH += cybootloaderutils
INCLUDEPATH += elfio
