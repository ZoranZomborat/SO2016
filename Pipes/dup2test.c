#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "unistd.h"

#define BUFFSIZE 100

int main(int argc, char* argv[])
{
	int pfd[2];
	int pid,pid2;

	if(pipe(pfd)==-1)
	{
		printf("Error: %s\n",strerror(errno));
		exit(0);
	}

	if( (pid=fork()) == -1)
	{
		printf("Error: %s\n",strerror(errno));
		exit(0);
	}
	else if(!pid)
	{
		close(pfd[1]);
		int sc;
		int nr;

				dup2(pfd[0],0);
				execlp("wc","wc","-m",NULL);

				printf("error at execlp!\n");

		close(pfd[0]);
		exit(0);
		
	}
	else{	
		char *str;
		str = (char*) malloc(BUFFSIZE * sizeof(char));
		close(pfd[0]);

		str = "aaaastring si mai lungaaaastring\0";
		sync();
		write(pfd[1],str,strlen(str));

		close(pfd[1]);
		exit(0);
	}

	return 0;
}