QT += core
QT += serialport
QT -= gui

TARGET = monoprog

CONFIG += console
CONFIG += warn_on
CONFIG += c++11
CONFIG += resources

TEMPLATE = app

SOURCES += main.cpp
SOURCES += HidDevice.cpp
SOURCES += Application.cpp
SOURCES += ProgrammerFactory.cpp
SOURCES += ProgrammerBase.cpp
SOURCES += CyacdProgrammer.cpp
SOURCES += ElfProgrammer.cpp
SOURCES += ElfReader.cpp
SOURCES += sleep.cpp
SOURCES += cybootloaderutils/cybtldr_api.c
SOURCES += cybootloaderutils/cybtldr_api2.c
SOURCES += cybootloaderutils/cybtldr_command.c
SOURCES += cybootloaderutils/cybtldr_parse.c

macx {
	SOURCES += hidapi/mac/hid.c
	LIBS += -framework CoreFoundation
}
linux {
	SOURCES += hidapi/linux/hid-libusb.c
	INCLUDEPATH += /usr/include/libusb-1.0
	LIBS += -lusb-1.0
}
windows {
	SOURCES += hidapi/windows/hid.c
	LIBS += setupapi.lib

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

HEADERS += cybootloaderutils/cybtldr_api.h
HEADERS += cybootloaderutils/cybtldr_api2.h
HEADERS += hidapi/hidapi/hidapi.h
HEADERS += Application.h
HEADERS += CombinedMemory.h
HEADERS += CyacdProgrammer.h
HEADERS += ElfProgrammer.h
HEADERS += ElfReader.h
HEADERS += HidDevice.h
HEADERS += IDeviceCommunicator.h
HEADERS += IMemorySection.h
HEADERS += InBootloaderMockDevice.h
HEADERS += IProgrammer.h
HEADERS += MemorySection.h
HEADERS += NotConnectedMockDevice.h
HEADERS += OutputCollector.h
HEADERS += ProgrammerBase.h
HEADERS += ProgrammerFactory.h
HEADERS += ResponsiveAppMockDevice.h
HEADERS += UnresponsiveAppMockDevice.h

INCLUDEPATH += cybootloaderutils
INCLUDEPATH += hidapi/hidapi
INCLUDEPATH += elfio

RESOURCES += resources.qrc
