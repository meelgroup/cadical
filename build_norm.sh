#!/usr/bin/env bash
rm -rf build/libcadical.*
rm -f compile_commands.json
make clean
CXXFLAGS=-fPIC ./configure --no-contracts --no-tracing --fp
bear -- make -j12
