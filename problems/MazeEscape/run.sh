#!/bin/bash

set -e

./sbuild.sh
[ -e state.dat1 ] && rm state.dat1
./runner complex.map L ./solution "$@"
cat state.dat1

