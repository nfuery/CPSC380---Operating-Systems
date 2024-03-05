#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 2

int npoints;
int hit_count = 0; // Shared hit count
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex to protect hit_count
void *pointCheck(void *arg); // Declaration of pointCheck function

int main(int argc, char *argv[]) {
    // Error handling for incorrect command line argument
    if (argc != 2) {
        fprintf(stderr, "Invalid entry");
        exit(EXIT_FAILURE);
    }

    npoints = atoi(argv[1]); // Takes input and assigns to npoints
    double pi; // Declaring pi 

    pthread_t threads[NUM_THREADS]; // Array of threads with size of NUM_THREADS

    // Loops through NUM_THREADS, in each iteration creates a thread that executes pointCheck
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, pointCheck, NULL);

    // Loops through threads and joins them into parent thread
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    pi = (double)hit_count / (npoints * NUM_THREADS) * 4.0; // Calculates value of pi with hit_count, npoints, and NUM_THREADS
    printf("Estimated value of pi: %f\n", pi);

    exit(EXIT_SUCCESS);
}

// Returns a random double
double randomDouble() {
    return random() / ((double)RAND_MAX + 1);
}

// Loops through npoints and chooses a random point in each iteration
// Determines if point lies in circle then adds to hit_count if so
void *pointCheck(void *arg) {
    for (int i = 0; i < npoints; i++) {
        double x = randomDouble() * 2.0 - 1.0;
        double y = randomDouble() * 2.0 - 1.0;

        if (sqrt(x * x + y * y) < 1.0) {
            pthread_mutex_lock(&mutex); // Acquire the mutex before updating hit_count
            hit_count++;
            pthread_mutex_unlock(&mutex); // Release the mutex after updating hit_count
        }
    }
    pthread_exit(NULL);
}
