#!/bin/bash

cmake ..
make clean
make
mkdir final
cp src/kazengine final
cp ../build/*.pk3 final

