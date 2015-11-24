#!/usr/bin/env bash
source configuration.sh

qmake CONFIG+=debug -o "$BUILDDIR"/Makefile monoprog.pro && \
qmake CONFIG+=debug -o "$BUILDDIR"/unittest/Makefile unittests.pro
