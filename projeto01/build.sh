#!/usr/bin/bash

MATMUL_DEMO ?=

rm -rf ./bin/matmul*
make -C src clean

if [[ $MATMUL_DEMO -eq 1 ]]; then
    make -C src matmul_demo -j$THREADS
    mv ./src/matmul_demo ./bin/
else
    make -C src matmul -j$THREADS
    mv ./src/matmul ./bin/
fi
