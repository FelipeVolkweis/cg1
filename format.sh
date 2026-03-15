#!/bin/bash
find src/ -not -path "src/transformations/*" \( -iname "*.cc" -o -iname "*.h" -o -iname "*.cpp" -o -iname "*.hpp" \) -print0 | xargs -0 -r clang-format -i
