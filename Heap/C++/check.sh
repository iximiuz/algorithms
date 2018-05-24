#!/bin/bash

g++ -std=c++11 listmedian.cpp
diff <(./a.out < input01.txt) output01.txt && echo "ok"

