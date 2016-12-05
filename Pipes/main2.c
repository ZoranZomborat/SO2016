/*pARCURGERE DIRECTOR , FACE DOI FII, TRIMITE FISIERE NORMAL SI TRIMITE DIRECTOARELE, PRINTEAZA NUME DIM FISERE/ DOAR NUME
*/

#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>


#define limsz 10*1024


void err(char *str)
{
	printf("%s\n",str);
}

int main(int argc, char *argv[])
{

	if(argc!=2)
	{
		err("bad arguments!\n");
		exit(0);
	}

	int pid1,pid2;
	int pfd1[2],pfd2[2];

	DIR *d1,*d2;
	struct dirent *de;
	struct stat *st;
	int sc;
	int size;
	int fdt,fda;
	char symname[100];
	char filename[100];
	char tfile[100];
	char fileBuff[limsz + 3];

	if(pipe(pfd1)<0)
	{
		printf("Eroare la crearea pipe-ului\n");
		exit(1);
	}


	if((pid1=fork())<0)
	{
		printf("Eroare la fork\n");
		exit(1);
	}
	else if (pid1==0)
	{      

		int sc;
		close(pfd1[1]);
		int size;
		int filesz;
		char * buff;
		buff=(char *)malloc( 100 * sizeof(char));

		sc=read(pfd1[0],(void *)&size,sizeof(int));

		while(sc > 0)
		{

			if(size > 0){

				sc=read(pfd1[0],(void *)&buff[0],size);

				if(sc<=0)
				{
					printf("Error at read!\n");
					exit(0);
				}

				sc=read(pfd1[0],(void *)&filesz,sizeof(int));

				if(sc<=0)
				{
					printf("Error at read!\n");
					exit(0);
				}

				printf("File {%s} of size %d \n",buff,filesz);
			}

			sc=read(pfd1[0],(void *)&size,sizeof(int));

		}

		if(sc<0)
		{
			printf("Error at read!\n");
			exit(0);
		}
		else
			printf("File read terminated succesfully!\n");

		close(pfd1[0]);
		exit(0);
	}
	else
	{

		if(pipe(pfd2)<0)
		{
			printf("Eroare la crearea pipe-ului\n");
			exit(1);
		}


		if((pid2=fork())<0)
		{
			printf("Eroare la fork\n");
			exit(1);
		}
		else if (pid2==0)
		{
			int sc;
			close(pfd2[1]);
			int size;
			char * buff;
			buff=(char *)malloc( 100 * sizeof(char));

			sc=read(pfd2[0],(void *)&size,sizeof(int));

			while(sc > 0)
			{

				if(size > 0){
					sc=read(pfd2[0],(void *)&buff[0],size);

					if(sc<=0)
					{
						printf("Error at read!\n");
						exit(0);
					}

					printf("Dir [%s] \n",buff);
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

			close(pfd2[0]);
			exit(0);
		}
		else
		{
			char abs[PATH_MAX];
			close(pfd1[0]);
			close(pfd2[0]);

			de = (struct dirent *)malloc(sizeof(struct dirent));
			st = (struct stat *)malloc(sizeof(struct stat));
			realpath(argv[1] , &abs[0]);
			d1=opendir(abs);

			de = readdir (d1);
			while(de!=NULL)
			{
				if(de->d_name[0]=='.')
				{
					de = readdir (d1);
					continue;
				}
				size=strlen(abs)+strlen(de->d_name);
				sprintf(&filename[0],"%s/%s",abs,de->d_name);
				filename[size + 1]='\0';

				sc = lstat(filename, st);
				if(sc)
					err("error at stat");

				if(S_ISREG(st->st_mode))
				{
					int nr;
					int len = strlen(filename);
					int size = st->st_size;

					nr=write(pfd1[1],(void *)&len,sizeof(int));
					if(nr!=sizeof(int))
					{
						printf("Error at write header file!\n");
						exit(0);
					}

					nr=write(pfd1[1],(void *)&filename[0], len * sizeof(char));
					if(nr != (len * sizeof(char)))
					{
						printf("Error at write filename !\n");
						exit(0);
					}

					nr=write(pfd1[1],(void *)&size,sizeof(int));
					if(nr!=sizeof(int))
					{
						printf("Error at write header file!\n");
						exit(0);
					}


				}
				else if(S_ISDIR(st->st_mode))
				{	      
					int nr;
					int len = strlen(filename);

					nr=write(pfd2[1],(void *)&len,sizeof(int));
					if(nr!=sizeof(int))
					{
						printf("Error at write header file!\n");
						exit(0);
					}

					nr=write(pfd2[1],(void *)&filename[0], len * sizeof(char));
					if(nr != (len * sizeof(char)))
					{
						printf("Error at write filename !\n");
						exit(0);
					}

				}

				de = readdir (d1);
			}

			closedir(d1);

			close(pfd1[1]);
			close(pfd2[1]);

			wait();
			wait();

			exit(0);

		}
	}
}

/*

Un program care primeste ca arg un dir si afiseaza numarul total de caractere din toate numele de fisere / dir si directorul respectiv, 
pentru afisare (ls) pnetru numarare word count 

*/