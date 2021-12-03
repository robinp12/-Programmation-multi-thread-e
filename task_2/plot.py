import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Effectuer la tache sur chaque fichier csv
files = ['out/task_2/philosophers.o.csv', 'out/task_2/producerConsumer.o.csv', 'out/task_2/readerWriter.o.csv']
for file in files:
    plt.figure()

    # Ouvrir fichier tache 1
    headercsv = pd.read_csv(file)
    datacsv = pd.read_csv(file.replace('task_2', 'task_1'), header=None)
    
    # Ouvrir fichier tache 2
    datacsv2 = pd.read_csv(file, header=None)

    # Initialisation des vecteurs
    header = headercsv.columns.tolist()[:-1]
    moyenne = len(header) * [0]
    ecart = len(header) * [0]

    moyenne2 = len(header) * [0]
    ecart2 = len(header) * [0]

    # Calcul de moyenne et ecart type
    i = 0
    for e in moyenne:
        moyenne[i] = np.mean(list(datacsv[i])[1:])
        ecart[i] = np.std(list(datacsv[i])[1:])

        moyenne2[i] = np.mean(list(datacsv2[i])[1:])
        ecart2[i] = np.std(list(datacsv2[i])[1:])

        i = i + 1

    program_name = ''
    if 'philosophers' in file:
        program_name = '(Philosophes)'
    elif 'producer' in file:
        program_name = '(Producteurs/Consommateurs)'
    elif 'reader' in file:
        program_name = '(Lecteurs/Ecrivains)'

    # Tracer la courbe avec la moyenne et l'ecart type
    plt.errorbar(
        header, moyenne, ecart, fmt='.-', capsize=4, color='blue', ecolor='grey', label=f'{program_name} (implémentation POSIX)'
    )
    plt.errorbar(
        header, moyenne2, ecart2, fmt='.-', capsize=4, color='orange', ecolor='grey', label=f'{program_name} (notre implémentation)'
    )

    # Fixer a 0 l'axe Y
    plt.ylim(bottom=0)
    print("write graphs for "+ program_name)

    # Ajouter des titres
    plt.title(f'Temps d\'exécution moyen\nen fonction du nombre de threads {program_name}')
    plt.xlabel('Nombre de threads')
    plt.ylabel('Temps d\'exécution moyen (sec)')

    # Permet d'ajouter une grille au graphe, rendant la lecture des données plus facile.
    plt.grid(True)

    # Ajouter une légende, loc peut prendre différentes valeurs (https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.legend.html)
    plt.legend(['implémentation POSIX', 'notre implémentation'], loc = 'upper right')

    # on enregistre le graphique. L'extension est directement déduite du nom donné en argument (png par défault).
    plt.savefig(f'{file.replace("out/task_2/", "")}.png')

# On ferme proprement le plot.
plt.close()
