#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MIN_PID 300
#define MAX_PID 5000
#define MAX_THREADS 4


struct LinkedList *list;



//Struct for node of Linked List
struct node {
  int pid;
  struct node *next;
};

//Struct for LinkedList
struct LinkedList{
    struct node *root;
    
};

//function to remove a certain pid from the list
//param PID - pid to be removed
int removeLL(int PID){
        struct node *current=list->root;
        struct node *before=NULL;
        
        while(1){
            if(current->pid==PID){
                before->next=current->next;
                free(current);
                return 1;
            }
            else if(current->next==NULL){
                return -1;
            }
            else{
                before=before->next;
                current=current->next;
            }
        }
    }

//function to create LinkedList
int allocate_map(void){
    struct LinkedList *list=(struct LinkedList *) malloc( sizeof(struct LinkedList) ); 
    list->root=(struct node *) malloc( sizeof(struct node) ); 
    if(list==NULL)
        return -1;
    else
        return 1;
}

//function to allocate a pid
int allocate_pid(void){
    //create a node to iterate through the list
    struct node *current=list->root;

    int PIDCounter=MIN_PID;
    
    //for a list with no current process ids 
    if(current->pid<=0){
        current->pid=PIDCounter;
    }
    //iterate through the list and create a new node at the end for the newe pid
    while(1){
        if(PIDCounter==MAX_PID+1)
            return -1;
        if(current->next==NULL){
            struct node *newNode=(struct node *) malloc( sizeof(struct node) ); ;
            newNode->pid=PIDCounter;
            return PIDCounter;
        }
        if(current->pid<=0){
            current->pid=PIDCounter;
            return PIDCounter;
        }
        else{
            current=current->next;
        }
        
        PIDCounter++;
    }
}

//function to release a pid from the linked List
//@param pid - pid to be released
void release_pid(int pid){
    removeLL(pid);
}

//function to mimic the process of pid requesting, execution and release
void *mimicProcess(void *param){
    
    int pid=(int)allocate_pid();
    printf("Requesting pid %d\n",pid);
    sleep(rand()/10000);
    
    release_pid(pid);
    printf("Releasing pid %d\n",pid);
    
   
}



int main(){
    //Create List to hold PIDs
    if(allocate_map()==1){
    
    //Thread Array Declaration
     pthread_t threadIds[MAX_THREADS];

    
    //Create and start threads in threadIds
    for (int i=0; i<MAX_THREADS; i++) {
    pthread_create(&(threadIds[i]), NULL, &mimicProcess, NULL);
    printf("Creating thread number %d, Thread ID=%lu \n", i, threadIds[i]);
    }
    
    
    //Wait for threads to finish and pids to be released 
    for (int i=0; i<MAX_THREADS; i++) {
    pthread_join((threadIds[i]),NULL);
    }
    
    
  
    }
    else
        printf("Error allocating List");
    
    
    
    return 0;
}

