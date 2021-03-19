#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAX_NUMBER_OF_PROCESS 10
#define MAX_TIME 100

//Keeps track of number of processes 
int sizeP=0;

//Struct to define a Process 
typedef struct node{
    char *processNumber;
    int arrivalTime;
    int priority;
    int burstTime;
    int responseTime;
    
    //Status of completetion
    // 0 = Not Completed
    // 1 = Completed
    int status;
    
    //Status of arrival 
    //0 = not arrived
    //1 = arrived
    int arrivalStatus;
}node;

//Functions to help with simulation and ordering
void swap();
void orderProcessesByArrival();


//Array that holds all processes read in from CSV file 
struct node *processes[MAX_NUMBER_OF_PROCESS];

int main(int argc, char *argv){
    
    //Gather and format the process input 
    FILE *inputFile;
    inputFile = fopen("project-processes.txt","r");
    
    if(inputFile==NULL){
        perror("Could not find the projectprocesses.txt");
        return 1;
    }
    
    
    //Variables used in the strtok function to read values from csv file 
    char *token;
    const char s[2]=",";
    char line[20];
    
        printf("Attempting to read process information from csv file\n");
        //process of reading in line and creating process structs
        int indexCounter=0;
        while(fgets(line,sizeof(line),inputFile)!=NULL){
             
            struct node *temp=malloc(sizeof(struct node));
            
            //Process Number (name)
            token=strtok(line,s);
            temp->processNumber=malloc(sizeof(char));
            strcpy(temp->processNumber,token);
            
            //Arrival Time
            token=strtok(NULL,s);
            int at=atoi(token);
            temp->arrivalTime=at;  
           
            //Priority 
            token=strtok(NULL,s);
            temp->priority=atoi(token);
            
            //Burst Time 
            token=strtok(NULL,s);
            temp->burstTime=atoi(token);
            
            //Status and arrival Status are both 0 as simulation hasn't started yet 
            temp->status=0;
            temp->arrivalStatus=0;
            
            //put created process struct in the array
            processes[indexCounter]=temp;
            indexCounter++;
            sizeP++;
            printf("Loading from text file Process with ID - %s | Arrival - %d | Priority - %d | CPU Burst - %d\n",temp->processNumber,temp->arrivalTime,temp->priority,temp->burstTime);
            
            }
            
            printf("\n");
            
    //organize processes into ascending arrival time      
    orderProcessesByArrival();
    printf("Organizing by Arrival Time with new order:\n");
    for(int i=0;i<4;i++){
        printf("Process with ID - %s | Arrival - %d | Priority - %d | CPU Burst - %d\n",processes[i]->processNumber,processes[i]->arrivalTime,processes[i]->priority,processes[i]->burstTime);

    }
    
    // -- Simulation Start -- //
    
    //Current time represents current system time in ms 
    int currentTime;
    //nextArrivalIndex represents the place in the processes array which holds the process that is arriving next 
    int nextArrivalIndex=0;
    //running index represents the place in array which holds the currently running process 
    int runningIndex=-1;
    //lastHighestPriorityIndex represents the place in the array which holds the previous highest priority ready to run process 
    int lastHighestPriorityIndex=-1;
    
    printf("\n-- Start Simulation --\n");
    
    for(currentTime=0; currentTime<=MAX_TIME; currentTime++){
        
        //check if another process arrives
        if(currentTime==processes[nextArrivalIndex]->arrivalTime){
            processes[nextArrivalIndex]->arrivalStatus=1;
            
            if(nextArrivalIndex<sizeP-1)
                nextArrivalIndex++;
        }
        
        //check and see which arrived process has higher priority and run it
            
            int highestPriority=99999;
            
            for(int i=0; i<sizeP; i++){
                if(processes[i]->arrivalStatus==1 && processes[i]->priority<highestPriority && processes[i]->status==0){
                    runningIndex=i;
                    
                }
            } 
            
        
        
        //if a currently running process is finished its cpu burst 
        if(runningIndex>=0 && runningIndex<sizeP)
            if(processes[runningIndex]->burstTime==0){
                processes[runningIndex]->status=1;
                printf("Process %s Completed\n",processes[runningIndex]->processNumber);
                runningIndex=lastHighestPriorityIndex;
            }
        if(runningIndex>=0 && runningIndex<sizeP){
        processes[runningIndex]->burstTime=processes[runningIndex]->burstTime-1;
        printf("Running Process: %s\tPriority: %d\tCurrent Scheduler Time: %d\tProcess Burst Time Left: %d\n",processes[runningIndex]->processNumber,processes[runningIndex]->priority,currentTime,processes[runningIndex]->burstTime);
        }
       
    }
    return 0;
}
//Simple selection sort to order proccesses by arrival time 
void orderProcessesByArrival(){
    int i, j, minIdx;
    int n=sizeP;
    
    for(i=0;i<n-1;i++){
        minIdx=i;
        for(j=i+1;j<n;j++)
            if(processes[j]->arrivalTime < processes[minIdx]->arrivalTime)
                minIdx=j;
          
        swap(processes[minIdx],processes[i]);
    }
}
//Swap function to help with orderProcessesByArrival() 
//Swaps values of two pointers
void swap(struct node *xp, struct node *yp){
   
    char *xpName=xp->processNumber;
    char *ypName=yp->processNumber;
     //printf("%s -- %s \n",xpName,ypName);
    struct node temp=*xp;
    *xp=*yp;
    *yp=temp;
    
    xp->processNumber=ypName;
    yp->processNumber=xpName;
}




