#!/bin/sh

BUILDDIR=../../build
PACKAGE=monoprog
EXE=monoprog
DISTDIR=${BUILDDIR}/debpackage
VERSION=0.6

PKGROOT=${DISTDIR}/${PACKAGE}_${VERSION}

get_abs_filename() {
	# $1 : relative filename
	if [ -d "$(dirname "$1")" ]; then
		echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
	else
		echo "$1"
	fi
}

if [ ! -d "${BUILDDIR}" ]; then
	echo $(get_abs_filename "${BUILDDIR}") does not exist, nothing to package.
	exit 1
fi

if [ -e "${DISTDIR}" ]; then
	rm -rf "${DISTDIR}"
fi

mkdir -p "${PKGROOT}/DEBIAN"
# Ideally we should use `dpkg --print-architecture` and generate 'control'.
cp control "${PKGROOT}/DEBIAN/"

BINDIR=${PKGROOT}/usr/bin
mkdir -p "${BINDIR}"
cp "${BUILDDIR}/$EXE" "${BINDIR}/"

sudo chown -R root:root "${PKGROOT}"

sudo dpkg-deb --build "${PKGROOT}"
