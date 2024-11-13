#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;  
int out = 0; 

sem_t empty;      
sem_t full;       
sem_t mutex;      

void* producer(void* arg);
void* consumer(void* arg);
void add_to_buffer(int item);
int remove_from_buffer();
int is_buffer_full();
int is_buffer_empty();
void print_buffer_status();

int main() {
    int numProducers, numConsumers;
    printf("Enter number of producers: ");
    scanf("%d", &numProducers);
    printf("Enter number of consumers: ");
    scanf("%d", &numConsumers);
    
    pthread_t producers[numProducers], consumers[numConsumers];

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  
    sem_init(&full, 0, 0);              
    sem_init(&mutex, 0, 1);            

    // Create producer threads
    for (int i = 0; i < numProducers; i++) {
        pthread_create(&producers[i], NULL, producer, (void*)(intptr_t)i);
    }

    // Create consumer threads
    for (int i = 0; i < numConsumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void*)(intptr_t)i);
    }

    // Wait for producer threads to finish (they won't in this case)
    for (int i = 0; i < numProducers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for consumer threads to finish (they won't in this case)
    for (int i = 0; i < numConsumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}

void* producer(void* arg) {
    int producer_id = (int)(intptr_t)arg;
    while (1) {
        int item = rand() % 100;  // Produce a random item

        sem_wait(&empty);     
        sem_wait(&mutex);     

        add_to_buffer(item);  
        printf("Producer %d: Produced item: %d\n", producer_id, item);

        print_buffer_status(); // Print buffer status after producing

        sem_post(&mutex);    
        sem_post(&full);      

        sleep(rand() % 2);  // Simulate variable production time
    }
    return NULL;
}

void* consumer(void* arg) {
    int consumer_id = (int)(intptr_t)arg;
    while (1) {
        sem_wait(&full);    
        sem_wait(&mutex);   

        int item = remove_from_buffer();  
        printf("Consumer %d: Consumed item: %d\n", consumer_id, item);

        print_buffer_status(); // Print buffer status after consuming

        sem_post(&mutex);   
        sem_post(&empty);   

        sleep(rand() % 3);  // Simulate variable consumption time
    }
    return NULL;
}

void add_to_buffer(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;  
}

int remove_from_buffer() {
    int item = buffer[out];
    buffer[out]=0;
    out = (out + 1) % BUFFER_SIZE;  
    return item;
}

int is_buffer_full() {
    return ((in + 1) % BUFFER_SIZE) == out; // Buffer is full if the next position of 'in' is 'out'
}

int is_buffer_empty() {
    return in == out; // Buffer is empty if 'in' equals 'out'
}

void print_buffer_status() {
    // Print buffer contents
    printf("Buffer status: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf(" %d ", buffer[i]);
    }
    printf("]\n");

    // Print values of in and out
    printf("in = %d, out = %d\n", in, out);
}
