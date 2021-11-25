import matplotlib.pyplot as plt
import csv

def make_figure(file):
    data = {}
    fig1 = plt.figure()
    with open('out/'+file+'.csv') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                line_count += 1
            else:
                nb_cores = int(row[0])
                time = float(row[1])
                line_count += 1
        
                l = data.get(nb_cores, [])
                l.append(time)
                data[nb_cores] = l

    x = list(data.keys())
    y = [sum(v) / len(v) for v in data.values()]

    # On trace la température moyenne en fonction du mois en bleu avec un trait plein de 1 pixel d'épaisseur
    plt.plot(x, y, color="grey", linewidth=1.0, linestyle="-")
    plt.ylim(ymin=0)
    plt.title("Temps moyen de compilation")
    plt.xlabel("Nombre de thread")
    plt.ylabel("Temps d'exécution moyen")

    # Permet d'ajouter une grille au graphe, rendant la lecture de vos données plus facile.
    plt.grid(True)

    # Ajouter une légende, loc peut prendre différentes valeurs (https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.legend.html)
    plt.legend(['Temps d\'execution par thread'], loc = 'upper right')

    # on enregistre le graphique. L'extension est directement déduite du nom donné en argument (png par défault).
    plt.savefig("graphs/"+file+".png")
    plt.savefig("graphs/"+file+".pdf")

    #  on affiche le graphe à l'écran (note: show est un appel bloquant, tant que le graphe n'est pas fermé, on est bloqué)
    plt.show()

    # On ferme proprement le plot.
    plt.close()

file = "philosopheProblem.o"
try:
    make_figure("philosopheProblem.o")
except:
    print("No such file")