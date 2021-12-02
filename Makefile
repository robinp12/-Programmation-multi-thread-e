all: philosophers producerConsumer readerWriter philosophers2 producerConsumer2 readerWriter2 performances graphs

philosophers:
		gcc -I -Wall -Werror -std=c99 task_1/philosophers.c -o philosophers.o -pthread
producerConsumer:
		gcc -I -Wall -Werror -std=c99 task_1/producerConsumer.c -o producerConsumer.o -pthread
readerWriter:
		gcc -I -Wall -Werror -std=c99 task_1/readerWriter.c -o readerWriter.o -pthread
philosophers2:
		gcc -I -Wall -Werror -std=c99 task_2/philosophers2.c -o philosophers2.o -pthread
producerConsumer2:
		gcc -I -Wall -Werror -std=c99 task_2/producerConsumer2.c -o producerConsumer2.o -pthread
readerWriter2:
		gcc -I -Wall -Werror -std=c99 task_2/readerWriter2.c -o readerWriter2.o -pthread
performances:
		bash time_perf.sh
graphs:
		python3 plot.py
clean:
		rm out/*
		rm *.o