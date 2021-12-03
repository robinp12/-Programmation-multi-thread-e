#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int nb_philo;
pthread_mutex_t *baguette;
int cycles = 100000;

// Fonction pour retourner les erreurs
void print_error(int err, char *msg) {
    fprintf(stderr, "'%s' returned error %d with message '%s'\n", msg, err,
            strerror(errno));
    exit(EXIT_FAILURE);
}

// Fonction appelée dans le philosophe
void eat(int id) {
    printf("Philosophe [%d] mange\n", id);
}

// Fonction appelée dans le thread
void *philosophe(void *arg) {
    int *id = (int *)arg;
    int left = *id;
    int right = (nb_philo != 1) ? ((left + 1) % nb_philo) : 1;

    while (cycles >= 0) {
        // philosophe pense
        if (left < right) {
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        } else {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }

        eat(*id);
        cycles--;
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int err;
    int nb_baguette;

    // Verification des arguments
    if (argc != 2) {
        printf("Error, expected one argument, but received %d.", (argc - 1));
        exit(EXIT_FAILURE);
    }
    if (isdigit(*argv[1])) {
        nb_philo = atoi(argv[1]);
    } else {
        printf("Error, the first argument should be an integer.");
        exit(EXIT_FAILURE);
    }

    if (nb_philo < 1) {
        printf("Error, the first argument should be a positive integer.");
        exit(EXIT_FAILURE);
    }

    // Allocation memoire pour l'id
    int *id = malloc(sizeof(int) * nb_philo);

    if (id == NULL) {
        print_error(-1, "malloc_id");
    }

    pthread_t phil[nb_philo];
    nb_baguette = nb_philo;

    // Cas lorsqu'il y a un philosophe
    if (nb_philo == 1) {
        nb_baguette = 2;
    }
    // Allocation memoire pour les baguettes
    baguette = malloc(sizeof(pthread_mutex_t) * nb_baguette);
    if (baguette == NULL) {
        print_error(-1, "malloc_baguette");
    }

    for (int i = 0; i < nb_philo; i++) id[i] = i;

    // Initialisation des mutex
    for (int i = 0; i < nb_baguette; i++) {
        err = pthread_mutex_init(&baguette[i], NULL);
        if (err != 0) print_error(err, "pthread_mutex_init");
    }

    // Creation des threads avec un argument id
    for (int i = 0; i < nb_philo; i++) {
        err = pthread_create(&phil[i], NULL, philosophe, (void *)&(id[i]));
        if (err != 0) print_error(err, "pthread_create");
    }

    for (int i = 0; i < nb_philo; i++) {
        pthread_join(phil[i], NULL);
        if (err != 0) print_error(err, "pthread_join");
    }

    // Destruction des mutex
    for (int i = 0; i < nb_baguette; i++) {
        pthread_mutex_destroy(&baguette[i]);
        if (err != 0) print_error(err, "pthread_mutex_destroy");
    }

    // Liberer la mémoire allouée par les mallocs
    free(baguette);
    baguette = NULL;
    free(id);
    id = NULL;

    exit(EXIT_SUCCESS);
}
