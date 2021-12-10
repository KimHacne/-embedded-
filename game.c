#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<malloc.h>

int equal = 0;
int start = 0;
char answer[30];

void *GetAnswer(char *); //get answer from answer.txt

int main(){
	
	//char *answer = malloc(sizeof(char)*10);
	pthread_t thread;
	int t;
	system("python3 makeRandom.py");
	t = pthread_create(&thread,NULL,GetAnswer,NULL);
	
	pthread_exit(NULL);
}


void *GetAnswer(){

	FILE *f;
	char *tmp[3]; //start result equal
	
	int tid=pthread_self();

	while(1){
		if((f=fopen("answer.txt","r"))){
			printf("game.c -> start reading\n");
			char *str;
			int i = 0;
			printf("file reading\n");
			
			for ( i; i<3;i++){
				str = (char*)malloc(sizeof(char)*100);
				fgets(str,100,f);
				tmp[i] = str;
			}
			fclose(f);
			
			printf("file close\n");
			//printf("%s,%s,%s",tmp[0],tmp[1],tmp[2]);
			
			if(!strcmp(tmp[0],"true\n")){
				start = 1;
			}
			
			if(!strcmp(tmp[2],"true\n")){
				equal = 1;
			}
			strcpy(answer,tmp[1]);

			printf("game.c -> finish reading\n");
			printf("start = %d\n",start);
			printf("equal = %d\n",equal);
			printf("answer = %s\n",answer);
			break;
		}else{
			continue;
		}				
	}
	pthread_exit(NULL);
}
