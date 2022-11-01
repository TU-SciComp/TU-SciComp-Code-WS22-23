#!/bin/bash
# TK27Oct2022
#
# from time(1)
# M      Maximum resident set size of the process during its lifetime, in Kilobytes.
# U      Total number of CPU-seconds that the process used directly (in user mode), in seconds.
# e      Elapsed real (wall clock) time used by the process, in seconds.
#
export TIME="time %C: %M %U %e"
echo "time                   Mem  User Real"

for i in 1 2 3 4 5 6 7 8 9 10
do
    /usr/bin/time ./fibonacci $i 45
done
