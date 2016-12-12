[![Build Status](https://travis-ci.org/getopenmono/monoprog.svg)](https://travis-ci.org/getopenmono/monoprog)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/getopenmono/monoprog?branch=master&svg=true "AppVeyor Build Status")](https://ci.appveyor.com/project/stoffera/monoprog)

# Mono device programmer

This is a tool for transferring applications to a [Mono device](http://openmono.com).

At the moments it works for OS X, Debian/Ubuntu GNU/Linux, and Windows.

To build Mono Programmer, you need Qt and various libraries, see architecture-specific sections below, but in general, you need to go the `src` directory and run the following scripts

- `setup`
- `compile`
- `run`

To run unit tests,

- `unittest`

To run acceptance tests, install [Aruba](https://github.com/cucumber/aruba) and

- `acceptancetest`

## OS X

[Install Qt](https://www.qt.io/download-open-source/) and set your PATH to point to the installed Qt bin directory, like adding `export PATH="~/Qt/5.4/clang_64/bin:$PATH"` to your ~/.bachrc.  Then go to `src` and run

    $ ./setup.sh
    $ ./compile.sh
    $ ./run.sh

To run unit tests,

    $ ./unittest.sh

To run acceptance tests, install [Aruba](https://github.com/cucumber/aruba) by `$ gem install aruba` and

    $ ./acceptancetest.sh

### XCode 7

If you upgrade to XCode 7, you need to edit `Qt/5.3/clang_64/mkspecs/qdevice.pri` to
contain the line

     QMAKE_MAC_SDK = macosx10.11


## Debian/Ubuntu

Install the packages listed in `provision/development-setup-as-root`.  Then go to `src` and run

    $ ./setup.sh
    $ ./compile.sh
    $ ./run.sh

To run unit tests,

    $ ./unittest.sh

To run acceptance tests, run

    $ ./acceptancetest.sh


## Windows

Monoprog has been tested with Visual Studio Express 2010.  It might work in other configurations, please report back.

- Install [Visual Studio C++ Express 2010](https://app.vssps.visualstudio.com/profile/review?download=true&family=VisualStudioCExpress&release=VisualStudio2010).

- Install [Qt for VS2010](http://download.qt.io/official_releases/qt/5.5/5.5.0/qt-opensource-windows-x86-msvc2010-5.5.0.exe).

- Add Qt (eg. C:\Qt\Qt5.5.0\5.5\msvc2010\bin) and msbuild (eg. C:\Windows\Microsoft.NET\Framework\v4.0.30319) to your PATH.

Run

    C:/> setup.bat
    C:/> unittest.bat
    C:/> run.bat

- Optionally install Ruby (2.0 for X86): http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-2.0.0-p645.exe, add bin to PATH.  Aruba: gem install aruba.  Optionally, http://adoxa.altervista.org/ansicon.

Run

    C:/> acceptancetest.bat


## Debugging ELF files

Run

    $ g++ -I. elfio/elfdump.cpp -o bin/elfdump

to get a binary `elfdump` that you can use to inspet ELF files.


## Releases

To create an installer package, see [RELEASE.md](RELEASE.md).
