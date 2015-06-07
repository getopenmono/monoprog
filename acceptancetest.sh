#!/bin/sh

./setup.sh && \
./compile.sh && \
cucumber -f progress $@
