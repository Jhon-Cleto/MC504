#!/usr/bin/bash

# Clean
rm -rf ./bin/rollercoaster*
make -C src clean

# Build
make -C src
mv ./src/rollercoaster ./bin