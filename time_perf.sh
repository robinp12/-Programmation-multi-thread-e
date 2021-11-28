#!/bin/bash
thread=$(($(nproc)*2))

for file in *.o;
do
	output="out/${file}.csv"
	echo -n "Nbr mesure\threads" > $output
	for ((nb_thread=1;nb_thread<=thread;++nb_thread))
	do
		if [ $nb_thread == $thread ]
		then
			echo ",$nb_thread" >> $output
		else
			echo -n ",$nb_thread" >> $output
		fi
	done

	for i in {1..5}
	do
		echo -n "$i, " >> $output
		for ((nb_thread=1;nb_thread<=thread;++nb_thread))
		do
			if [ $file == "readerWriter.o" ]
			then
				time=$(/usr/bin/time -f %e ./$file $i $i 2>&1|tail -n 1)

			else
				time=$(/usr/bin/time -f %e ./$file $i 2>&1|tail -n 1)
			fi
			if [ $nb_thread == $thread ]
			then
				echo "$time," >> $output
			else
				echo -n "$time," >> $output
			fi
			make clean -s
		done
	done
done