#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct arg_struct {
    int id;
    int PHILOSOPHES;
};

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

///AAA

void* philosophe ( void* arg )
{
    struct arg_struct *args = arg;

  int *id=(int *) args;
  int left = args->id;
  int right = (left + 1) % args->PHILOSOPHES;
  while(true) {
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
    pthread_mutex_unlock(&baguette[left]);
    pthread_mutex_unlock(&baguette[right]);
  }
  return (NULL);
}
///BBB
int main ( int argc, char *argv[])
{
    struct arg_struct args;

   args.PHILOSOPHES = atoi(argv[1]);
   int id[];
   args.id = id[args.PHILOSOPHES];
   int err;
   pthread_t phil[args.PHILOSOPHES];
    pthread_mutex_t baguette[args.PHILOSOPHES];

   srand(getpid());

   for (int i = 0; i < args.PHILOSOPHES; i++)
     id[i]=i;

   for (int i = 0; i < args.PHILOSOPHES; i++) {
     err=pthread_mutex_init( &baguette[i], NULL);
      if(err!=0)
	error(err,"pthread_mutex_init");
   }

   for (int i = 0; i < args.PHILOSOPHES; i++) {
     err=pthread_create(&phil[i], NULL, philosophe, (void *)&args );
     if(err!=0)
       error(err,"pthread_create");
   }

   for (int i = 0; i < args.PHILOSOPHES; i++) {
      pthread_join(phil[i], NULL);
      if(err!=0)
	error(err,"pthread_join");
   }

   for (int i = 0; i < args.PHILOSOPHES; i++) {
      pthread_mutex_destroy(&baguette[i]);
      if(err!=0)
	error(err,"pthread_mutex_destroy");
   }

   return (EXIT_SUCCESS);
}