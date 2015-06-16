QT += core
QT -= gui

TARGET = monoprog

CONFIG += console
CONFIG += warn_on
CONFIG += c++11
CONFIG += resources
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp
SOURCES += src/HidDevice.cpp
SOURCES += src/Application.cpp
SOURCES += src/ProgrammerFactory.cpp
SOURCES += src/ProgrammerBase.cpp
SOURCES += src/CyacdProgrammer.cpp
SOURCES += src/ElfProgrammer.cpp
SOURCES += src/ElfReader.cpp
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

INCLUDEPATH += cybootloaderutils
INCLUDEPATH += hidapi/hidapi
INCLUDEPATH += elfio

HEADERS += cybootloaderutils/cybtldr_api.h
HEADERS += cybootloaderutils/cybtldr_api2.h
HEADERS += hidapi/hidapi/hidapi.h
HEADERS += src/OutputCollector.h
HEADERS += src/IDeviceCommunicator.h
HEADERS += src/HidDevice.h
HEADERS += src/NotConnectedMockDevice.h
HEADERS += src/Application.h
HEADERS += src/IProgrammer.h
HEADERS += src/ProgrammerFactory.h
HEADERS += src/ProgrammerBase.h
HEADERS += src/CyacdProgrammer.h
HEADERS += src/ElfProgrammer.h
HEADERS += src/ElfReader.h
HEADERS += src/IMemorySection.h
HEADERS += src/MemorySection.h
HEADERS += src/CombinedMemory.h

RESOURCES += resources.qrc

