#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_CYCLES 10000


void think();
void eat();
void *dine(void *id);


pthread_t *philosopher_threads;
pthread_mutex_t *fork_mutexes;
int nb_philosophers;


int main(int argc, char *argv[]) {
	if (argc != 2 || (nb_philosophers = atoi(argv[1])) == 0) {
		printf("ERROR, this program expects a single integer argument");
		return EXIT_FAILURE;
	}

	int *philosopher_ids = (int *) malloc(nb_philosophers * sizeof(int));
	philosopher_threads = (pthread_t *) malloc(nb_philosophers * sizeof(pthread_t));
	fork_mutexes = (pthread_mutex_t *) malloc(nb_philosophers * sizeof(pthread_mutex_t));

	if (philosopher_ids == NULL || philosopher_threads == NULL || fork_mutexes == NULL) {
		printf("ERROR, out of memory");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < nb_philosophers; i++) {
		philosopher_ids[i] = i;
		pthread_mutex_init(&fork_mutexes[i], NULL);
	}

	for (int i = 0; i < nb_philosophers; i++) {
		pthread_create(&philosopher_threads[i], NULL, dine, (void *) &philosopher_ids[i]);
	}

	for (int i = 0; i < nb_philosophers; i++) {
		pthread_join(philosopher_threads[i], NULL);
	}

	for (int i = 0; i < nb_philosophers; i++) {
		pthread_mutex_destroy(fork_mutexes[i]);
	}

	free(philosopher_ids);
	free(philosopher_threads);
	free(fork_mutexes);

	return EXIT_SUCCESS;
}

void think() {
	// thinking
}

void eat() {
	// eating
}

void *dine(void *arg) {
	int *id = (int *) arg;
	int left = *id;
	int right = (left + 1) % nb_philosophers;

	for (int i = 0; i < NB_CYCLES; i++) {
		think();

		if (*id % 2 == 0) {
			pthread_mutex_lock(&fork_mutexes[left]);
			pthread_mutex_lock(&fork_mutexes[right]);
		}
		else {
			pthread_mutex_lock(&fork_mutexes[right]);
			pthread_mutex_lock(&fork_mutexes[left]);
		}

		eat();
		pthread_mutex_unlock(&fork_mutexes[left]);
		pthread_mutex_unlock(&fork_mutexes[right]);
	}

	return NULL;
}
