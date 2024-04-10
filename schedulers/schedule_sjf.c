#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Global variables
struct node *head = NULL;
int num_tasks = 0;

// Function to add a new task to the list
void add(char *name, int priority, int burst) {
    struct node *new_task = malloc(sizeof(struct node));
    new_task->task = malloc(sizeof(struct task));
    new_task->task->name = name;
    new_task->task->priority = priority;
    new_task->task->burst = burst;
    new_task->next = NULL;

    if (head == NULL) {
        head = new_task;
    } else {
        struct node *current = head;
        struct node *prev = NULL;

        // Find the appropriate position to insert the new task
        while (current != NULL && current->task->burst <= burst) {
            prev = current;
            current = current->next;
        }

        if (prev == NULL) {
            new_task->next = head;
            head = new_task;
        } else {
            prev->next = new_task;
            new_task->next = current;
        }
    }

    num_tasks++;
}

// Function to invoke the scheduler
void schedule() {
    struct node *current_task = head;
    int total_turnaround_time = 0;
    int total_response_time = 0;
    int total_wait_time = 0;

    // Iterate through the task list
    while (current_task != NULL) {
        Task *task = current_task->task;

        // Update response time for non-first tasks
        if (num_tasks > 0) {
            total_response_time += total_turnaround_time; // Response time is total turnaround time of previous tasks
        }

        // Run the task
        run(task, task->burst);

        // Update total times
        total_turnaround_time += task->burst;
        total_wait_time += total_turnaround_time - task->burst;

        // Move to the next task
        current_task = current_task->next;
    }

    // Calculate average times
    float avg_turnaround_time = (float)total_turnaround_time / num_tasks;
    float avg_response_time = (float)total_response_time / num_tasks;
    float avg_wait_time = (float)total_wait_time / num_tasks;

    // Print results
    printf("The average turnaround time is: %f time units\n", avg_turnaround_time);
    printf("The average response time is: %f time units\n", avg_response_time);
    printf("The average wait time is: %f time units\n", avg_wait_time);
}
