all: clean philosophers

philosophers:
		gcc -I -Wall -Werror -std=c99 src/philosophers.c -o out/philosophers.exe -pthread

clean:
		rm out/*