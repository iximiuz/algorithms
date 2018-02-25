#!/bin/bash

set -e

./sbuild.sh
[ -e state.dat ] && rm state.dat
./runner simple.map R ./solution "$@"
cat state.dat

