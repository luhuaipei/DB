#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// char* processQuery(char* query, char* userPart){
// 	//replce [##] in the query with userPart
// 	int lengthQuery = sizeof(*query)/ sizeof(char);
// 	int lengthPart = sizeof(*userPart)/ sizeof(char);
// 	char s[2];
// 	s[0]='[';
// 	s[1]='\0';
// 	char* res =	(char*)malloc(sizeof(char)*(lengthQuery+lengthPart-2));
// 	char* token;
// 	char* token1;
// 	char* token2;
// 	token = strtok(query,s);
// 	token1 = token;
// 	token = strtok(NULL, s);
// 	token2 = token;
// 	printf("token 1: %s\n",token1);
// 	printf("toekn 2: %s\n",token2);
// 	strcat(res,token1);	
// 	strcat(res,userPart);
// 	strcat(res,token2);
// 	printf("token final: %s\n",res);	
// 	return res;
// } 

char* processQuery(char* query, char* userPart,int flag){
	//replce flag=1:'[', flag=2: ']' flag=3: '?' in the query with userPart
	//warning: after return from the function, please free the response pointer
	int lengthQuery = sizeof(*query)/ sizeof(char);
	int lengthPart = sizeof(*userPart)/ sizeof(char);
	char s[2]="\0";
	if (flag==1){
		s[0]='[';   //replace the '[' in query with userPart
	}else if (flag==2){
		s[0]=']';   //replace the ']' in query with userPart
	}else{
		s[0]='?';	//replace the '?'' in query with userPart
	}
	char* res =	(char*)malloc(sizeof(char)*(lengthQuery+lengthPart-2));
	char* token;
	token = strtok(query,s);
	while(token!=NULL){
		strcat(res,token);	
		strcat(res,userPart);
		token=strtok(NULL,s);
	}
	return res;
}
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

	printf("The contents of %s file are : \n", file_name);
	fseek(fp,0,SEEK_SET);
	int i=0;
	while( ( ch = fgetc(fp) ) != EOF ){
//		printf("%c",ch);
		query[i] = ch;
		i++;
	}
	query = processQuery(query, "----",1);
	printf("query:\n%s\n",query);

	fclose(fp);
	return 0;
	}

