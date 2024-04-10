#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <unistd.h> // For sleep
#include <fcntl.h>  // For O_CREAT, O_EXEC, etc.

typedef struct buffer_item {
    uint8_t data[30];
    uint16_t cksum;
} BUFFER_ITEM;
#define BUFFER_SIZE 10

int insert_item(BUFFER_ITEM item);
int remove_item(BUFFER_ITEM *item);

extern sem_t *empty, *full;

extern pthread_mutex_t mutex;

extern BUFFER_ITEM buffer[BUFFER_SIZE];

extern int insert_position;
extern int remove_position;