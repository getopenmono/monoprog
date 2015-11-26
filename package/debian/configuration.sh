#!/usr/bin/env bash
source ../../configuration.sh

PACKAGE=monoprog
BUILDDIR=../../$BUILDDIR
DISTDIR=${BUILDDIR}/debpackage

PKGROOT=${DISTDIR}/${PACKAGE}_${VERSION}
