set -x
for i in *.cpp
do
    NAME=`basename $i .cpp`
    g++ -std=c++17 -Wall -Wextra -Wpedantic -O -o $NAME $i
done

