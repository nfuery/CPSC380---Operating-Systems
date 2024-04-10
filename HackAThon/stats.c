#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void *average_func(void *arg);
void *min_func(void *arg);
void *max_func(void *arg);
int _argc;
char** _argv;

int main(int argc, char *argv[]) {
    _argc = argc;
    _argv = argv;
    int avg;
    int min;
    int max;
    pthread_t a_thread;
    pthread_t b_thread;
    pthread_t c_thread;
    void *avg_result;
    void *min_result;
    void *max_result;

    int input[argc-1];
    for (int i = 0; i < argc-1; i++) {
        input[i] = atoi(argv[i+1]);
    }
    avg = pthread_create(&a_thread, NULL, average_func, NULL);
    min = pthread_create(&b_thread, NULL, min_func, NULL);
    max = pthread_create(&c_thread, NULL, max_func, NULL);

    if (avg != 0 || min != 0 || max != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    avg = pthread_join(a_thread, &avg_result);
    min = pthread_join(b_thread, &min_result);
    max = pthread_join(c_thread, &max_result);
    if (avg != 0 || min != 0 || max != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("The average value is %d\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);
    exit(EXIT_SUCCESS);
}

void *average_func(void *arg) {
    int avg = 0;
    for (int i = 1; i < _argc; i++) {
        avg += atoi(_argv[i]);
    }
    avg /= sizeof(_argv) / sizeof(_argv[0]);
    pthread_exit("Thank you for the CPU time");
}

void *min_func(void *arg){
    int min = atoi(_argv[0]);
    for (int i = 1; i < _argc; i++) {
        if(atoi(_argv[i]) < min){
            min = atoi(_argv[i]);
        }
    }
    pthread_exit("Thank you for the CPU time");
}

void *max_func(void *arg){
    int max = atoi(_argv[0]);
    for (int i = 1; i < _argc; i++) {
        if(atoi(_argv[i]) > max){
            max = atoi(_argv[i]);
        }
    }
    pthread_exit("Thank you for the CPU time");
}
