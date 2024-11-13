#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;  
int out = 0; 
int count = 0; // To track how many items are in the buffer

pthread_mutex_t mutex;  // Mutex lock for critical section
pthread_cond_t not_full; // Condition variable for full buffer
pthread_cond_t not_empty; // Condition variable for empty buffer

void* producer(void* arg);
void* consumer(void* arg);
void add_to_buffer(int item);
int remove_from_buffer();
int is_buffer_full();
int is_buffer_empty();
void print_buffer();  // Function to print buffer content

int main() {
    int numProducers, numConsumers;
    printf("Enter number of producers: ");
    scanf("%d", &numProducers);
    printf("Enter number of consumers: ");
    scanf("%d", &numConsumers);
    
    pthread_t producers[numProducers], consumers[numConsumers];

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

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

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}

void* producer(void* arg) {
    int producer_id = (int)(intptr_t)arg;
    while (1) {
        int item = rand() % 100;  // Produce a random item

        pthread_mutex_lock(&mutex);  // Enter critical section

        // Wait if buffer is full
        while (is_buffer_full()) {
            printf("Producer %d: Buffer is full! Waiting...\n", producer_id);
            pthread_cond_wait(&not_full, &mutex);
        }

        add_to_buffer(item);
        printf("Producer %d: Produced item: %d\n", producer_id, item);
        print_buffer();  // Print buffer after producing an item

        // Signal that buffer is not empty
        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&mutex);  // Exit critical section

        sleep(rand() % 2);  // Simulate variable production time
    }
    return NULL;
}

void* consumer(void* arg) {
    int consumer_id = (int)(intptr_t)arg;
    while (1) {
        pthread_mutex_lock(&mutex);  // Enter critical section

        // Wait if buffer is empty
        while (is_buffer_empty()) {
            printf("Consumer %d: Buffer is empty! Waiting...\n", consumer_id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = remove_from_buffer();
        printf("Consumer %d: Consumed item: %d\n", consumer_id, item);
        print_buffer();  // Print buffer after consuming an item

        // Signal that buffer is not full
        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&mutex);  // Exit critical section

        sleep(rand() % 3);  // Simulate variable consumption time
    }
    return NULL;
}

void add_to_buffer(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;  // Increase the count of items in the buffer
}

int remove_from_buffer() {
    int item = buffer[out];
    buffer[out]=0;
    out = (out + 1) % BUFFER_SIZE;
    count--;  // Decrease the count of items in the buffer
    return item;
}

int is_buffer_full() {
    return count == BUFFER_SIZE;
}

int is_buffer_empty() {
    return count == 0;
}

// Function to print buffer contents
void print_buffer() {
    printf("Buffer: [ ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == out && count == 0) { // Case when buffer is empty
            printf("_ ");
        } else {
            printf("%d ", buffer[i]);
        }
    }
    printf("] (in = %d, out = %d, count = %d)\n", in, out, count);
}
