/*
	Author: Javier Mondragon Martin del Campo
	Institution: ITESM
	ID: A01365137
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int lim, count, on;
char buffer1[255], buffer2[255];

FILE *f;

int main(int nArgs,char *args[]){
	printf("\n");
	if(nArgs != 3){
		printf("ERROR: Arguments not valid.\n");
		return 1;
	}
	if(!strcmp("-s", args[1])){
		lim = atoi(args[2]);
	} else {
		printf("ERROR: Arguments not valid.");
		return 1;
	}
	f = fopen("file.txt", "w");
	if(f == NULL){
		printf("ERROR: File could not be opened.");
		return 1;
	}

	for(count=0; count<lim && !on; count++){
		printf("Enter Name:\n");
		scanf("%s", &buffer1);
		printf("Enter ID:\n");
		scanf("%s", &buffer2);
		fprintf(f, "%s: %s\n", buffer2, buffer1);
		printf("Do you wish to add more? [Y/n]\n");
		scanf("%s", &buffer1);
		if(strncasecmp("y", buffer1, 1))
			on = 1;
	}
	fclose(f);
	return 0;
}
