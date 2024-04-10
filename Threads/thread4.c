#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 10

int npoints;
int hit_count = 0;  // Shared hit count
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex to protect hit_count

void *pointCheck(void *arg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_points>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    npoints = atoi(argv[1]);
    double pi;

    pthread_t workers[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&workers[i], NULL, pointCheck, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(workers[i], NULL);

    pi = (double)hit_count / (npoints * NUM_THREADS) * 4.0;
    printf("Estimated value of π: %f\n", pi);

    exit(EXIT_SUCCESS);
}

double random_double() {
    return random() / ((double)RAND_MAX + 1);
}

void *pointCheck(void *arg) {
    for (int i = 0; i < npoints; i++) {
        double x = random_double() * 2.0 - 1.0;
        double y = random_double() * 2.0 - 1.0;

        if (sqrt(x * x + y * y) < 1.0) {
            // Acquire the mutex before updating hit_count
            pthread_mutex_lock(&mutex);
            hit_count++;
            // Release the mutex after updating hit_count
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}
