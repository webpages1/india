#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100 

int matA[MAX][MAX], matB[MAX][MAX], matMul[MAX][MAX], matAdd[MAX][MAX], matSub[MAX][MAX];
int row1, col1, row2, col2;

void *multiply(void *arg) {
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            matMul[i][j] = 0;
            for (int k = 0; k < col1; k++) {
                matMul[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

void *add(void *arg) {
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col1; j++) {
            matAdd[i][j] = matA[i][j] + matB[i][j];
        }
    }
    pthread_exit(NULL);
}

void *subtract(void *arg) {
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col1; j++) {
            matSub[i][j] = matA[i][j] - matB[i][j];
        }
    }
    pthread_exit(NULL);
}

int main() {
    printf("Enter rows and columns of matrix A: ");
    scanf("%d%d", &row1, &col1);
    
    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col1; j++) {
            scanf("%d", &matA[i][j]);
        }
    }

    printf("Enter rows and columns of matrix B: ");
    scanf("%d%d", &row2, &col2);

    if (col1 != row2) {
        printf("Error: For multiplication, matrix dimensions do not match.\n");
        return -1;
    }

    printf("Enter elements of matrix B:\n");
    for (int i = 0; i < row2; i++) {
        for (int j = 0; j < col2; j++) {
            scanf("%d", &matB[i][j]);
        }
    }

    if (row1 != row2 || col1 != col2) {
        printf("Error: For addition and subtraction, matrices must have the same dimensions.\n");
        return -1;
    }

    pthread_t thread_mul, thread_add, thread_sub;

    if (pthread_create(&thread_mul, NULL, multiply, NULL)) {
        printf("Error: Unable to create multiplication thread\n");
        return -1;
    }

    if (pthread_create(&thread_add, NULL, add, NULL)) {
        printf("Error: Unable to create addition thread\n");
        return -1;
    }

    if (pthread_create(&thread_sub, NULL, subtract, NULL)) {
        printf("Error: Unable to create subtraction thread\n");
        return -1;
    }

    pthread_join(thread_mul, NULL);
    pthread_join(thread_add, NULL);
    pthread_join(thread_sub, NULL);

    printf("Resultant matrix after multiplication (Matrix A * Matrix B):\n");
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            printf("%d ", matMul[i][j]);
        }
        printf("\n");
    }

    printf("Resultant matrix after addition (Matrix A + Matrix B):\n");
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col1; j++) {
            printf("%d ", matAdd[i][j]);
        }
        printf("\n");
    }

    printf("Resultant matrix after subtraction (Matrix A - Matrix B):\n");
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col1; j++) {
            printf("%d ", matSub[i][j]);
        }
        printf("\n");
    }

    return 0;
}

