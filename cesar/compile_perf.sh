#!/bin/bash

NB_CORES=16
NB_RUNS=5

echo "nb_cores,elapsed_secs"

for (( n=1; n<=$NB_CORES; n+=2 ))
do
	for (( i=0; i<$NB_RUNS; i++ ))
	do
		make clean -s 2>&1 > /dev/null
		# time=$(( /usr/bin/time -f %e make -s -j $n 2>&1) | cut -d\) -f 2)
		time=$(( /usr/bin/time -f %e make -s -j $n 2>&1))

		echo $n","$time
	done
done
