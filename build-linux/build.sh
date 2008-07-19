#!/bin/bash

cmake ..
make clean
make
rm -R final
mkdir final
cp src/kazengine final
cp ../build/*.pk3 final

