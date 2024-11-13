#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;      
sem_t writeLock;  
int readers = 0;  
int data = 0;     

void* reader(void* arg);
void* writer(void* arg);
void enter_reader(int reader_id);
void exit_reader(int reader_id);
void enter_writer(int writer_id);
void exit_writer(int writer_id);

int main() {
    int readerNum, writerNum;
    printf("Enter number of readers: ");
    scanf("%d", &readerNum);
    printf("Enter number of writers: ");
    scanf("%d", &writerNum);

    if (readerNum < 0 || writerNum < 0) {
        printf("Number of readers and writers cannot be less than 0\n");
        return 0;
    }

    pthread_t r_threads[readerNum], w_threads[writerNum];                                                               

    sem_init(&mutex, 0, 1);
    sem_init(&writeLock, 0, 1);

    for (int i = 0; i < readerNum; i++) {
        pthread_create(&r_threads[i], NULL, reader, (void*)(size_t)(i + 1));
    }

    for (int i = 0; i < writerNum; i++) {
        pthread_create(&w_threads[i], NULL, writer, (void*)(size_t)(i + 1));
    }

    for (int i = 0; i < readerNum; i++) {
        pthread_join(r_threads[i], NULL);
    }

    for (int i = 0; i < writerNum; i++) {
        pthread_join(w_threads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&writeLock);

    return 0;
}

void* reader(void* arg) {
    int reader_id = (int)(size_t)arg;
    while (1) {
        sleep(rand() % 5); 
        enter_reader(reader_id);

        printf("Reader %d: Reading data: %d\n", reader_id, data);

        exit_reader(reader_id);
    }
    return NULL;
}

void* writer(void* arg) {
    int writer_id = (int)(size_t)arg;
    while (1) {
        sleep(rand() % 5); 
        enter_writer(writer_id);

        data = rand() % 100;  
        printf("Writer %d: Writing data: %d\n", writer_id, data);

        exit_writer(writer_id);
    }
    return NULL;
}

void enter_reader(int reader_id) {
    sem_wait(&mutex);  
    readers++;
    if (readers == 1) {
        sem_wait(&writeLock);
    }
    printf("Reader %d is entering the critical section.\n", reader_id);
    sem_post(&mutex);  
}

void exit_reader(int reader_id) {
    sem_wait(&mutex); 
    printf("Reader %d is exiting the critical section.\n", reader_id);
    readers--;
    if (readers == 0) {
        sem_post(&writeLock);
    }
    sem_post(&mutex);  
}

void enter_writer(int writer_id) {
    sem_wait(&writeLock); 
    printf("Writer %d is entering the critical section.\n", writer_id);
}

void exit_writer(int writer_id) {
    printf("Writer %d is exiting the critical section.\n", writer_id);
    sem_post(&writeLock);  
}
