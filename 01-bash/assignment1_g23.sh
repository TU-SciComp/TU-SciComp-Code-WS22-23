# Solution of group 23
# without use of external programs
#
search_dir=Data_SC
result_file=Assignment1.txt

for filename in "$search_dir"/*.csv 
do
    echo "$filename"
    max_p1_int=$((-2**31))
    max_p1_decimal=$((0))
    min_p1_int=$((2**31))
    min_p1_decimal=$((0))
    non_zero_p2=100
    strict_positive_p4=100


    # We got the file read it into string
    while read line
    do
    #    # we got the line split it into array
        IFS=',' read -r -a array <<< "$line"
        IFS='.' read -r -a p1_split <<< "${array[0]:0:20}"
        IFS='.' read -r -a p2_split <<< "${array[1]:0:20}"
        IFS='.' read -r -a p4_split <<< "${array[3]:0:20}"

        if [[ ${p4_split[0]} == *"-"* ]]; then
            strict_positive_p4=$((${strict_positive_p4}-1))
        fi

        while [ ${#p1_split[1]} -lt 15 ];
        do 
            p1_split[1]+='0'
        done
    
        p_1_int=$((${p1_split[0]}))
        p_2_int=$((${p2_split[0]}))
        p_4_int=$((${p4_split[0]}))
        p_1_decimal=$(("1${p1_split[1]}"))
        p_2_decimal=$(("1${p2_split[1]}"))
        p_4_decimal=$(("1${p4_split[1]}"))

        if [[ $((${p_1_int})) -lt $min_p1_int || ($((${p_1_int})) -eq $min_p1_int && $((${p_1_decimal})) -gt $min_p1_decimal) ]]; then
            min_p1_int=${p_1_int}
            min_p1_decimal=${p_1_decimal}
        elif [[ $((${p_1_int})) -gt $max_p1_int || ($((${p_1_int})) -eq $max_p1_int && $((${p_1_decimal})) -gt $max_p1_decimal) ]]; then
            max_p1_int=${p_1_int}
            max_p1_decimal=${p_1_decimal}
        fi

        if [[ ${p_2_int} -eq 0 && ${p_2_decimal:0:10} -eq 0 ]]; then 
            non_zero_p2=$((${non_zero_p2}-1))
        fi

    done < <(tail -n +2 $filename)

    p_1_diff_int=$(($max_p1_int-$min_p1_int))
    p_1_diff_decimal=$(($max_p1_decimal+$min_p1_decimal))

    echo "${max_p1_int}"
    echo "${min_p1_int}"
    echo "${min_p1_decimal}"
    echo "${max_p1_decimal}"
    echo "${p_1_diff_decimal}"
    echo "${p_1_diff_decimal:0:1}"

    if [[ $p_1_diff_decimal -lt 0 ]]; then
        p_1_diff_int = $((${p_1_diff_int}-1))
    fi

    if [[ ${p_1_diff_decimal:0:1} -eq 3 ]]; then
        p_1_diff_int=$((${p_1_diff_int}+1))
    fi

    arr1=${filename##*/}
    arr2=${arr1%.csv}
    
    echo "${arr2}|${p_1_diff_int}.${p_1_diff_decimal:1:10}|${non_zero_p2}|${strict_positive_p4}" >> $result_file
done
