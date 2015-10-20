#!/usr/bin/env bash
source configuration.sh

./compile.sh && \
"$BUILDDIR"/monoprog $@
