#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_READS 2560
#define MAX_WRITES 640


void *read(void *arg);
void *write(void *arg);
void read_database(void);
void write_database(void);


pthread_t *reader_threads;
pthread_t *writer_threads;
pthread_mutex_t z;
pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;
sem_t db;
sem_t rsem;
int nb_reader_threads;
int nb_writer_threads;
int nb_reads = 0;
int nb_writes = 0;


sem_init(&db, NULL, 1);
sem_init(&rsem, NULL, 1);


int main(int argc, char *argv[]) {
	if (argc != 3
		|| (nb_reader_threads = atoi(argv[1])) == 0
		|| (nb_writer_threads = atoi(argv[2])) == 0) {
		printf("ERROR, this program expects two integer arguments");
		return EXIT_FAILURE;
	}

	int *reader_ids = (int *) malloc(nb_reader_threads * sizeof(int));
	int *writer_ids = (int *) malloc(nb_writer_threads * sizeof(int));
	reader_threads = (pthread_t *) malloc(nb_reader_threads * sizeof(pthread_t));
	writer_threads = (pthread_t *) malloc(nb_writer_threads * sizeof(pthread_t));

	for (int i = 0; i < nb_reader_threads; i++) {
		reader_ids[i] = i;
	}

	for (int i = 0; i < nb_writer_threads; i++) {
		writer_ids[i] = i;
	}

	for (int i = 0; i < nb_reader_threads; i++) {
		pthread_create(&reader_threads[i], NULL, read, (void *) &reader_ids[i]);
	}

	for (int i = 0; i < nb_writer_threads; i++) {
		pthread_create(&writer_threads[i], NULL, write, (void *) &writer_ids[i]);
	}

	for (int i = 0; i < nb_reader_threads; i++) {
		pthread_join(reader_threads[i], NULL);
	}

	for (int i = 0; i < nb_writer_threads; i++) {
		pthread_join(writer_threads[i], NULL);
	}

	return EXIT_SUCCESS;
}

void *read(void *arg) {
	int *id = (int *) arg;

	for (int i = 0; i < MAX_READS; i++) {
		pthread_mutex_lock(&z);
		sem_wait(&rsem);
		pthread_mutex_lock(&read_mutex);
		nb_reads++;
		if (nb_reads == 1) {
			sem_wait(&db);
		}
		pthread_mutex_unlock(&read_mutex);
		sem_post(&rsem);
		pthread_mutex_unlock(&z);

		read_database();

		pthread_mutex_lock(&read_mutex);
		nb_reads--;
		if (nb_reads == 0) {
			sem_post(&db);
		}
		pthread_mutex_unlock(&read_mutex);
	}

	return NULL;
}

void *write(void *arg) {
	int *id = (int *) arg;

	for (int i = 0; i < MAX_WRITES; i++) {
		pthread_mutex_lock(&write_mutex);
		nb_writes++;
		if (nb_writes == 1) {
			sem_wait(&rsem);
		}
		pthread_mutex_unlock(&write_mutex);
		sem_wait(&db);

		write_database();

		sem_post(&db);
		pthread_mutex_lock(&write_mutex);
		nb_writes--;
		if (nb_writes == 0) {
			sem_post(&rsem);
		}
		pthread_mutex_unlock(&write_mutex);
	}

	return NULL;
}

void read_database(void) {
	while (rand() > RAND_MAX / 10000);
}

void write_database(void) {
	while (rand() > RAND_MAX / 10000);
}
