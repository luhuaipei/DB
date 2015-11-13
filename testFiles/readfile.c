#include <stdio.h>
#include <stdlib.h>
 
int main()
{
	char ch;
	FILE *fp;
	int length;
	char* file_name="select.sql";
	fp = fopen(file_name,"r"); // read mode
	fseek(fp,0,SEEK_END);
	length = ftell(fp);
	char* query= (char*)malloc(sizeof(char)*length);
	if( fp == NULL ){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

//	printf("The contents of %s file are :\n", file_name);
	fseek(fp,0,SEEK_SET);
	int i=0;
	while( ( ch = fgetc(fp) ) != EOF ){
//		printf("%c",ch);
		query[i] = ch;
		i++;
	}
//	printf("query:\n%s\n",query);
	fclose(fp);
	return 0;
}