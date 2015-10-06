#!/bin/sh

APP=Monoprog
BUILDDIR=../../build
PACKAGEMAKER=/Applications/PackageMaker.app/Contents/MacOS/PackageMaker

if [ -x "$PACKAGEMAKER" ]; then
	"$PACKAGEMAKER" \
		--doc "$APP.pmdoc" \
		--version 1.1 \
		--title "Mono Programmer" \
		--out "$BUILDDIR/$APP.pkg"
	exit 0
else
	echo $PACKAGEMAKER not found
	exit 1
fi

ROOTDIR=$BUILDDIR/$APP.app

rm -rf "$ROOTDIR/"
mkdir -p "$ROOTDIR/Contents/MacOS"
mkdir -p "$ROOTDIR/Contents/Resources"
cp "$BUILDDIR/monoprog" "$ROOTDIR/Contents/MacOS"
cp ./Info.plist "$ROOTDIR/Contents"

pkgbuild \
	--root "$ROOTDIR" \
	--component-plist monoprog.plist \
	--identifier com.openmono.pkg.monoprog \
	--version 0.6.0 \
	--install-location /usr/local/bin \
	"$BUILDDIR/$APP.pkg"
