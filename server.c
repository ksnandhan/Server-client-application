/*    GROUP 13	
        2019B2A31399H 	B Aditya  
	2019B2A31084H	k sivanandhan
	2019A8PS0550H	Sai Akash
	2020A4PS2323H	Samarth Aashish Srivastava
	2019B3A30657H	Ravi Raj 
	2019B4A40944H	S NIKHIL
	2020B2PS1388H	mohitha
		  */


#include<stdio.h>
 
#include<unistd.h>
 
#include<sys/shm.h>
 
#include<pthread.h>
 
#include<string.h>
 
#include<stdlib.h>
 
#include<sys/ipc.h>
 
#include<stdlib.h>
 
#include<stdbool.h>
 
 
 
 
struct message{
 
   char name[200];
 
   int n1;
 
   int n2;
 
   char operator;
 
   int option;
 
   int id;
 
   bool sync;
 
   int response;
 
};
 
const int def=1235;
key_t key=def;
  int size=sizeof(struct message);
  int shmId;
  struct message* shm=NULL;
 struct message* new=NULL;
#define max_clients 100
 
int num_clients=0;
int counter=0;
int counter1[max_clients]={0};
char clients[max_clients][200];
 void deleteClient(int id){
     for(int i=id-1;i<99;i++)
     strcpy(clients[i],clients[i+1]);
 }
 void connectshm(int id){
   shmId=shmget((key_t)id,size, 0666);
 
	if(shmId<0)
 
	{
 
		perror("error in asking memory");
 
		exit(0);
 
	}
 
	//attach shared memory
 
	new=(struct message*)shmat(shmId,NULL,0);
 
	if(new==(struct message*)-1)
 
	{
 
		perror("error in asking memory");
 
		exit(0);
 
	}
	new->sync=0;
	new->id=id;
 }
 void createDefault(){
   shmId=shmget(key,size,IPC_CREAT | 0666);
 
	if(shmId<0)
 
	{
		perror("error in asking memory");
 
		exit(0);
 
	}
 
	//attach shared memory
 
	shm=(struct message*)shmat(shmId,NULL,0);
 
	if(shm==(struct message*)-1)
 
	{
		perror("error in asking memory");
 
		exit(0);
 
	}
	shm->sync=0;
	shm->id=def;
 }
 int createShm(char* name){
    strcpy(clients[num_clients++],name);
   shmId=shmget((key_t)num_clients,size,IPC_CREAT | 0666);
	if(shmId<0)
 
	{
 
		perror("error in asking memory");
 
		exit(0);
 
	}
	new=(struct message*)shmat(shmId,NULL,0);
 
	if(new==(struct message*)-1)
 
	{
 
		perror("error in asking memory");
 
		exit(0);
 
	}
	 strcpy(new->name,name);
	 new->sync=0;
	new->id=num_clients;
	return num_clients;
 }
 void checkClient(){
    shm->id=def;
    for(int i=0;i<num_clients;i++)
    {
      if(strcmp(shm->name,clients[i])==0)
        {
          printf("%s",clients[i]);
          printf("%d",num_clients);
          shm->id=i+1;
          connectshm(i+1);
          break;
        }
    }
    shm->sync=0;
 }
 void * RequestProcessing(){
    bool in=0;
  int newoption=new->option;
        if(newoption==4)
        {
        printf("arithmetic\n");
         int ans=0;
         char op=new->operator;
         printf("%c",op);
         int n1=new->n1;
         int n2=new->n2;
         if(op=='+')
         ans=n1+n2;
         else if(op=='*')
         ans=n1*n2;
         else if(op=='/')
         ans=n1/n2;
         else if(op=='-')
         ans=n1-n2;
         new->response=ans;
        }
        else if(newoption==5){
        printf("is prime\n");
          int n1=new->n1;
          new->response=1;
          for(int i=2;i<n1;i++)
          {
            if(n1%i==0){
            new->response=0;
            break;
            }
          }
        }
        else if(newoption==6){
        printf("odd/even\n");
          int n1=new->n1;
          new->response=0;
          if(n1%2)
          new->response=1;
        }
        else  if(newoption==7) {printf("is negitive?\n");new->response=0;}
        else    {in=1;  printf("enter valid option\n");}
        if(!in)      printf("generating response.....\n");
 }
int main()
 
{
printf("server intiated\n");
printf("created connect channel\n");
    createDefault();
    while(1)
    {
     if(shm->sync!=1){
     sleep(1);
     continue;
     }
     int option=shm->option;
    if(option==0){
     checkClient();
    }
    else if(option==1)
    {
     counter++;
    printf("server registeration request recieved\n");
      int id=createShm(shm->name);
      printf("new comm channel created for name %s with id %d\n",new->name,id);
      shm->id=id;
      printf("Successfully registered\n");
    }
    else if(option==2){
     counter++;
      counter1[new->id-1]++;
    printf("server request recieved\n");
    printf("\n");
    pthread_t thread;
    printf("creating threads\n");
       if(pthread_create(&thread,NULL,&RequestProcessing,NULL)!=0)
           {
             printf("error creating thread\n");
            }
        if(pthread_join(thread,NULL)!=0)
        {
           printf("error in pthread_join\n");
         }
         printf("client response sequense number for client : %s is %d\n",new->name,counter1[new->id-1]);
        new->sync=0;
    }
    else if(option==3){
         counter++;
             deleteClient(new->id);
             shmId=shmget((key_t)new->id,size,0666);
               if (shmctl(shmId, IPC_RMID, NULL) == -1) {
 
             perror("shmctl");
 
              exit(1);
              }
            printf("unregister successfully\n");
          shm->response=1;
    }
     printf("summary info............\n");
     printf("total no.of registered clients is %d\n",num_clients);
     printf("Server Response sequence number : %d\n",counter);
     shm->sync=0;
    }
 
}
 
