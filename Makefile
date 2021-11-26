all: clean philosophers

philosophers:
		gcc -I -Wall -Werror -std=c99 task_1/philosophers.c -o philosophers.exe -pthread

clean:
		rm out/*