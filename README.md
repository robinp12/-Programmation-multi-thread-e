# Programmation multi-thread

The project requires having a version of Python 3 installed, along with the Matplotlib, Numpy and Pandas libraries.

### Run project (linux)

`make all`

#### Compile (file).c

`gcc task_1/(file).c -Wall -Werror -pthread -o (file).o`

#### Run (file).c

`./(file).o <nb>`

#### Make performance

`bash .\time_perf.sh`

#### Make graphs

`python plot.py`