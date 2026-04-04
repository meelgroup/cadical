#!/bin/bash
set -e
cmake -S . -B build-static -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="$(pwd)/build"
cmake --build build-static -j$(nproc)
cmake --install build-static
