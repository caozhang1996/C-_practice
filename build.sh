# !/usr/bin/bash

if [ -d build ]; then
    rm -rf build
fi

mkdir build
cd build


cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=../install \
      ..
cmake --build .

make install