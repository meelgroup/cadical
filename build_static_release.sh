#!/bin/bash
set -e
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="$(pwd)/build"
cmake --build build -j$(nproc)
cmake --install build
