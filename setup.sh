#!/bin/sh

qmake CONFIG+=debug -o build/Makefile monoprog.pro && \
qmake CONFIG+=debug -o build/unittest/Makefile unittests.pro
