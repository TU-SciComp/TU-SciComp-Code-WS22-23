#!/bin/bash
set -x
g++ -std=c++17 -Wall -mfpmath=387 -o fpprec fpprec.cpp
