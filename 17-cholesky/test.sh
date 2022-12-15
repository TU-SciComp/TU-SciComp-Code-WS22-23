# TK11Dec2022
./$1 10 30 1
./$1 20 40 2
./$1 30 50 3
./$1 40 60 4
./$1 data/a10.mm
./$1
./$1 gibtsnich.mm
./$1 17 6 56
for i in data/err*mm
do
    ./$1 $i
done
# Important to call from makefile for coverage
exit 0

