# Releases

## MacOS

Use a physical machine:

To build an installer, run

    $ ./compile.sh
    $ cd package/osx
    $ ./package.sh

which places a package in `../bin/Monoprog-vX.Y.Z-x64.pkg`.

## Debian/Ubuntu

Build the installer in a virtual machine by using [Vagrant](http://vagrantup.com):

    $ vagrant up debian
    $ vagrant ssh
    $ cd src
    $ ./compile.sh
    $ cd package/debian
    $ ./makedeb.sh

which places a package in `../bin/debpackage/monoprog_X.Y.Z.deb`.


## Windows

{TODO: Vagrant setup}

To build an installer, install [NSIS](http://nsis.sourceforge.net/Download/),
edit `configuration.bat`, and run

    C:/> compile.bat
    C:/> cd package\windows
    C:/> package.bat

which places a package in `bin/MonoprogSetup-vX.Y.Z-X86.exe`.
