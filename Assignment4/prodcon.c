#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "buffer.h"

extern bool terminate;

void *producer(void *param) {
    BUFFER_ITEM item;
    while (!terminate) {
        sleep(rand()); // Sleep for random duration

        // Populate item data with randoms values
        for (int i = 0; i < 30; i++) {
            item.data[i] = rand() % 256;
        }
        item.cksum = rand(); // Random checksum

        printf("Producer is producing an item with checksum: %u\n", item.cksum);
        if (insert_item(item) != 0) {
            fprintf(stderr, "Producer encountered an error condition.\n");
            break;
        }
    }
    return NULL;
}

void *consumer(void *param) {
    BUFFER_ITEM item;

    while (!terminate) {
        // Removes item and checks if operation worked correctly
        if (remove_item(&item) == -1) {
            fprintf(stderr, "Error consuming an item.\n");
            continue;
        }

        printf("Consumer consumed an item with checksum: %u\n", item.cksum);
    }

    return NULL;
}



