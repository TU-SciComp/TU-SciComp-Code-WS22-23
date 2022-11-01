#/bin/bash
#
# a) Difference between max P1 and min P1
# b) Number of non-zero entries in P2
# c) Number of positive entries for P4, not including zero value
#
# Locale can interfere with sort -n/-g if set to German due to Dezimalkomma
export LC_ALL=C
for file in Data_SC/*.csv
do
	>p1.dat
	>p2.dat
	>p4.dat
	# tail to skip over the first line
	tail -n +2 $file |
	while IFS="," read -r p1 p2 p3 p4 #p5 p6 p7 p8 p9 p0
	do
	    echo $p1 >> p1.dat
	    
	    if [ "$p2" != "0.0" ] # Will only work if written as 0.0
	    then
		echo $p2 >> p2.dat
	    fi
	    if [[ "$p4" != -* ]]
	    then
		echo $p4 >> p4.dat
	    fi
	done 
	
	p1min=`sort -n p1.dat | head -n 1`
	p1max=`sort -n p1.dat | tail -n 1`
	p1dif=`bc <<< "$p1max - $p1min"`
	p2cnt=`cat p2.dat | wc -l`
	p4cnt=`cat p4.dat | wc -l`
	
	echo $(basename $file .csv)"|"$p1dif"|"$p2cnt"|"$p4cnt 

done > Assignment1.txt
# Clean up
rm p[124].dat
