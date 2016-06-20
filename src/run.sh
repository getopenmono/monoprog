#!/usr/bin/env bash
source configuration.sh

platform=$(uname)
if [[ "$platform" == "Darwin" ]]; then
	EXEPATH="$BUILDDIR/$EXE.app/Contents/MacOS/$EXE"
else
	EXEPATH="$BUILDDIR/$EXE"
fi

./compile.sh && \
"$EXEPATH" $@
