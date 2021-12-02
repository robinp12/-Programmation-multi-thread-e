﻿#!/bin/bash

# Appel systeme pour le nombre de thread * 2 
thread=$(nproc)

# Effectuer la tache pour chaque fichier ".o"
for file in *.o;
do
	if [ $file != "*.o" ]
	then
		# Initialiser un nouveau fichier csv
		output="out/${file}.csv"
		echo -n "" > $output

		# Boucle pour les headers
		for ((nb_thread=1;nb_thread<=thread;++nb_thread))
		do
			if [ $nb_thread == $thread ]
			then
				echo "$nb_thread," >> $output
			else
				echo -n "$nb_thread," >> $output
			fi
		done

		# Boucle pour les cinqs mesures
		for i in {1..5}
		do
			# Boucle pour chaque thread
			for ((nb_thread=1;nb_thread<=thread;++nb_thread))
			do
				# Mesure du temps d'execution
				# Philosophe prend 1 seul argument
				if [ $file != "philosophers.o" ] && [ $file != "philosophers2.o" ] 
				then
					time=$(/usr/bin/time -f %e ./$file $nb_thread*2 $nb_thread*2 2>&1|tail -n 1)
				else
					time=$(/usr/bin/time -f %e ./$file $nb_thread*2 2>&1|tail -n 1)
				fi
				# Ecrire le resultat dans le fichier
				if [ $nb_thread == $thread ]
				then
					echo "$time," >> $output
				else
					echo -n "$time," >> $output
				fi
			done
		done
	fi
done