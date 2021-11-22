#!/bin/bash

COMMANDS=("philosophers.exe")
NB_CORES=16
NB_RUNS=10

NB_PHILOSOPHERS=10

make clean -s 2>&1 > /dev/null
make -s all 2>&1 > /dev/null

for cmd in ${COMMANDS[@]};
do
	output="out/${cmd}.csv"
	echo "nb_cores,elapsed_secs" > $output

	for (( n=1; n<=$NB_CORES; n+=2 ))
	do
		for (( i=0; i<$NB_RUNS; i++ ))
		do
			case "$cmd" in
			"philosophers.exe")
				time=$(( /usr/bin/time -f %e ./$cmd $NB_PHILOSOPHERS ))
			    ;;
			*)
			    ;;
			esac

			echo "$n,$time" >> $output
		done
	done
done
