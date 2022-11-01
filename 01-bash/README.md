This assignment is about learning how to use the usual computational environment.

# THE SUBSMISSION MUST INCLUDE BOTH THE Script AND the Assignment1.txt FILE

# Problem's Story:
The research lab is doing experiments for four weeks. They repeat the process 100 times daily and measure ten different parameters (P1-P10).
The data is stored in separate .csv files with 100 entries for each day.

# TODO

Please write a bash script to:
Read data files from the directory.
Parse each file and for each day, calculate:

a) Difference between max P1 and min P1

b) Number of non-zero entries in P2

c) Number of positive entries for P4, not including zero value

# Notice: 

The results should be written to a plain text file, called __Assignment1.txt__, where each line contains the filename, and the answers to question a-c per file.
The format should be __filename|a|b|c__ .
For example (this is not necessarily the correct answer for this problem): 20220201|191.21317263172|100|54
The data is in the file named __Data_SC.zip__ .

# Notes

1) a major difficulty is that bash cannot handle floating point values.
   Therefore at least the computation of the difference has to be done using some other program, e.g., bc(1), awk(1), ...

2) in case you use sort(1) to find the min/max, ensure you use -n/-g for numerical sort.
   If you do so, it is important that your locale(1) is set to C or EN. Use "export LC_ALL=C" at the beginning of your scirpt.
   
    
 
