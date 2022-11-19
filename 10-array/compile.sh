for i in "-O0" "-O1" "-O2" "-O3" "-Ofast"
do
    g++ -std=c++17 -Wall -Wextra -Wpedantic -O -o array array.cpp
    ./array
done
