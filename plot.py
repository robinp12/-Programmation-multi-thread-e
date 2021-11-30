import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import glob

for file in glob.glob('**/*.o.csv'):
    plt.figure()
    headercsv = pd.read_csv(file)
    datacsv = pd.read_csv(file,header=None)
    header = headercsv.columns.tolist()[:-1]
    moyenne= len(header) * [0]
    ecart= len(header) * [0]

    i = 0
    for e in moyenne:
        moyenne[i] = np.mean(list(datacsv[i])[1:])
        ecart[i] = np.std(list(datacsv[i])[1:])
        i = i + 1

    plt.errorbar(header, moyenne, ecart, fmt='.-', capsize=5, ecolor='black', label="Philosophes tâche 1")

    plt.ylim(bottom=0)
    plt.title("Temps moyen de compilation : " + file[4:-6])
    plt.xlabel("Nombre de thread")
    plt.ylabel("Temps d'exécution moyen")

    # Permet d'ajouter une grille au graphe, rendant la lecture de vos données plus facile.
    plt.grid(True)

    # Ajouter une légende, loc peut prendre différentes valeurs (https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.legend.html)
    plt.legend(['Temps d\'execution par thread'], loc = 'upper right')

    # on enregistre le graphique. L'extension est directement déduite du nom donné en argument (png par défault).
    plt.savefig(file+".png")

    #  on affiche le graphe à l'écran (note: show est un appel bloquant, tant que le graphe n'est pas fermé, on est bloqué)
plt.show()

    # On ferme proprement le plot.
plt.close()
