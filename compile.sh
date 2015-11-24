#!/usr/bin/env bash
source configuration.sh

./setup.sh && \
cd "$BUILDDIR" && \
make
