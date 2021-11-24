/* LEITOURGIKA SYSTHMATA 2021-22 - ERGASIA 1 - MAVROMMATIS PANAGIOTIS - sdi1800115 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h> 
#include <semaphore.h>
#include "defines.h"
#include "parent_processinput.h"


int main(int argc, char *argv[] )
{   
	const char* semname="/semaphore1";
	const char* semname2="/semaphore2";
	const char* semname3="/semaphore3";
	const char* name = "/shared_mem";
	//se periptwsh pou to programma den eixe termatisei swsta thn prohgoumenh fora kanw unlink tous semaphorous kai thn shared memory gia na mhn dimourgithei provlima
	sem_unlink(semname);
	sem_unlink(semname2);
	sem_unlink(semname3);
	shm_unlink(name);


	char input_file[256];//X
	int no_of_processes=0,no_of_communications=0;//K,N
	int ret=0,data_counter=0;
	char* path="./os1_child";
	char** data_array;

	ret=input_handler(argc,argv,input_file,&no_of_processes,&no_of_communications);
	if(ret!=0)
		return -1;

	printf("input_file(X): %s ,no_of_processes(K): %d , no_of_communications(N): %d\n ",input_file,no_of_processes,no_of_communications);

	data_array=processFile(input_file,&data_counter);
	if(data_array==NULL)
		return -1;
	
/*
	printf("%d\n",data_counter);
	for(int i=0;i<data_counter;i++){
		printf("%s\n",data_array[i]);
	}
*/

	//ftiaxnoume ta orismata se string gia ta paidia
	char strdatacounter[10],strnoofcommunications[10];
	sprintf(strdatacounter, "%d",data_counter );
	sprintf(strnoofcommunications, "%d", no_of_communications);


	//ftiaxnoume thn shared_memory

    int shm_fd; /* shared memory file descriptor */
    void* ptr;  //gia thn shared memory xrhsimopoioume void pointer gia na mporoume  na metaferoume kai int kai string 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); //dimiourgoume h diavazoume an exei hdh dhmiourgithei thn shared memory
   	ftruncate(shm_fd, MAX_LINE); //kathorizoume to megethos ths shared memory
    ptr = mmap(0, MAX_LINE, PROT_WRITE, MAP_SHARED, shm_fd, 0); //maps files or devices into memory, antistoixizei to arxeio tou shared memory me thn antistoixei thesh sthn mnhmh

	//semaphores
    sem_t* sem1;
    sem1=sem_open(semname,O_CREAT,0666,0);//ksekinaei me 0 dioti me to pou ginei wait h diergasia pagwnei kai perimenei na ginei post
 
    sem_t* sem2;
    sem2=sem_open(semname2,O_CREAT,0666,0);
    
    sem_t* sem3;
    sem3=sem_open(semname3,O_CREAT,0666,1);//ksekinaei me 1 giati h prwth diergasia pou tha kanei wait prepei na ksekinisei kai na mhn perimenei na kanei post o parent

	int  id[no_of_processes];//pinakas pou krataw tis epistrofes ths fork

	id[0]=fork();
	if(id[0]==0){//ama eimai sto paidi
		execl(path,path,strdatacounter,strnoofcommunications,NULL);// ektelw to paidi me ta swsta orismata
	}
	for(int i=1;i<no_of_processes;i++){
		id[i]=fork();
		if(id[i]==0){//ama eimai sto paidi kalw
			execl(path,path,strdatacounter,strnoofcommunications,NULL);// ektelw to paidi me ta swsta orismata
		}
	}
	
	char test[MAX_LINE];

	for(int i=0;i<no_of_processes;i++){
		for(int j=0;j<no_of_communications;j++){
			sem_wait(sem1);//perimenw na grapsei h diergasia paidi ton aithmo ths grammhs pou thelei
			strcpy(test,(char*)ptr);//Afou parw to sem_post apo thn diergasia paidi apothikevw ton arithmo ths grammhs poy zhtaei to paidi kai exei grapsei sthn shared memory
			int temp_int=atoi(test);//metatrepw ton arithmo se int
			sprintf(ptr, "%s", data_array[temp_int]);//apothikevw thn grammh sthn shared memory
			sem_post(sem2);//enhmwrwnw to paidi oti graftike h grammh sthn shared memory
		}
		usleep(1000);//0.001 Seconds kathisterish ana process gia na mhn yparxei periptwsh lathos apotelesmatos.
		sem_post(sem3);//ksekinaw na asxoloumai me thn epomenh diergasia
		
	}
	
	int status[no_of_processes];
	int ret_kids=0;
	for(int i=0;i<no_of_processes;i++){
		waitpid(id[i],&status[i],0);

        int return_value = WEXITSTATUS(status[i]);
        ret_kids+=return_value;
        //printf("Return value of %d : %d\n", id[i],return_value);
    
	}
	if(ret_kids>0)
		printf("Someting went wrong with %d kids\n",ret_kids);
	else
		printf("All kids returned 0!\n");
	//while((temp=wait(&temp2))>0);

//FREE'S
	for(int i=0; i<data_counter;i++){
		free(data_array[i]);

	}
	free(data_array);

	shm_unlink(name);//kanw unlink thn shared memory apo to arxeio

	sem_close(sem1);//kleinw tous semahorous kai tous kanw unlink apo to arxeio
  	sem_unlink(semname);
  	sem_close(sem2);
  	sem_unlink(semname2);
  	sem_close(sem3);
  	sem_unlink(semname3);
return 0;

}