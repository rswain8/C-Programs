#include <stdio.h>

int main() {


    printf("Enter input file name :");

    //Windows allows up to 260 characters in a filename
    const char* fileIn[260];

    //Get input file name/location
   //fgets(fileIn,260,stdin);
    scanf("%s", &fileIn);

    //Same thing for output file
    printf("Enter output file name :");
    char* fileOut[260];

    scanf("%s", &fileOut);




    //Create output file
    FILE* outputFile = NULL;

    //open input file
    FILE* inputFile;

    int maxCharacters = 1024;

    char* lineread[maxCharacters];







    //check if input file exists 
    inputFile = fopen(fileIn, "r+");

    if (inputFile == NULL) {
        printf("Error opening input file, terminating program");
        return 0;
    }



    outputFile = fopen(fileOut, "w+");

    if (outputFile == NULL) {
        printf("Error creating output file, terminating program");
        return 0;
    }


    while (fgets(lineread, maxCharacters, inputFile)) {

        fprintf(outputFile, lineread);

        if (lineread == NULL)
            break;

    }






    return 0;
}