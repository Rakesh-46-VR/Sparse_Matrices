#include <stdlib.h>
#include <stdio.h>

/*
Triplet form of representation of a sparse matrix :-
Sparse element structure containing 3 integer members - row, column and value.
*/
typedef struct
{
    int row;
    int col;
    int value;
} sparseElement;

/*
Sparse matrix structure containing the rows and cols as the size of the sparse matrix.
count - is used to set the maximum number of <row col value> triplet.
current_count - Keeps the track of the current number of the <row col value> triplet present in the Mat.
Mat is a structue pointer which will be refered further to a array of its kind in the _init_() function 
and its maximum size is equal to count.
*/
typedef struct
{
    int rows;
    int cols;
    int count;
    int current_count;
    sparseElement *Mat;
} sparseMatrix;

/*
Function prototypes :
*/
sparseMatrix *_init_(int, int, int);
void addElement(sparseMatrix *, int, int, int);
void removeElement(sparseMatrix *, int, int);
void matrixPrint(sparseMatrix *);
sparseMatrix *transpose(sparseMatrix *);
void multiply(sparseMatrix *, int, int, int *[]);
int isOrdered(sparseMatrix *);
int start(sparseMatrix *, int);
int end(sparseMatrix *, int);

//Initializes a basic sparseMatrix (No elements are added by default, elements can be added using the addElement() function): 
sparseMatrix *_init_(int row, int col, int count)
{
    if (count >= (row * col) / 2)
    {
        printf("Matrix is not sparse");
        return NULL;
    }
    sparseMatrix *newMatrix = (sparseMatrix *)malloc(sizeof(sparseMatrix));
    if (newMatrix == NULL)
        return NULL;

    newMatrix->rows = row;
    newMatrix->cols = col;
    newMatrix->count = count;
    newMatrix->current_count = 0;
    newMatrix->Mat = (sparseElement *)malloc((unsigned int)count * sizeof(sparseElement));

    if (newMatrix->Mat == NULL)
        return NULL;
    return newMatrix;
}


//Adds a element to the array of sparseElement of the sparseMatrix :-
void addElement(sparseMatrix *Matrix, int row, int col, int value)
{
    if (Matrix != NULL)
    {
        if (Matrix->current_count < Matrix->count && row < Matrix->rows && col < Matrix->cols)
        {
            Matrix->Mat[Matrix->current_count].row = row;
            Matrix->Mat[Matrix->current_count].col = col;
            Matrix->Mat[Matrix->current_count].value = value;
            Matrix->current_count++;
        }
        else
        {
            printf("Index out of range\n");
        }
    }
}

//Removes a element from the array of sparseElement of the sparseMatrix :-
void removeElement(sparseMatrix *Matrix, int row, int col)
{
    if (Matrix != NULL)
    {
        if (row < Matrix->rows && col < Matrix->cols)
        {
            int i;
            for (i = 0; i < Matrix->current_count; i++)
            {
                if (Matrix->Mat[i].row == row && Matrix->Mat[i].col == col)
                {
                    break;
                }
            }
            if (i != Matrix->current_count)
            {
                Matrix->Mat[i] = Matrix->Mat[Matrix->current_count - 1];
                Matrix->current_count--;
            }
            else
            {
                printf("Element not found\n");
            }
        }
        else
        {
            printf("Index out of range\n");
        }
    }
}

//Prints a sparseMatrix :-
void matrixPrint(sparseMatrix *Matrix)
{
    if (Matrix != NULL)
    {
        printf("Row\tCol\tValue\n");
        for (int i = 0; i < Matrix->current_count; i++)
        {
            printf("(%d\t%d\t%d)\n", Matrix->Mat[i].row, Matrix->Mat[i].col, Matrix->Mat[i].value);
        }
    }
}

//Returns a new transposed sparseMatrix:-
sparseMatrix *transpose(sparseMatrix *Matrix)
{
    if (Matrix != NULL)
    {
        sparseMatrix *tMat = NULL;
        tMat = _init_(Matrix->cols, Matrix->rows, Matrix->current_count);
        if (tMat != NULL)
        {
            int k = 0;
            for (int i = 0; i < Matrix->cols; i++)
            {
                for (int j = 0; j < Matrix->current_count; j++)
                {
                    if (Matrix->Mat[j].col == i)
                    {
                        tMat->Mat[k].col = Matrix->Mat[j].row;
                        tMat->Mat[k].row = Matrix->Mat[j].col;
                        tMat->Mat[k].value = Matrix->Mat[j].value;
                        tMat->current_count++;
                        k++;
                    }
                }
            }
            return tMat;
        }
    }
    return NULL;
}


//Multiplies sparse matrix with a dense matrix and currently it doesnot return any thing :-
void multiply(sparseMatrix *spMatrix, int row, int col, int * Matrix[col])
{
    if (spMatrix->cols == row && spMatrix->current_count != 0)
    {

        // int trans[col][row];
        int ** trans = (int **)malloc((unsigned int)col * sizeof(int *));
        int ** newMat = (int **)malloc((unsigned int)row * sizeof(int *));
        for(int i=0;i<col;i++){
            trans[i] = (int *)malloc((unsigned int)row * sizeof(int));
        }
        for(int i=0;i<row;i++){
            newMat[i] = (int *)malloc((unsigned int)col * sizeof(int));
        }

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                trans[j][i] = Matrix[i][j];
            }
        }

        int sum = 0;
        if (isOrdered(spMatrix))
        {
            for (int i = 0; i < spMatrix->rows; i++)
            {
                int st = start(spMatrix, i);
                int en = end(spMatrix, i);
                if (st != -1)
                {
                    for (int k = 0; k < col; k++)
                    {
                        for (int s = st; s < en; s++)
                        {
                            sum += spMatrix->Mat[s].value * trans[k][spMatrix->Mat[s].col];
                        }
                        newMat[spMatrix->Mat[st].row][k] = sum;
                        sum = 0;
                    }
                }
                else
                {
                    for (int l = 0; l < col; l++)
                    {
                        newMat[i][l] = 0;
                    }
                }
            }
        }
        free(trans);
        free(newMat);
    }
}

//To check whether the sparseMatrix is arranged as per the rows (ascending) :-
int isOrdered(sparseMatrix *Matrix)
{
    for (int i = 0; i < (Matrix->current_count - 1); i++)
    {
        if (Matrix->Mat[i].row > Matrix->Mat[i + 1].row)
        {
            return 0;
        }
    }
    return 1;
}

/*
Utility functions :-
start  - It returns the index of the first occuring row in the sparseMatrix
end - It returns the index of the last occuring row in the sparseMatrix
free - Used to free a sparseMatrix
*/

int start(sparseMatrix *spMatrix, int row)
{
    for (int i = 0; i < spMatrix->current_count; i++)
    {
        if (spMatrix->Mat[i].row == row)
        {
            return i;
        }
    }
    return -1;
}

int end(sparseMatrix *spMatrix, int row)
{
    int res = -1;
    for (int i = 0; i < spMatrix->current_count; i++)
    {
        if (spMatrix->Mat[i].row == row)
        {
            for (int j = i; j < spMatrix->current_count; j++)
            {
                res = j;
                if (spMatrix->Mat[j].row != row)
                {
                    return j;
                }
            }
            return res + 1;
        }
    }
    return res;
}

void freeMatrix(sparseMatrix *Matrix)
{
    if (Matrix != NULL)
    {
        free(Matrix->Mat);
        free(Matrix);
    }
}
