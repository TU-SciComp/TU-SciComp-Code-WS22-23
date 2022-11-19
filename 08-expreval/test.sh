#/bin/bash
$1 <test.txt 2>&1 | diff -s - test.out.ok
$1 -h
$1 "12*7+3"
