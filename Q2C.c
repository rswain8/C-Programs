#include <stdio.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> 
#include <pthread.h>

//to get rid of unused parameter warning
#define UNUSED(x) (void)(x)

#define TRUE 1 
#define FALSE 0

#define SOUTHBOUND 0
#define NORTHBOUND 1



//Values to determine whos turn it is to cross the bridge 
int isNorthTurn=TRUE;
int isFirstArriving=TRUE;

//represent how many farmers are waiting on each side 
int northLoad=0;
int southLoad=0;


//Binary Semaphore 
sem_t mutex;

void *crossBridge(void *param);

/*Command Line Arguements
argv[0]= program filename
argv[1]= number of southbound farmers
argv[2]= number of northbound farmers
*/
int main(int argc, char *argv[]) {
    if(argc==3){ 
    sem_init(&mutex,0,1);
    
    int southAmount=atoi(argv[1]);
    int northAmount=atoi(argv[2]);
    
    
    //Thread Array Declaration
     pthread_t threadIds[southAmount+northAmount];
    
    printf("-- Vermont Bridge Traffic --\n");
    
    //Create and start threads for Southbound Farmers 
    int s=SOUTHBOUND; 
    for (int i=0; i<southAmount; i++) { 
    printf("A SouthBound Farmer Arrives\n");
    southLoad++;
    pthread_create(&(threadIds[i]), NULL, &crossBridge, (void* )&s);
    
    }
    
    //Create and start threads for NorthBound Farmers 
    int n=NORTHBOUND; 
    for (int i=southAmount; i<northAmount+southAmount; i++) {
    printf("A NorthBound Farmer Arrives\n");
    northLoad++;
    pthread_create(&(threadIds[i]), NULL, &crossBridge, (void* )&n);
    }
    
    //Wait for threads to finish and pids to be released 
    for (int i=0; i<southAmount+northAmount; i++) {
    (void) pthread_join((threadIds[i]),NULL);
    }
    
    printf("All Farmers have successfully crossed the bridge");
    sem_destroy(&mutex);
    }
    else
        printf("Error getting Command Line Arguements");
        
    return 0;
}

//Function to represent how farmers wait for their turn and then cross the bridge in a random amount of time 
//@param param=An int that represents the direction of the farmer either 1 or 0 as defined at the top
void *crossBridge(void *param){
    
    
    int direction = *((int *)param);
    
    //So farmers don't take a silly amount of time to cross the bridge (<3000 microseconds)
    int timeToCross =  rand()%3000; 
    
    //The First farmer to arrive gets to go first 
    if(isFirstArriving==TRUE){
        isFirstArriving=FALSE;

        if(direction==NORTHBOUND)
            isNorthTurn=TRUE;
        else if(direction==SOUTHBOUND)
            isNorthTurn=FALSE;
        
    }
    
    
    /*
    Not very efficient but does prevent deadlocking.
    The Bridge alternates between which direction of farmers are allowed to cross.
    A farmer has to wait for their direction to be allowed to cross and their place at the front of the line
    unless there are no other farmers at the other side of the bridge waiting
    */
    
    while(TRUE){
        sem_wait(&mutex);
        if((isNorthTurn==TRUE && direction==NORTHBOUND) || (direction==NORTHBOUND && southLoad==0)){
           
           northLoad--;
           printf("Crossing: NB - SouthBound waiting: %d - NorthBound waiting: %d\n",southLoad,northLoad);
           
           usleep(timeToCross);
           isNorthTurn=FALSE;
           sem_post(&mutex);
           break;
      } 
      else if((isNorthTurn==FALSE && direction==SOUTHBOUND) || (direction==SOUTHBOUND && northLoad==0)){

           southLoad--;
           printf("Crossing: SB - SouthBound waiting: %d - NorthBound waiting: %d\n",southLoad,northLoad);
           
           usleep(timeToCross);
           isNorthTurn=TRUE;
           sem_post(&mutex);
           break;
        }
        sem_post(&mutex);
    }
    
}


