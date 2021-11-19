#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int nbr_philo;
pthread_mutex_t * baguette;
int cycles = 10000;

// Custom message d'erreur
void error(int err, char *msg) {
  fprintf(stderr,"%s a retourné %d message d'erreur : %s\n",msg,err,strerror(errno));
  exit(EXIT_FAILURE);
}

void mange(int id) {
  printf("Philosophe [%d] mange\n",id);
  for(int i=0;i< rand(); i++) {
    // philosophe mange
  }
}

// Fonction appelée dans le thread
void* philosophe ( void* arg )
{
  int *id=(int *) arg;
  int left = *id;
  int right = (nbr_philo!=1)?((left + 1) % nbr_philo):1;
  while(cycles>=0) {
    // philosophe pense
    if(left<right) {
      pthread_mutex_lock(&baguette[left]);
      pthread_mutex_lock(&baguette[right]);
    }
    else {
      pthread_mutex_lock(&baguette[right]);
      pthread_mutex_lock(&baguette[left]);
    }
    mange(*id);
    cycles--;
    pthread_mutex_unlock(&baguette[left]);
    pthread_mutex_unlock(&baguette[right]);
  }
  return (NULL);
}

int main ( int argc, char *argv[])
{
  int err;

  // Vérifie le nombre d'arguments
  if(argc<2){
    printf("\n Argument manquant (int) \n");
  }
  nbr_philo = atoi(argv[1]);

  // Alloue un espace memoire pour les id's
  int* id = malloc(sizeof(int) * nbr_philo);
  if(id == NULL){error(err,"malloc_id");}

  pthread_t phil[nbr_philo];

// Alloue un espace memoire pour les baguettes
  baguette = malloc(sizeof(pthread_mutex_t) * nbr_philo);
  if(baguette == NULL){error(err,"malloc_baguette");}

  srand(getpid());
  
  // Alloue un id
  for (int i = 0; i < nbr_philo; i++)
    id[i]=i;

  // Initialiser mutex
  for (int i = 0; i < nbr_philo; i++) {
    err=pthread_mutex_init( &baguette[i], NULL);
    if(err!=0) error(err,"pthread_mutex_init");
  }
  // Creer thread
  for (int i = 0; i < nbr_philo; i++) {
    err=pthread_create(&phil[i], NULL, philosophe, (void*)&(id[i]) );
    if(err!=0) error(err,"pthread_create");
  }
  // Join thread
  for (int i = 0; i < nbr_philo; i++) {
    pthread_join(phil[i], NULL);
    if(err!=0) error(err,"pthread_join");
  }

  // Detruire mutex
   for (int i = 0; i < nbr_philo; i++) {
      pthread_mutex_destroy(&baguette[i]);
      if(err!=0) error(err,"pthread_mutex_destroy");
   }

   //Liberer la mémoire allouée par les mallocs
   free(baguette);
   free(id);

   return (EXIT_SUCCESS);
}