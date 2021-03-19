#include <stdio.h>
#define MIN_PID 300
#define MAX_PID 5000

/*
I was confused about using a list when the methods had void parameters and I didnt want to make a global variable because 
the assignment was to make an API so I assumed when the functions were being used they had access to some kind of global LinkedList 
variable called "list" to do their operations on. I also defined (for what was needed) a LinkedList structure that these functions 
would make use of.

*/



//Structs for the linked list
struct node {
  int pid;
  struct node *next;
};
struct LinkedList{
    struct node *root;
    
};
int removeLL(int PID){
        struct node *current=&list.root;
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




//Methods to define
int allocate_map(void){
    struct LinkedList *list=(struct LinkedList *) malloc( sizeof(struct LinkedList) ); 
    list->root=(struct node *) malloc( sizeof(struct node) ); 
    if(list==NULL)
        return -1;
    else
        return 1;
}

int allocate_pid(void){
    
    struct node *current=&list->root;
    int PIDCounter=MIN_PID;
    
    if(current->pid==NULL){
        current->pid=PIDCounter;
    }
    while(1){
        if(PIDCounter==MAX_PID+1)
            return -1;
        if(current->next==NULL){
            struct node *newNode=(struct node *) malloc( sizeof(struct node) ); ;
            newNode->pid=PIDCounter;
            return PIDCounter;
        }
        if(current->pid==NULL){
            current->pid=PIDCounter;
            return PIDCounter;
        }
        else{
            current=&current->next;
        }
        
        PIDCounter++;
    }
}
void release_pid(int pid){
    removeLL(pid);
}



