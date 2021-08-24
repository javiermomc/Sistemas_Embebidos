/*
	Author: Javier Mondragon Martin del Campo
	Institution: ITESM
	ID: A01365137
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int p;
int l;
int w;

int main(int argc, char *arg[]){
	if(argc != 6)
		printf("ERROR: Arguments not valid");
	argc=argc-2;
	if(strcmp("-p", arg[1]))
		p=0;
	else if(strcmp("-a", arg[1]))
		p=1;
	else{
		printf("ERROR: Arguments not valid\n");
		return 1;
	}
	arg = &arg[2];
	printf("\n");
	for(; argc>0; argc=argc-2){
		if(!strcmp("-l", arg[0]))
			l = atoi(arg[1]);
		else if(!strcmp("-w", arg[0]))
			w = atoi(arg[1]);
		else{
			printf("ERROR: Arguments not valid\n");
			return 1;
		}
		arg = &arg[2];
	}
	if(p)
		printf("perimeter = %d units\n", l*2+w*2);
	else
		printf("area = %d units\n", l*w);
	printf("\n");
}
