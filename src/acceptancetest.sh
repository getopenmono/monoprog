#!/usr/bin/env bash
source configuration.sh

./setup.sh && \
./compile.sh && \
cucumber -f progress $@
