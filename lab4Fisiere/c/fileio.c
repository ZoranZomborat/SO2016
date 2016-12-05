#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

//typedef struct stat stats;

int main(int argn,void *args[])
{
	int fd;
	struct stat st;
	fd = open(args[1],O_RDWR);
	lseek(fd, -1 , SEEK_SET);
	stat(args [1], &st);	
	printf("Mesaj %s!\n", strerror(errno));
	close(fd);
	return 0;
}

