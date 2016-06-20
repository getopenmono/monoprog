#!/usr/bin/env bash
source ../../configuration.sh

PACKAGE=monoprog
BUILDDIR=../../../src/$BUILDDIR
DISTDIR=${BUILDDIR}/debpackage

PKGROOT=${DISTDIR}/${PACKAGE}_${VERSION}
