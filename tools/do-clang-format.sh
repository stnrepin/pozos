#!/bin/bash

files="$(find kern/ examples/ -iname '*.c' -o -iname '*.h')"
clang-format -i $files
