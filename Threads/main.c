#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFSZ 100
typedef void* (*ThreadHandler)(void *arg);

int *sums, *pos, *counts;
float *avg;
volatile int inUse = 0;
pthread_t *threads;
ThreadHandler *thandlers;
static FILE *fin;

void *THandler(void *arg)
{
	int i = *((int *)arg);
	char ch;
	int fpos;
	int nr,sc;
	sums[i]=0;

	while(inUse);

	inUse=1;
	fpos=ftell(fin);
	sc=fscanf(fin,"%c",&ch);
	inUse=0;

	while(sc!=EOF)
	{
		nr=atoi(&ch);
		sums[i]+=nr;
		counts[i]++;
		//printf("%d:%d@%d\n",i,nr,fpos);
		//write(1,&ch,1);
		while(inUse);
		
		inUse=1;
		fpos=ftell(fin);
		sc=fscanf(fin,"%c",&ch);
		inUse=0;
	}

	if(counts[i])
		avg[i]=((float)sums[i])/counts[i];
	printf("sum %d cnt %d avg %f\n",sums[i],counts[i],avg[i]);

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
	char *abs;
	float totalavg=0;

	if(argc!=3)
		err("bad arguments");

	abs=(char*)malloc(BUFFSZ);
	abs=realpath(argv[1],abs);

	if(abs==NULL)
		err("realpath");

	fin=fopen(abs,"r");

	n=atoi(argv[2]);
	sums=(int*)malloc(sizeof(int)*n);
	pos=(int*)malloc(sizeof(int)*n);
	counts=(int*)malloc(sizeof(int)*n);
	avg=(float*)malloc(sizeof(float)*n);
	threads=(pthread_t*)malloc(sizeof(pthread_t)*n);
	thandlers=(ThreadHandler*)malloc(sizeof(ThreadHandler)*n);
	
	for(i=0;i<n;i++){
		thandlers[i]=&THandler;
		pos[i]=i;
		counts[i]=0;
		sums[i]=0;
	}

	for(i=0;i<n;i++){
		int cnt = i;
		sc=pthread_create(&threads[i], NULL, thandlers[i] , (void*)(&pos[i]));

		if(sc)
			err("thread creation failed");

	}

	for(i=0;i<n;i++){
		sc=pthread_join(threads[i], NULL);

		if(sc)
			err("thread join failed");

	}

	for(i=0;i<n;i++)
		totalavg+=avg[i];

	totalavg=totalavg/n;
	printf("%f\n",totalavg);

	return 0;
}