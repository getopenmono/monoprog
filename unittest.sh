#!/bin/sh

./setup.sh
cd build/unittest
make && ./unittests -silent $@
