#!/bin/bash
thread=$(($(nproc)*2))

for file in *.o;
do
	output="out/${file}.csv"
	echo "nb_thread,elapsed_secs" > $output

	for ((nb_thread=1;nb_thread<=thread;++nb_thread))
	do
		for i in {1..5}
		do
			time=$(/usr/bin/time -f %e ./$file $i 2>&1|tail -n 1)
			
			echo "$i,$time" >> $output
			make clean -s
		done
	done
done