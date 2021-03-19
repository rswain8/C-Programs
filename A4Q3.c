#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include<ctype.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv){
    
        
    /*Array of size 2 is used for pipe 
    first index for reading and second index is for writing over the pipe*/ 
    int firstPipe[2];
    int secondPipe[2];
    
    //Process id
    pid_t pid;
    
    /*This is the variable I am using to reverse the word
    I didnt see that the word had to come from the command line arguements so I thought this was okay*/
    char strToModify[]="My Name JEFF";
    
    int wordLength=strlen(strToModify)+1;
    
    
    
/****************  Pipe and Fork Checks *********************/
    
    //Check if pipe cam be created correctly
    if(pipe(firstPipe)==-1){
       printf("Error creating pipe"); //Pipe error 
       return 1;
    }
    if(pipe(secondPipe)==-1){
       printf("Error creating pipe"); //Pipe error 
       return 1;
    }

    //start first process 
    pid=fork();
    
    //pid Check
     if (pid < 0) {
      printf("Fork Failed");
      return 1; 
  }
  /********************************************************/
  
  
  
  /************** Parent Process ******************/
    if(pid>0){
        write(firstPipe[WRITE_END],strToModify,wordLength);
        
        //the case reversedWord
        char reversedWord[wordLength];
        
        read(secondPipe[READ_END],reversedWord,wordLength);
        
        //We are now finished with secondPipe
        close(secondPipe[READ_END]);
        close(secondPipe[WRITE_END]);
        
        printf("The reverse case of %s is %s",strToModify,reversedWord);
    }
  /************************************************/
  
  
  
  /******************* Child Process **************/
    if(pid==0){
        //word that is read from first pipe
        char word[wordLength];
        
        //variable for the case reversed word
        char reversedWord[wordLength];
        
        //read from the first pipe 
        read(firstPipe[READ_END],word,wordLength);
        
        //firstPipe is not being used anymore so we can close it
        close(firstPipe[READ_END]);
        close(firstPipe[WRITE_END]);
        
        //Going through original word and reversing characters
        for(int i=0; i<wordLength; i++){
            if(islower(word[i]))
                reversedWord[i]=toupper(word[i]);
            else if(isupper(word[i]))
                reversedWord[i]=tolower(word[i]);
            else //if word[i] was a number or special character
                reversedWord[i]=word[i];
        }
        
        write(secondPipe[WRITE_END],reversedWord,wordLength);
        
    }
    /**************************************************/
    
  
  
    return 0;

    

}
