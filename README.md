# Mono device programmer

This is a tool for transferring applications to a [Mono device](http://openmono.com).

At the moments it works for
 - [X] OSX
 - [ ] Linux
 - [ ] Windows

To build Mono Programmer, you need Qt.

Then

    $ ./setup.sh
    $ ./compile.sh
    $ ./run.sh

To run unit tests,

    $ ./unittest.sh

To run acceptance tests, install [Aruba](https://github.com/cucumber/aruba) and

    $ ./acceptancetest.sh

## OSX

 - [install Qt](https://www.qt.io/download-open-source/) and set your PATH to point to bin.

## Debian-based Linux

 - Install Qt from http://www.qt.io/download-open-source, see http://www.bogotobogo.com/Qt/Qt5_Install_Ubuntu_14_64bit.php and set your PATH to point to installed bin directory, like
	export PATH=~/Qt5.4.2/5.4/gcc_64/bin:$PATH

 - Install libusb-1.0-0-dev

 - Install Aruba: `$ sudo aptitude install ruby-aruba`

