all: clean philosophers

philosophers:
		gcc -I -Wall -Werror -std=c99 task_1/philosophers.c -o philosophers.o -pthread

clean:
		#rm out/*