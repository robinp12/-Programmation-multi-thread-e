#!/bin/bash
 
# Le premier argument du script correspond au nom du fichier à exécuter
file=$1
# Appel systeme pour le nombre de coeurs
max_threads=$(nproc)

# Initialiser un nouveau fichier csv
output="${file}.csv"
echo -n "" > $output
echo "writing time result in $file.csv"

# Boucle pour les headers
for ((nb_thread=1;nb_thread<=max_threads;++nb_thread))
do			
	if [ $nb_thread == $max_threads ]
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
	for ((nb_thread=1;nb_thread<=max_threads;++nb_thread))
	do
		# Mesure du temps d'execution pour les differents fichiers
		if [ "$file" == "out/task_2/philosophers.o" ] || [ "$file" == "out/task_2/test_and_set.o" ]  || [ "$file" == "out/task_2/test_and_test_and_set.o" ]
		then
			time=$(/usr/bin/time -f %e ./$file $nb_thread*2 2>&1|tail -n 1)
		else
			time=$(/usr/bin/time -f %e ./$file $nb_thread*2 $nb_thread*2 2>&1|tail -n 1)
		fi
				
		# Ecrire le resultat dans le fichier
		if [ $nb_thread == $max_threads ]
		then
			echo "$time," >> $output
		else
			echo -n "$time," >> $output
		fi
	done
done
