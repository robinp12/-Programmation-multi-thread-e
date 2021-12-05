# Programmation multi-thread

The project requires having a version of Python 3 installed, along with the Matplotlib, Numpy and Pandas libraries.

### Run project (on linux)

`make`

### Run the first part of the project

`make task1`

### Run the second part of the project

`make task2`

#### Compile <file>.c

`gcc -I -Wall -Werror -std=c99 <file>.c -o <file>.o -pthread`

#### Run <file>.c

`./<file>.o <nb>`

#### Make performance

`bash ./time_perf.sh`

#### Make graphs

`python task_<1 or 2>/plot.py`

#### Clean output files

`make clean`

## Philosophes
![Philosophes](graphs/task_2/philosophers.o.csv.png?raw=true "Philosophes")

## Producteurs/Consommateurs
![Producteurs/Consommateurs](graphs/task_2/producerConsumer.o.csv.png?raw=true "Producteurs/Consommateurs")

## Lecteurs/Ecrivains
![Lecteurs/Ecrivains](graphs/task_2/readerWriter.o.csv.png?raw=true "Lecteurs/Ecrivains")

## Test & Set
![Test & Set](graphs/task_2/test_and_set.o.csv.png?raw=true "Test & Set")

