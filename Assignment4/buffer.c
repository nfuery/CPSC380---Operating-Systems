#include "buffer.h"

BUFFER_ITEM buffer[BUFFER_SIZE];
sem_t *empty, *full;
pthread_mutex_t mutex;

int insert_position = 0;
int remove_position = 0;

bool terminate = false;

void *producer(void *param);
void *consumer(void *param);

int insert_item(BUFFER_ITEM item) {
    if (sem_wait(empty) != 0) {
        return -1; // Failed to wait on semaphore
    }
    if (pthread_mutex_lock(&mutex) != 0) {
        return -1; // Failed to lock mutex
    }

    // Insert the item into the buffer
    buffer[insert_position] = item;
    insert_position = (insert_position + 1) % BUFFER_SIZE;

    if (pthread_mutex_unlock(&mutex) != 0) {
        return -1; // Failed to unlock mutex
    }
    if (sem_post(full) != 0) {
        return -1; // Failed to post to semaphore
    }

    return 0; // Success
}

int remove_item(BUFFER_ITEM *item) {
    if (sem_wait(full) != 0) {
        return -1; // Failed to wait on semaphore
    }
    if (pthread_mutex_lock(&mutex) != 0) {
        return -1; // Failed to lock mutex
    }

    // Remove item from the buffer at the current remove position
    *item = buffer[remove_position];
    remove_position = (remove_position + 1) % BUFFER_SIZE;

    if (pthread_mutex_unlock(&mutex) != 0) {
        return -1; // Failed to unlock mutex
    }
    if (sem_post(empty) != 0) {
        return -1; // Failed to post to semaphore
    }

    return 0; // Success
}

int main(int argc, char *argv[]) {
    // Clean up previous runs
    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");

    // Getting input from user
    if (argc != 4) {
        fprintf(stderr, "Run with this format: ./prodcon <delay> <#producer threads> <#consumer threads>\n");
        return 1;
    }

    int delay = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    // Initialize semaphores and mutex lock
    empty = sem_open("/empty_sem", O_CREAT, 0644, BUFFER_SIZE);
    full = sem_open("/full_sem", O_CREAT, 0644, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    for (int i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Sleep for delay
    sleep(delay);

    terminate = true;

    // Joining producer threads 
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Joining consumer threads 
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Cleanup and exit
    sem_close(empty);
    sem_close(full);
    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");
    pthread_mutex_destroy(&mutex);

    printf("Program terminated after %u seconds\n", delay);
    return 0;
}
