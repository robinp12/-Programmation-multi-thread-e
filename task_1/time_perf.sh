#!/bin/bash

# Appel systeme pour le nombre de coeurs
thread=$(nproc)

# Effectuer la tache pour chaque fichier ".o"
for file in out/task_1/*.o;
do
	if [ $file != "*.o" ]
	then
		# Initialiser un nouveau fichier csv
		output="${file}.csv"
		touch $output
		
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

		# Boucle pour les cinq mesures
		for i in {1..5}
		do
			# Boucle pour chaque thread
			for ((nb_thread=1;nb_thread<=thread;++nb_thread))
			do
				# Mesure du temps d'execution pour les differents fichiers
				# Programme "Philosophes" prend 1 seul argument
				if [ $file != "philosophers.o" ]
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
