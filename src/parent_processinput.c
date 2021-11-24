/* LEITOURGIKA SYSTHMATA 2021-22 - ERGASIA 1 - MAVROMMATIS PANAGIOTIS - sdi1800115 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "parent_processinput.h"
//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Error in # of args\n");
        return 0;
    }
    return 1;
}

int input_handler(int argc, char *argv[],char *input_file,int* no_of_processes,int* no_of_communications){
	if (!argsOK(argc, argv)) {
		printf("Exiting....\n");
		return -1; //error checks on arguments
	}
	strcpy(input_file,argv[1]);//apothikevw ta orismata 
	(*no_of_processes)=atoi(argv[2]);
	(*no_of_communications)=atoi(argv[3]);
return 0;

}

char** processFile(char *input_file,int * data_counter){
	FILE *file;
	//int data_counter=0;
	file = fopen( input_file, "r");
	char *line=NULL;
	size_t len;
	if (! file)
    {
        printf("file not found\n");
        return NULL;
    }
     while (getline(&line, &len, file)!=-1)
   	{
   		(*data_counter)++;//metraw to megethos tou arxeiou

    }

    char** data_array;
   	data_array=malloc((*data_counter)*sizeof(char*));//kanw analogi desmefsh mnhmhs
   //	for(int i=0;i<(*data_counter);i++){
   		//data_array[i]=malloc(MAX_LINE*sizeof(char));
   	//}
   	fclose(file);//kleinw kai ksananoigw to arxeio
   	file = fopen( input_file, "r");
   	if (! file)
    {
        printf("file not found\n");
        return NULL;
    }
    int counter=0;
    ssize_t chars;
     while (chars=getline(&line, &len, file)!=-1)
   	{	
        data_array[counter]=strdup(line);
   		data_array[counter][strlen(data_array[counter])-1]='\0';//den thelw to '\n', opote termatizw to string mia thesh nwritera
   		counter++;

    }
    free(line);
    fclose(file);
    return data_array;
}
