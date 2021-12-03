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
