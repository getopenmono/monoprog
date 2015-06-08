# Mono device programmer

This is a tool for transferring applications to a [Mono device](http://openmono.com).

At the moments it works for
 - [X] OSX
 - [ ] Linux
 - [ ] Windows

To build Mono Programmer, [install Qt](https://www.qt.io/download-open-source/) and set your PATH to point to bin.

Then

    $ ./setup.sh
    $ ./compile.sh
    $ ./run.sh

To run unit tests,

    $ ./unittest.sh

To run acceptance tests, install [Aruba](https://github.com/cucumber/aruba) and

    $ ./acceptancetest.sh
