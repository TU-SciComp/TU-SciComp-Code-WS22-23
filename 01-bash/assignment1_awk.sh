#/bin/bash
# TK22Oct2022
#
# a) Difference between max P1 and min P1
# b) Number of non-zero entries in P2
# c) Number of positive entries for P4, not including zero value
#
for file in Data_SC/*.csv
do
    echo -n $(basename $file .csv)
    awk -F, '/^[0-9\-\. ]+/ {if($1<p1min){p1min = $1} if($1>p1max){p1max=$1} if($2!=0.0){p2cnt++} if ($4>0.0){p4cnt++}} END {printf("|%.13f|%d|%d\n", p1max - p1min, p2cnt, p4cnt)}' $file
done >Assignment1.txt

