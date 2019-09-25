#!/usr/bin/env bash

find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i
find test -name "*.cpp" -o -name "*.h" | xargs clang-format -i
