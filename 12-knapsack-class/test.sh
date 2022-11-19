#/bin/bash
for i in instances/ksp000*dat
do
    NAME=`basename $i .dat`
    SOLVAL=`fgrep $NAME instances/solution.dat | cut -d ' ' -f 2`
    RESULT=`$1 <$i | fgrep Total | cut -d ' ' -f 2`
    if [ $SOLVAL != $RESULT ]
    then
	echo "Error" $i " was " $RESULT " should be " $SOLVAL
    else
	echo $i " ok"
    fi
done
for i in instances/ksp00[1-9]*.dat
do
    NAME=`basename $i .dat`
    SOLVAL=`fgrep $NAME instances/solution.dat | cut -d ' ' -f 2`
    RESULT=`$1 $2 $3 $4 $5 $i | fgrep Total | cut -d ' ' -f 2`
    if [ $SOLVAL != $RESULT ]
    then
	echo "Error" $i " was " $RESULT " should be " $SOLVAL
    else
	echo $i " ok"
    fi
done
