#!/usr/bin/env bash
source ../../configuration.sh

APP=Monoprog-v$VERSION-x64.pkg
BUILDDIR=../../$BUILDDIR
QTDIR=../../$QTDIR/5.4/clang_64
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

if [ ! -x "$QTDIR/bin/macdeployqt" ]; then
	echo "$QTDIR/bin/macdeployqt" does not exist, cannot create package.
	exit 2
fi
"$QTDIR/bin/macdeployqt" "$BUILDDIR/$EXE.app" -no-plugins

if [ -e "$DISTDIR" ]; then
	rm -rf "$DISTDIR"
fi

mkdir -p "$DISTDIR/Applications"
cp -R "$BUILDDIR/$EXE.app" "$DISTDIR/Applications/"
mkdir -p "$DISTDIR/usr/local/bin"
ln -s "../../../Applications/$EXE.app/Contents/MacOS/$EXE" "$DISTDIR/usr/local/bin/$EXE"

if ! hash pkgbuild 2>/dev/null; then
	echo pkgbuild does not exist, cannot create package.
	exit 3
fi
pkgbuild \
	--root "$DISTDIR" \
	--identifier com.openmono.monoprog \
	--version 1 \
	--install-location / \
	"$BUILDDIR/$APP"

