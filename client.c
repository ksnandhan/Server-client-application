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
void printOptions(){
 
  printf("1)Register\n");
 
  printf("2)Request\n");
 
  printf("3)unregister\n");
 
  }
 
  void printRequests(){
 
  printf("4)Arithmatic\n");
 
  printf("5)isPrime\n");
 
  printf("6)even or odd\n");
 
  printf("7)isNegative\n");
 
  }
 void connectshm(int id){
   shmId=shmget((key_t)id,size, 0666);
 
	if(shmId<0)
 
	{
 
		perror("error in asking memory\n");
 
		exit(0);
 
	}
 
	//attach shared memory
 
	new=(struct message*)shmat(shmId,NULL,0);
 
	if(new==(struct message*)-1)
 
	{
 
		perror("error in asking memory\n");
 
		exit(0);
 
	}
	new->sync=0;
	new->id=id;
 }
 void connectDefault(){
   shmId=shmget(key,size, 0666);
 
	if(shmId<0)
 
	{
 
		perror("error in asking memory\n");
 
		exit(0);
 
	}
 
	//attach shared memory
 
	shm=(struct message*)shmat(shmId,NULL,0);
 
	if(shm==(struct message*)-1)
 
	{
 
		perror("error in asking memory\n");
 
		exit(0);
 
	}
	shm->sync=0;
	shm->id=def;
 }
 int checkClient(){
    char name[200];
     printf("enter Name\n");
     scanf("%s",name);
    if(shm==NULL)connectDefault();
   strcpy(shm->name,name);
   shm->option=0;
   shm->sync=1;
   while(shm->sync!=0)
   sleep(1);
   return shm->id;
}
int main()
 
{
 
	while(1){
 
	printOptions();
	int c;
	scanf("%d",&c);
         if(c==1){
         printf("client register request\n");
         int id=checkClient();
           if(id==def)
           {
             shm->option=1;
             shm->sync=1;
             while(shm->sync!=0)sleep(1);
             connectshm(shm->id);
             printf("connecting to server with id :%d\n",shm->id);
           }
           else{
           printf("already registered\n");
           }
         }
         else if(c==2){
         printf("client request processing.......\n");
          int id=checkClient();
          if(id==def)
          {
            printf("please register\n");
            continue;
           }
            printRequests();
            shm->option=2;
             int ch;
             scanf("%d",&ch);
             int n1,n2;
             char op,chrr;
            switch(ch){
            case 4:
                printf("enter number\n");
                scanf("%d",&n1);
                printf("enter number\n");
                scanf("%d",&n2);
                scanf("%c",&chrr);
                printf("enter operator\n");
                scanf("%c",&op);
                new->option=4;
                new->n1=n1;
                new->n2=n2;
                new->operator=op;
                new->sync=1;
                shm->sync=1;
                while(new->sync!=0)sleep(1);
                printf("answer of %d %c %d is %d\n",n1,op,n2,new->response);
                break;
            case 5:
                printf("enter number\n");
                scanf("%d",&n1);
                new->option=5;
                new->n1=n1;
                new->sync=1;
                shm->sync=1;
                while(new->sync!=0)sleep(1);
                if(new->response==1)
                 printf("it is a prime\n");
                 else 
                 printf("it is not a prime\n");
                break;
            case 6: 
                printf("enter number\n");
                scanf("%d",&n1);
                new->option=6;
                new->n1=n1;
                new->sync=1;
                shm->sync=1;
                while(new->sync!=0)sleep(1);
                if(new->response==1)
                 printf("it is odd\n");
                 else 
                 printf("it is even\n");
                break;
             case 7: 
              new->option=7;
              new->sync=1;
              shm->sync=1;
              while(new->sync!=0)sleep(1);
              if(new->response==0)
              printf("Not supported\n");
              break;   
            }
         }
         else if(c==3)
         {
           int id=checkClient();
           if(id==def)
           printf("not registered\n");
           else
           {
             connectshm(id);
             shm->option=3;
              new->id=id;
              new->sync=1;
              shm->sync=1;
             while(shm->sync!=0)sleep(1);
             if(shm->response==1)
             printf("unregister successfully\n");
           }
         }
      }	
 
}
 
