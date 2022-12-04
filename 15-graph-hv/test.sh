#!/bin/bash
DATA=/data/vorlesung/SP
for i in data/*.gph gibtesnich.gph
do
    $1 $4 $3 $4 $i 22 88
done
exit 0
