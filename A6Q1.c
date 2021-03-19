#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1 
#define FALSE 0 


int isUsingMem = FALSE;
int userFinished = FALSE;
int userTurn = TRUE;

int currentNum;
int sum = 0;

void* getNumbers(void* param);
void* addNumbers(void* param);

int main() {

    pthread_t inputT;
    pthread_t sumT;

    pthread_create(&inputT, NULL, getNumbers, NULL);
    pthread_create(&sumT, NULL, addNumbers, NULL);






    pthread_join(inputT, NULL);
    pthread_join(sumT, NULL);

    return 0;
}
void* getNumbers(void* param) {

    while (userFinished == FALSE) {
        if (isUsingMem == FALSE) {
            isUsingMem = TRUE;

            printf("Enter in the number to add to the sum: ");

            scanf("%d", &currentNum);




            isUsingMem = FALSE;
            userTurn = FALSE;
            sleep(1);
        }

    }
}
void* addNumbers(void* param) {
    while (userFinished == FALSE) {
        if (isUsingMem == FALSE && userTurn == FALSE) {

            isUsingMem = TRUE;
            if (currentNum < 0) {
                userFinished = TRUE;
                printf("The sum is %d", sum);
            }
            else
                sum = sum + currentNum;


            isUsingMem = FALSE;
            userTurn = TRUE;
        }
    }
}
