/* LEITOURGIKA SYSTHMATA 2021-22 - ERGASIA 1 - MAVROMMATIS PANAGIOTIS - sdi1800115 */
#ifndef __parent_processinput__
#define __parent_processinput__

int argsOK(int argc, char *argv[]);
int input_handler(int argc, char *argv[],char *input_file,int* no_of_processes,int* no_of_communications);
char** processFile(char *input_file,int * data_counter);

#endif