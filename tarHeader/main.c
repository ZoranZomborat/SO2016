#include <stdlib.h>
#include <stdio.h>

typedef struct tarHeader
{
	char filename[100];
	char filemode[8];
	char userUID[8];
	char groupUID[8];
	char filesizeBytes[12];
	char lastModifDate[12];
	char headerREcordChecksump[8];
	char linkIndicator;
	char nameOfLinkedFile[100];
}tarHeader;

int main(int argc, char *args[])
{
	void *magicNumber;
	magicNumber = (void *)malloc(sizeof(char) * 8);
	
	if (argc!=2){
		printf("Missuse of program.\n");
		exit(1);
	}

	FILE *fin;
	fin = fopen(args[1],"r");
	
	fseek(fin,257,SEEK_CUR);
	fread(magicNumber,sizeof(char),8,fin);
	
	printf("mnum: %s\n",(char*)magicNumber);	

	return 0;
}
