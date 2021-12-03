all: task1 task2

task1: compile1 perf1 graphs1

task2: compile2 perf2 graphs2


compile1: philosophers1 producerConsumer1 readerWriter1

compile2: philosophers2 producerConsumer2 readerWriter2 test_and_set test_test_and_set


clean: clean1 clean2


philosophers1:
		gcc -I -Wall -Werror -std=c99 task_1/philosophers.c -o out/task_1/philosophers.o -pthread
producerConsumer1:
		gcc -I -Wall -Werror -std=c99 task_1/producerConsumer.c -o out/task_1/producerConsumer.o -pthread
readerWriter1:
		gcc -I -Wall -Werror -std=c99 task_1/readerWriter.c -o out/task_1/readerWriter.o -pthread
perf1:
		bash task_1/time_perf.sh
graphs1:
		python3 task_1/plot.py
clean1:
		rm out/task_1/** && touch out/task_1/empty


philosophers2:
		gcc -I -Wall -Werror -std=c99 task_2/philosophers.c -o out/task_2/philosophers.o -pthread
producerConsumer2:
		gcc -I -Wall -Werror -std=c99 task_2/producerConsumer.c -o out/task_2/producerConsumer.o -pthread
readerWriter2:
		gcc -I -Wall -Werror -std=c99 task_2/readerWriter.c -o out/task_2/readerWriter.o -pthread
test_and_set:
		gcc -I -Wall -Werror task_2/test_and_set.c -o out/task_2/test_and_set.o -pthread
test_test_and_set:
		gcc -I -Wall -Werror task_2/test_and_test_and_set.c -o out/task_2/test_and_test_and_set.o -pthread
perf2:
		bash task_2/time_perf.sh
graphs2:
		python3 task_2/plot.py
clean2:
		rm out/task_2/** && touch out/task_2/empty
