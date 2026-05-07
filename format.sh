#!/bin/bash
find src/ -not -path "src/math/transformations/*" -not -path "src/math/projections/projection.cc" \( -iname "*.cc" -o -iname "*.h" -o -iname "*.cpp" -o -iname "*.hpp" \) -print0 | xargs -0 -r clang-format -i
