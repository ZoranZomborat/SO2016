#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef void* (*ThreadHandler)(void *arg);

int *sums;
volatile int count = 0;
pthread_t *threads;
ThreadHandler *thandlers;

void *THandler(void *arg)
{
	int *iarg = (int *)arg;
	(*iarg)++;
	printf("hi from %d!\n",*iarg);
	return NULL;
}

void err(char *str)
{
	printf("error : %s [%s] \n",str,strerror(errno));
	exit(0);
}

int main(int argc, char const *argv[])
{
	int n,i;
	int sc;

	if(argc!=3)
		err("bad arguments");

	n=atoi(argv[2]);
	sums=(int*)malloc(sizeof(int)*n);
	threads=(pthread_t*)malloc(sizeof(pthread_t)*n);
	thandlers=(ThreadHandler*)malloc(sizeof(ThreadHandler)*n);
	
	for(i=0;i<n;i++){
		thandlers[i]=&THandler;
	}

	for(i=0;i<n;i++){
		sc=pthread_create(&threads[i], NULL, thandlers[i] , (void*)(&count));

		if(sc)
			err("thread creation failed");

	}

	for(i=0;i<n;i++){
		sc=pthread_join(threads[i], NULL);

		if(sc)
			err("thread join failed");

	}

	printf("done\n");
	return 0;
}