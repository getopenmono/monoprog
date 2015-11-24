#!/usr/bin/env bash
source ../../configuration.sh

PACKAGE=monoprog
BUILDDIR=../../$BUILDDIR
DISTDIR=${BUILDDIR}/debpackage

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

UDEVDIR=${PKGROOT}/etc/udev/rules.d
mkdir -p "${UDEVDIR}"
cp etc-udev-rules.d-openmono.rules "${UDEVDIR}/openmono.rules"

sudo chown -R root:root "${PKGROOT}"

sudo dpkg-deb --build "${PKGROOT}"
