#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> 

#define MINPID 300
#define MAXPID 5000



typedef struct pid_type {
    int pidnum;
    /* process control block */
    struct pid_type* nextpid;
} pid_type;

/* One global list */
static pid_type* pidlist;
/*Semaphore to lock and unlock threads*/
sem_t mutex;

int allocate_map(void);
int allocate_pid(void);
void release_pid(int delpid);
void print_pids(void);

int main() { 
 
    sem_init(&mutex,0,1);
    int result;
    int pids[10];
    result = allocate_map();
    if (result == -1)
        exit(1);
    for (int i = 0; i < 5; i++) {
        result = allocate_pid();
        if (result < 0) {
            fprintf(stderr, "Could not allocate PID\n");
        }
        else {
            pids[i] = result;
            printf("Got back: %d\n", pids[i]);
        }
    }
    print_pids();
    release_pid(300);
    print_pids();
    release_pid(301);
    print_pids();
    release_pid(302);
    print_pids();
    release_pid(303);
    print_pids();
    
    sem_destroy(&mutex);
}

int allocate_map(void) {
   
    sem_wait(&mutex);
    pidlist = NULL;
    sem_post(&mutex);
    if (pidlist != NULL)
        return -1;
    else
        return 1;
         
    
}

int allocate_pid(void) {
    
        pid_type* temp;
        temp = (pid_type*)malloc(sizeof(pid_type));
        if (pidlist == NULL) {
            sem_wait(&mutex);
            /* First element */
            temp->pidnum = MINPID;
            temp->nextpid = temp; /* circular list */
            pidlist = temp;
            sem_post(&mutex);
            return temp->pidnum;
        }
        else if (pidlist == pidlist->nextpid) {
            sem_wait(&mutex);
            /* then single element */
            temp->pidnum = (pidlist->pidnum + 1);
            temp->nextpid = pidlist->nextpid;
            pidlist->nextpid = temp;
            pidlist = temp;
            sem_post(&mutex);
            return temp->pidnum;
        }
        else {
            sem_wait(&mutex);
            /* Multiple elements, find the next available pid */
            pid_type* pidfinder = pidlist;
            bool found = false, wrapped = false;
            while (!found && !wrapped) {
                if ((*pidfinder).pidnum < MAXPID &&
                    ((pidfinder->nextpid->pidnum < pidfinder->pidnum) ||
                        (pidfinder->nextpid->pidnum > pidfinder->pidnum + 1))) {
                    /* insert here */
                    temp->pidnum = pidfinder->pidnum + 1;
                    temp->nextpid = pidfinder->nextpid;
                    pidfinder->nextpid = temp;
                    pidlist = temp;
                    found = true;
                }
                else if (pidfinder->pidnum == MAXPID &&
                    pidfinder->nextpid->pidnum != MINPID) {
                    temp->pidnum = MINPID;
                    temp->nextpid = pidfinder->nextpid;
                    pidfinder->nextpid = temp;
                    pidlist = temp;
                    found = true;
                }
                else {
                    /* move pidfinder along the list */
                    pidfinder = pidfinder->nextpid;
                    /* if we return to start then there are no free pids */
                    if (pidfinder == pidlist)
                        wrapped = true;
                }
                sem_post(&mutex);
            }
            if (!found && wrapped)
                return -1;
            else
                return temp->pidnum;
        }
        
    
}

void release_pid(int delpid) {
    pid_type* pidfinder = pidlist;
    pid_type* pidprev; /* how to get the previous element in list? */
    pid_type* temppid = NULL;
    sem_wait(&mutex);
    /* No PIDS */
    if (pidlist == NULL) {
        return;

        /* One PID */
    }
    else if (pidlist == pidlist->nextpid) {
        if (pidfinder->pidnum == delpid) {
            /* delete this one */
            /* printf("Freeing pid: %d\n", pidfinder->pidnum); */
            temppid = pidlist;
            pidlist = NULL;
            free(temppid);
            return;
        }
        else {
            /* not the PID you're looking for */
            return;
        }

        /* More than one PID */
    }
    else {
        /* special case for head deletion */
        if (pidfinder->pidnum == delpid) {
            /* quickly loop around to find the prev list element */
            pidprev = pidfinder->nextpid;
            while (pidprev->nextpid != pidfinder)
                pidprev = pidprev->nextpid;
            /* perform the deletion */
            /* printf("Pidfinder: %p\tPidprev: %p\n", pidfinder, pidprev); */
            temppid = pidfinder;
            pidprev->nextpid = pidfinder->nextpid;
            /* deleting head means moving head */
            pidlist = pidfinder->nextpid;
            /* printf("Pidfinder: %p\tPidprev: %p\n", pidfinder, pidprev); */
            free(temppid);
            return;
            /* Find and delete */
        }
        else {
            pidprev = pidfinder;
            pidfinder = pidfinder->nextpid;
            bool found = false;
            while (!found && pidfinder != pidlist) {
                if (pidfinder->pidnum == delpid) {
                    /* printf("Freeing pid: %d\n", pidfinder->pidnum); */
                    temppid = pidfinder;
                    pidprev->nextpid = pidfinder->nextpid;
                    free(temppid);
                    found = true;
                }
                else {
                    pidprev = pidfinder;
                    pidfinder = pidfinder->nextpid;
                }
            }
            return;
        }
        sem_post(&mutex);
    }
    /* Could eliminate all the return statements and put one here.
    Would this be a good idea or not? Explain.
    */
}

void print_pids(void) {
    if (pidlist == NULL) {
        printf("NO PIDs\n");
        return;
    }
    pid_type* i = pidlist;
    bool wrapped = false;
    printf("---\n");
    while (!wrapped) {
        printf("Address: %p\tContents: pidnum:%d\tnextpid:%p\n", (void*)i,
            i->pidnum, (void*)((*i).nextpid));
        i = (*i).nextpid;
        if (i == pidlist)
            wrapped = true;
    }
    printf("---\n");
}