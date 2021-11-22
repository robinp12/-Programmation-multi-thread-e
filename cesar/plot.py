import matplotlib as plt


data = {}

for line in lines:
    (nb_cores, time) = line.split(",")
    nb_cores = int(nb_cores)
    time = float(time)
    
    l = data.get(nb_cores, [])
    l.append(time)
    data[nb_cores] = l

x = list(data.keys())
y = [sum(v) / len(v) for v in data.values()]

print(x)
print(y)

plt.plot(x, y)

plt.title("Temps moyen de compilation")
plt.xlabel("Nombre de coeurs")
plt.ylabel("Temps d'exécution moyen")

plt.show()
plt.close()
