/**
 * Example illustrating POSIX named semaphores
 *
 * Compilation (on Linux or macOS):
 *
 *    gcc -lpthread posix-sem.c -o posix-sem -lrt
 *
 * This example includes the appropriate error checking
 * that is not covered in the text.
 *
 * Note: macOS requires linking with the real-time library (-lrt).
 *
 * Operating System Concepts - Ninth Edition
 * John Wiley & Sons - 2013.
 */

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

int main(void)
{
    sem_t *sem;

    // Use sem_open to create a named semaphore
    sem = sem_open("pSem", O_CREAT | O_EXCL, 0664, 1);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    if (sem_wait(sem) != 0)
    {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }

    printf("*** Critical Section *** \n");

    if (sem_post(sem) != 0)
    {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }

    printf("*** Non-Critical Section *** \n");

    if (sem_close(sem) != 0)
    {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("/my_semaphore") != 0)
    {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
