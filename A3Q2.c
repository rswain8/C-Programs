#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int finalAverage;
int finalMax;
int finalMin;

//Easy function for finding an average of a set of number with a certain size 
//Param @values - array of int values to be averaged
//Param @size - size of values
void *average(int values[],int size){
    int counter=0;
    int total=0;
    
   
    while(counter<size){
       total=total+values[counter];
     
       counter++;
      
       if(counter==size)
        break;
    }
    
   finalAverage=total/size;
}
//Easy function for finding the maximum value of a set of number with a certain size 
//Param @values - array of int values to find the maximum value of
//Param @size - size of values
void *max(int values[],int size){
    int counter=0;
    while(counter<size){
        if(counter==0){
            finalMax=values[counter];
        }
        else if(values[counter]>finalMax)
            finalMax=values[counter];
         counter++;   
    }
}
//Easy function for finding the minimum value of a set of number with a certain size 
//Param @values - array of int values to find the minimum value of
//Param @size - size of values
void *min(int values[],int size){
    int counter=0;
    while(counter<size){
        if(counter==0){
            finalMin=values[counter];
        }
        else if(values[counter]<finalMin)
            finalMin=values[counter];
        counter++;
    }
}

int main(int argc, char *argv[]){
    if(argc>1){
    
    //array of values to be operated on with size argc-1 
    int values[argc-1];
    int counter=1;
   
    //create values in the list with the values given from the command line
    while(counter<argc ){
      
        values[counter-1]=atoi(argv[counter]);
        
        counter++; 
    }
     
   
    //Thread declaration
    pthread_t averageT;
    pthread_t maxT;
    pthread_t minT;
    
    //Create and start threads
    pthread_create(&averageT, NULL, average(values, argc-1), NULL); 
    pthread_create(&maxT, NULL, max(values, argc-1), NULL); 
    pthread_create(&minT, NULL, min(values, argc-1), NULL); 
    
    
    

    
    fflush(stdout);
    printf("The average value is %d\nThe minimum is %d\nThe maximum is %d",finalAverage,finalMin,finalMax);
    }
    else
    printf("Error getting arguements");
    
    return 0;
}
