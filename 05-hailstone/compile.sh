#!/bin/bash
# TK27Oct2022
#
set -x
for i in hailstone1 hailstone2 hailstone3 power2
do
    g++ -std=c++17 -Wall -Wextra -Wpedantic -O -o $i $i.cpp
done
g++ -std=c++17 -Wall -Wextra -Wpedantic -O -s -static -o ex02_autograde ex02_autograde.cpp
