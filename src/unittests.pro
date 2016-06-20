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
SOURCES += ElfReader.cpp
HEADERS += unittest/autotest.h
HEADERS += unittest/test-output-collector.h
HEADERS += unittest/test-bootloader-packet.h
HEADERS += unittest/test-elf-reader.h
HEADERS += unittest/test-combined-memory.h
HEADERS += OutputCollector.h
HEADERS += ElfReader.h
HEADERS += IMemorySection.h
HEADERS += MemorySection.h
HEADERS += CombinedMemory.h

windows {
	QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
	QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
	Release:DESTDIR = $$(RELEASEDIR)
	Release:OBJECTS_DIR = $$(RELEASEDIR)/.obj
	Release:MOC_DIR = $$(RELEASEDIR)/.moc
	Release:RCC_DIR = $$(RELEASEDIR)/.rcc
	Release:UI_DIR = $$(RELEASEDIR)/.ui

	Debug:DESTDIR = $$(DEBUGDIR)
	Debug:OBJECTS_DIR = $$(DEBUGDIR)/.obj
	Debug:MOC_DIR = $$(DEBUGDIR)/.moc
	Debug:RCC_DIR = $$(DEBUGDIR)/.rcc
	Debug:UI_DIR = $$(DEBUGDIR)/.ui
}

INCLUDEPATH += .
INCLUDEPATH += cybootloaderutils
INCLUDEPATH += elfio
