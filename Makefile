all: clean task1 task2

clean: clean1 clean2

task1: compile1 perf1 graphs1

task2: compile2 perf2 graphs2


compile: compile1 compile2

compile1: philosophers1 producerConsumer1 readerWriter1

compile2: philosophers2 producerConsumer2 readerWriter2 test_and_set test_test_and_set


philosophers1:
		gcc -I -Wall -Werror -std=c99 task_1/philosophers.c -o out/task_1/philosophers.o -pthread
producerConsumer1:
		gcc -I -Wall -Werror -std=c99 task_1/producerConsumer.c -o out/task_1/producerConsumer.o -pthread
readerWriter1:
		gcc -I -Wall -Werror -std=c99 task_1/readerWriter.c -o out/task_1/readerWriter.o -pthread
perf1:
		./task_1/time_perf.sh out/task_1/philosophers.o && ./task_1/time_perf.sh out/task_1/readerWriter.o && ./task_1/time_perf.sh out/task_1/producerConsumer.o &&
graphs1:
		python3 task_1/plot.py
clean1:
		rm out/task_1/** && touch out/task_1/empty && rm graphs/task_1/** && touch graphs/task_1/empty


philosophers2:
		gcc -I -Wall -Werror task_2/philosophers.c -o out/task_2/philosophers.o -pthread
producerConsumer2:
		gcc -I -Wall -Werror task_2/producerConsumer.c -o out/task_2/producerConsumer.o -pthread
readerWriter2:
		gcc -I -Wall -Werror task_2/readerWriter.c -o out/task_2/readerWriter.o -pthread
test_and_set:
		gcc -I -Wall -Werror task_2/test_and_set.c -o out/task_2/test_and_set.o -pthread
test_test_and_set:
		gcc -I -Wall -Werror task_2/test_and_test_and_set.c -o out/task_2/test_and_test_and_set.o -pthread
perf2:
		./task_2/time_perf.sh out/task_2/philosophers.o && ./task_2/time_perf.sh out/task_2/readerWriter.o && ./task_2/time_perf.sh out/task_2/producerConsumer.o && ./task_2/time_perf.sh out/task_2/test_and_set.o && ./task_2/time_perf.sh out/task_2/test_and_test_and_set.o
graphs2:
		python3 task_2/plot.py && python3 task_2/plot_locks.py 
clean2:
		rm out/task_2/** && touch out/task_2/empty && rm graphs/task_2/** && touch graphs/task_2/empty
