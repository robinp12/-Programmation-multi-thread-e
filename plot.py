import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import glob

# Effectuer la tache sur chaque fichier csv
for file in glob.glob('**/*.o.csv'):
    if "2" not in file:
        # Creer les figures 
        plt.figure()
        filename = file[4:-6]

        # Ouvrir fichier tache n1
        headercsv = pd.read_csv(file)
        datacsv = pd.read_csv(file,header=None)

        # Ouvrir fichier tache n2
        datacsv2 = pd.read_csv(file[:-6]+"2"+file[-6:],header=None)

        # Initialisation des vecteurs
        header = headercsv.columns.tolist()[:-1]
        moyenne= len(header) * [0]
        ecart= len(header) * [0]

        moyenne2= len(header) * [0]
        ecart2= len(header) * [0]

        # Calcul de moyenne et ecart type
        i = 0
        for e in moyenne:
            moyenne[i] = np.mean(list(datacsv[i])[1:])
            ecart[i] = np.std(list(datacsv[i])[1:])

            moyenne2[i] = np.mean(list(datacsv2[i])[1:])
            ecart2[i] = np.std(list(datacsv2[i])[1:])
            i = i + 1

        # Tracer la courbe de la moyenne et l'ecart type
        plt.errorbar(header, moyenne, ecart, fmt='.-', capsize=4, color='blue',ecolor='grey', label=filename)
        plt.errorbar(header, moyenne2, ecart2, fmt='.-', capsize=4, color='orange',ecolor='grey', label=filename+"2")

        # Fixer a 0 l'axe Y
        plt.ylim(bottom=0)
        
        # Ajouter des titres
        plt.title("Temps d'execution moyen par thread \n pour le \"" + filename+"\"")
        plt.xlabel("Nombre de thread")
        plt.ylabel("Temps d'exécution (sec)")

        # Permet d'ajouter une grille au graphe, rendant la lecture de vos données plus facile.
        plt.grid(True)

        # Ajouter une légende, loc peut prendre différentes valeurs (https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.legend.html)
        plt.legend(loc = 'upper right')

        # on enregistre le graphique. L'extension est directement déduite du nom donné en argument (png par défault).
        plt.savefig(file+".png")

# On affiche le graphe à l'écran (note: show est un appel bloquant, tant que le graphe n'est pas fermé, on est bloqué)
#plt.show()

# On ferme proprement le plot.
plt.close()
