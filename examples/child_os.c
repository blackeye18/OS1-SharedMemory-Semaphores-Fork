/* LEITOURGIKA SYSTHMATA 2021-22 - ERGASIA 1 - MAVROMMATIS PANAGIOTIS - sdi1800115 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "defines.h"

//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error in # of args\n");
        return 0;
    }
    return 1;
}


int input_handler(int argc, char *argv[],int* no_of_filelines,int* no_of_communications){

	if (!argsOK(argc, argv)) {
		printf("Exiting....\n");
		return -1; //error checks on arguments
	}
	(*no_of_filelines)=atoi(argv[1]);//diavazoume kai apothikevoume ta orismata
	(*no_of_communications)=atoi(argv[2]);
	return 0;
}

int main(int argc, char *argv[] )
{	//to srand tha einai ^getpid() gia na yparxei tyxaiothta kathws diaforetika ola ta paidia vgazane idies times
	pid_t pid=getpid();
	srand(time(NULL)^pid);

	int no_of_communications=0,no_of_filelines=0;
	
	int ret=input_handler(argc,argv,&no_of_filelines,&no_of_communications);
	if(ret!=0)
		return -1;

	//printf("Child::: no_of_filelines(K): %d , no_of_communications(N): %d\n ",no_of_filelines,no_of_communications);

	const char* name = "/shared_mem";
    int shm_fd; /* shared memory file descriptor */
    void* ptr;  //gia thn shared memory xrhsimopoioume void pointer gia na mporoume  na metaferoume kai int kai string 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); //dimiourgoume h diavazoume an exei hdh dhmiourgithei thn shared memory
   	ftruncate(shm_fd, MAX_LINE); //kathorizoume to megethos ths shared memory
    ptr = mmap(0, MAX_LINE, PROT_WRITE, MAP_SHARED, shm_fd, 0); //maps files or devices into memory, antistoixizei to arxeio tou shared memory me thn antistoixei thesh sthn mnhmh

	//semaphores
    const char* semname="/semaphore1";// onoma semaforou
    sem_t* sem1;
    sem1=sem_open(semname,O_CREAT,0666,0);//ksekinaei me 0 dioti me to pou ginei wait h diergasia pagwnei kai perimenei na ginei post
    const char* semname2="/semaphore2";
    sem_t* sem2;
    sem2=sem_open(semname2,O_CREAT,0666,0);
    const char* semname3="/semaphore3";
    sem_t* sem3;
    sem3=sem_open(semname3,O_CREAT,0666,1);//ksekinaei me 1 giati h prwth diergasia pou tha kanei wait prepei na ksekinisei kai na mhn perimenei na kanei post o parent

    char test[MAX_LINE];
    clock_t start, end;
    double time_used_by_com=0,sum_time=0;
  
    sem_wait(sem3);//semaforos poy 

	for(int i=0;i<no_of_communications;i++){
		int randi=rand()%no_of_filelines;//ypologizw thn rand timh
		//printf("randi:%d\n",randi);
		start=clock();
		sprintf(ptr, "%d", randi);//thn grafw sthn shared memory

		sem_post(sem1);//enhmerwnw oti exei graftei h plhroforia sthn shared memory

		sem_wait(sem2);//perimenw na graftei h grammh sthn shared memory
		strcpy(test,(char*)ptr);//molis graftei thn diavazw
		end=clock();
		time_used_by_com=((double) (end - start)) / CLOCKS_PER_SEC;
		sum_time+=time_used_by_com;//ypologizw ton synoliko xrono gia ta communication
		printf("CHILD:: line %d : %s \n",randi+1,test);
		
	}

	sum_time=sum_time/no_of_communications;//ypologizw ton meso oro
	printf("CHILD with pid %d average time: %f\n",pid,sum_time);
	sem_close(sem1);
	sem_close(sem2);
	sem_close(sem3);
	return 0;
}
