#include <stdio.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> 
#include <pthread.h>
#include <string.h>

#define TRUE 1 
#define FALSE 0 

#define numberOfChairs 3


//Each index represents a chair, the student number will represent an occupied chair, 0 if the chair is empty
//The lower the index represents who arrived first
int chairs[numberOfChairs]={0,0,0};

int officeHours=TRUE;
int studentsWaiting=0;
int studentsProgramming=0;
int numberOfStudents;
int executionTime;

char TA[10];
char sleeping[]="Sleeping";

void *studentActivities(void *param);
void *taActivities(void *param);

sem_t taSem;

int isLineFull(){
    
    //returns TRUE if line is full and false otherwise
    
    for(int i=0;i<numberOfChairs;i++){
        if(chairs[i]==0)
            return FALSE;
    }
    
    return TRUE;
}
int isLineEmpty(){
    for(int i=0;i<numberOfChairs;i++){
        if(chairs[i]!=0)
            return FALSE;
    }
    return TRUE; 
}
void addToChair(int ID){
    for(int i=0;i<numberOfChairs;i++){
        if(chairs[i]==0)
            chairs[1]=ID;
            
    }
}
void printState(){
    printf("%c,%d,%d\n",sleep,studentsWaiting,studentsProgramming);
}
//Moves everyone up one place in line and returns the student's ID who was just helped
int moveLineUp(){
    int id=chairs[0];
    //move students up one place in line
    
    for(int i=0;i<numberOfChairs;i++){
        if(i=numberOfChairs-1)
            chairs[i]=0;
        else
            chairs[i]=chairs[i+1];
        
    }
    return id;
}
void helpStudent(){
    sem_wait(&taSem);
    studentsWaiting--;
    usleep(rand());
    int id=moveLineUp();
    char help[]="Helping";
    strcpy(help,TA);
    printState();
    sem_post(&taSem);
}
int isInLine(int id){
    for(int i=0;i<numberOfStudents;i++){
        if(chairs[i]==id)
            return TRUE;
    }
    return FALSE;
}
int main(int argc, char *argv[]){
    if(argc==3){
        
    executionTime=atoi(argv[1]);
    numberOfStudents=atoi(argv[2]);
    
   
   
    //ta semaphore creation
    sem_init(&taSem,0,1);
    
    //Student Thread Creation
     pthread_t threadIds[numberOfStudents];
     for (int x=0; x<numberOfStudents; x++) { 
        pthread_create(&(threadIds[x]), NULL, &studentActivities, NULL);
    
    }
    
    //TA Thread Creation
    pthread_t taThread;
    pthread_create(&taThread,NULL,&taActivities,NULL);
    
    
    
    pthread_join(taThread,NULL);
        
    printf("All Done");
    
     /**/
    }
    else
        printf("Error getting arguements from command line");
       
}
void *studentActivities(void *param){
    
     
    int id=atoi(pthread_self());
    while(officeHours==TRUE){
        printf("%d - id\n",id);
       if(isInLine(id)){
        //programming
        studentsProgramming++;
        printState();
        usleep(5);
        }
        //seek help
        if(sem_trywait(&taSem)==0){
            //Is getting Help
        }
        //If line full go back to programming 
        if(isLineFull()==TRUE){
        }
        //If line not full wait in line 
        if(isLineFull()==FALSE){
            addToChair(id);
            studentsWaiting++;
            studentsProgramming--;
            printState();
            
        }
        //If a student is not in line anymore that means the teacher has helped them
        if(isInLine(id)==FALSE){
            }     
         
        
    }
}
void *taActivities(void *param){
    clock_t start=clock();
    clock_t end;
    while(officeHours==TRUE){
        
    //Checks line
    if(isLineEmpty()==TRUE){
      //If Line empty, go to sleep 
      char sleep[]="Sleeping";
      printf("%s,%d,%d\n",sleep,studentsWaiting,studentsProgramming);
    }
    else{
        
    //help student
    while(isLineEmpty()==FALSE){
        helpStudent();
    }
    }
    
  
   printf("%d,%d,%d\n",chairs[0],chairs[1],chairs[2]);
    end=clock();
    if((end-start)>=executionTime){
        printf("Office Hours Over\n");
        officeHours==FALSE;
        break;}
    }
    
}
