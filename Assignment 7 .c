#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>

#define maxSleep 3 
#define waitingSpots 3 
#define numberOfStudents 5

#define TRUE 1 
#define FALSE 0 

//semaphore declartion
sem_t students_sem;
sem_t ta_sem;

//Thread functions 
void *students(void *param);
void *TAJOB();

//Values to check office hours 
int isOfficeHours=TRUE;
int officeHours;

//Student thread array 
pthread_t student_array[numberOfStudents];
//ta thread 
pthread_t ta_thread;
//Initilize mutex
pthread_mutex_t mutexLock;

//Fucntion to create student threads 
void create_students();

//Important values to keep track of 
int waiting_students=0;
int sleep_time;
int waiting_queue[waitingSpots];
int semValue;



//main function
//argv[0] = output fiele
//argv[1] = office hours 
//argv[2] = number of students 
int main(int argc, char *argv[]){
   
    officeHours=atoi(argv[1]);
   
    //Initiate Semaphores for TA and students 
    sem_init(&students_sem,0,1);
    sem_init(&ta_sem,0,1);
    
    //Create Threads for TA and students 
    create_students();
    pthread_create(&ta_thread,NULL,TAJOB,NULL);
    
    //TA closes the Office 
    pthread_join(ta_thread,NULL);

    //Kill Threads 
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    
  
    return 0;
    
}
//create student threads 
void create_students(){
    for(int i=0;i<numberOfStudents;i++){
        int *arg=malloc(sizeof(*arg));
        
        *arg=i;
        
        if(pthread_create(&student_array[i],NULL,students,(void *)arg)){
            printf("Error creating student thread");
        }
    }
    
}
//TA doing their job 
void *TAJOB(){
   //Check if office hours are over 
   if(clock()>officeHours){
       
    printf("Office Hours are over");
    isOfficeHours==FALSE; 
    return NULL;
   }
    sem_wait(&students_sem);
    
    pthread_mutex_lock(&mutexLock);
    printf("Sleeping , %d, %d\n" ,waiting_students,(numberOfStudents-waiting_students));
    
    sleep_time = (rand()%maxSleep)+1;
    //If there is only one student waiting 
    if(waiting_students == 1){
       
        waiting_students--;
        printf("S%d , %d, %d\n",waiting_queue[0] ,waiting_students,(numberOfStudents-waiting_students));
        
    }
    //If there is two student waiting 
    else if(waiting_students == 2){
        
        waiting_students--;
      printf("S%d , %d, %d\n",waiting_queue[0] ,waiting_students,(numberOfStudents-waiting_students));
       
    }
    //If there is three student waiting 
    else if(waiting_students == 3){
        
        waiting_students--;
      printf("S%d , %d, %d\n",waiting_queue[0] ,waiting_students,(numberOfStudents-waiting_students));
       
    }
    pthread_mutex_unlock(&mutexLock);
    sleep(sleep_time);
    sem_post(&ta_sem);
    TAJOB();
}
void *students(void *param){
    
    if(isOfficeHours==FALSE){
            printf("Office Hours are over");
            return NULL;
        pthread_mutex_unlock(&mutexLock);
        
       
    }
    int i= *((int *)param);
    
    int sleeptime=(rand()&maxSleep)+1;
    //Student programming 
    sleep(sleeptime);
    
    pthread_mutex_lock(&mutexLock);
    //Check to see if ta is busy 
    sem_getvalue(&ta_sem, &semValue);
    
    pthread_mutex_unlock(&mutexLock);
    
    if(semValue>0){
        
        sem_wait(&ta_sem);
        
        pthread_mutex_lock(&mutexLock);
        //Need to check how many students are currently waiting to add to correct spot 
        if(waiting_students>=0 && waiting_students<waitingSpots){
            //No waiting students
            if(waiting_students == 0){
                
                 waiting_queue[0]=i;
                 
                waiting_students++;
                printf("S%d , %d, %d\n",waiting_queue[0] ,waiting_students,(numberOfStudents-waiting_students));
            }
            //if there is only 1 wating student
            else if(waiting_students==1){
                
                
                 waiting_queue[1]=i;
    
                waiting_students++;
                 printf("S%d , %d, %d\n",waiting_queue[0] ,waiting_students,(numberOfStudents-waiting_students));
            }
            else if(waiting_students==2){
                
                waiting_queue[2]=i;
                
                waiting_students++;
                printf("S%d , %d, %d\n",waiting_queue[0] ,waiting_students,(numberOfStudents-waiting_students));
            }
            
            pthread_mutex_unlock(&mutexLock);
            
            sem_post(&students_sem);
            sleep_time = (rand()% maxSleep)+1;
            
            sleep(sleep_time);
        }
        else{
            //Wating List is full at this time will try later
            pthread_mutex_unlock(&mutexLock);
           
            sleep(rand()%maxSleep);
            
            students(&i);
        }
    }
    else{
        sem_post(&students_sem);
    }
    //repeat 
    students((void*)&i);
    
}
