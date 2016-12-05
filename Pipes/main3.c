/*

Un program care primeste ca arg un dir si afiseaza numarul total de caractere din toate numele de fisere / dir si directorul respectiv, 
pentru afisare (ls) pnetru numarare word count 

*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>


#define BUFFSIZE 1024

 void  err(char *string)
 {
	 printf("error : %s\n",string);
	 return;
 }


 int main(int argc, char *argv[])
 {
 	int pid1,pid2;
 	int pfd1[2],pfd2[2];
 	FILE *stream;
 	int sc;

 	if(argc!=2)
 		err("bad arguments");

 	printf("pid parinte %d \n",getpid());

	if(pipe(pfd1)<0)
	{
		err("Eroare la crearea pipe-ului");
		exit(1);
	}

 	if((pid1=fork())<0)
 	{
 		err("eroare la fork!");
 	}
 	else if(pid1 == 0)
 	{
 		char abs[BUFFSIZE];
 		char *rp;
 		close(pfd1[0]);
 		printf("pid c1 %d \n",getpid());
 		rp=realpath(argv[1],abs);
 		if(rp==NULL)
 			err("REALPATHERR");
 		sc=dup2(pfd1[1],1);
 		if(sc<0)
 			err("dup2 error!");

 		execlp("ls","ls",abs,"-1",NULL);

 		err("Erroare la exec");

 		close(pfd1[1]);
 		exit(1);
 	}
 	else
 	{
 		if(pipe(pfd2)<0)
		{
			err("Eroare la crearea pipe-ului");
			exit(1);
		}

 		if((pid2=fork())<0)
	 	{
	 		err("eroare la fork!");
	 	}
	 	else if(pid2 == 0)
	 	{
	 		char * fname;
	 		int size;
	 		close(pfd1[0]);
	 		close(pfd1[1]);
	 		close(pfd2[1]);

	 		fname=(char *)malloc(sizeof(char)*BUFFSIZE);

	 		printf("pid c2 %d \n",getpid());
	 		
	 		sc=read(pfd2[0],(void *)&size,sizeof(int));

			while(sc > 0)
			{

				if(size > 0){
					sc=read(pfd2[0],(void *)&fname[0],size);

					if(sc<=0)
					{
						printf("Error at read!\n");
						exit(0);
					}

					printf("[%s]\n",fname);
				}

				sc=read(pfd2[0],(void *)&size,sizeof(int));
			}

			if(sc<0)
			{
				printf("Error at read!\n");
				exit(0);
			}
			else
				printf("Directory read terminated succesfully!\n");

	 		sc=close(pfd2[0]);
	 		if(sc<0)
	 			err("err at close");
	 		exit(1);
	 	}
	 	else
	 	{
	 		char *str;
 			str=(char *)malloc( BUFFSIZE * sizeof(char));
	 		int nr;
	 		int len;
	 		close(pfd1[1]);
	 		close(pfd2[0]);

	 		//dup2(pfd2[1],1);

	 		stream = fdopen(pfd1[0],"r");
	 		nr = fscanf(stream,"%s",str);
	 		while(nr!=EOF)
	 		{	
	 			printf("{%d} nr\n",nr);
				len = strlen(str);
				nr=write(pfd2[1],(void *)&len,sizeof(int));
				if(nr!=sizeof(int))
				{
					printf("Error at write header file!\n");
					exit(0);
				}

				nr=write(pfd2[1],(void *)&str[0], len * sizeof(char));
				if(nr != (len * sizeof(char)))
				{
					printf("Error at write filename !\n");		
					exit(0);
				}
				nr = fscanf(stream,"%s",str);
				printf("{{%d}} nr\n",nr);
	 		}


	 		printf("WAITING\n");
	 		fclose(stream);
	 		close(pfd2[1]);
	 		close(pfd1[0]);
	 		printf("WAITING\n");
	 		waitpid(pid1,&sc,NULL);
	 		waitpid(pid2,&sc,NULL);
 		}
 	}

 	return 0;
 }