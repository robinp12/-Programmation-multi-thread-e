all: philosophers producerConsumer readerWriter performances graphs

philosophers:
		gcc -I -Wall -Werror -std=c99 task_1/philosophers.c -o philosophers.o -pthread
producerConsumer:
		gcc -I -Wall -Werror -std=c99 task_1/producerConsumer.c -o producerConsumer.o -pthread
readerWriter:
		gcc -I -Wall -Werror -std=c99 task_1/readerWriter.c -o readerWriter.o -pthread
performances:
		bash time_perf.sh
graphs:
		python plot.py
clean:
		#rm out/*