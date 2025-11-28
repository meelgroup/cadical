#!/usr/bin/env bash
rm -rf build/libcadical.*
rm -f compile_commands.json
make clean
CXXFLAGS=-fPIC ./configure --no-contracts --no-tracing --debug -ggdb3
bear -- make -j12
