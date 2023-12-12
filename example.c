#include<stdio.h> 
#include"Sparse.h"
#include <time.h> 

int main(){

    sparseMatrix * matrix = NULL;
    matrix = _init_(10,8,10);
    printf("Created the matrix.\n");
    addElement(matrix , 0 , 0 ,21);
    addElement(matrix , 1 , 1, 31);
    addElement(matrix , 2 , 2 , 46);
    addElement(matrix , 3 , 3 , 52);
    addElement(matrix , 4 , 4 , 62);
    addElement(matrix , 5 , 5 , 72);
 
    matrixPrint(matrix);

    srand((unsigned int)time(NULL));

    int **myArray = (int **)malloc(8 * sizeof(int *));

    for (int i = 0; i < 8 ; i++) {
        myArray[i] = (int *)malloc(3 *sizeof(int));
        for (int j = 0; j < 3; j++) {
            myArray[i][j] = 1; 
    }
    clock_t t; 
    t = clock();
    multiply(matrix, 8 , 3, myArray);
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("multiply() took %f seconds to execute \n", time_taken); 
    
    freeMatrix(matrix);
    return 0;
}