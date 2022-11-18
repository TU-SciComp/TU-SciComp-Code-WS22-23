# TK13Nov2022

param filename  := "instances/instances_n10000_2.csv";
param items     := read filename as "1n" use 1;
set   I         := { 1 .. items };
param value [I] := read filename as "<1n> 2n" skip 1 use items;
param weight[I] := read filename as "<1n> 3n" skip 1 use items;
param capacity  := read filename as "1n" skip items + 1 use 1;

var x[I] binary;

maximize profit: 
   sum <i> in I : value[i] * x[i];

subto limit:
   sum <i> in I : weight[i] * x[i] <= capacity;

