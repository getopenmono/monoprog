#!/bin/sh

APP=Monoprog
EXE=monoprog
BUILDDIR=../../build
DISTDIR=$BUILDDIR/dist

get_abs_filename() {
	# $1 : relative filename
	if [ -d "$(dirname "$1")" ]; then
		echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
	else
		echo "$1"
	fi
}

if [ ! -d "$BUILDDIR" ]; then
	echo $(get_abs_filename "$BUILDDIR") does not exist, nothing to package.
	exit 1
fi

if [ -e "$DISTDIR" ]; then
	rm -rf "$DISTDIR"
fi
mkdir -p "$DISTDIR/bin"
cp "$BUILDDIR/$EXE" "$DISTDIR/bin/"

pkgbuild \
	--root "$DISTDIR" \
	--identifier com.openmono.monoprog \
	--version 1 \
	--install-location /usr/local \
	"$BUILDDIR/$APP.pkg"
