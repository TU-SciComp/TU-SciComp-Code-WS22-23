#!/bin/bash
set -x
g++ -std=c++17 -Wall -Wextra -Wpedantic -mfpmath=387 -o fpprec fpprec.cpp
