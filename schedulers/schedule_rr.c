#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Global variables
struct node *head = NULL;
struct node *last = NULL;
int num = 0;

void add(char *name, int priority, int burst) {
    // Increment the total number of tasks
    num++;

    // Create a new node for the task
    struct node *new_node = malloc(sizeof(struct node));
    new_node->task = malloc(sizeof(struct task));
    new_node->task->name = name;
    new_node->task->burst = burst;
    new_node->task->priority = priority;
    new_node->next = NULL;

    // If the list is empty, set the new node as the head and last node
    if (head == NULL) {
        head = new_node;
        last = new_node;
    } else {
        // If the list is not empty, append the new node to the end
        last->next = new_node;
        last = new_node;
    }
}

void schedule() {
    struct node *current = head;
    int total_turnaround_time = 0;
    int total_response_time = 0;
    int total_wait_time = 0;
    int num_tasks = 0;

    // Iterate through the task list
    while (current != NULL) {
        Task *task = current->task;

        // Update response time for non-first tasks
        if (num_tasks > 0) {
            total_response_time += total_turnaround_time - task->burst; // Response time is total turnaround time - burst time of current task
        }

        // Run the task
        run(task, task->burst);

        // Update total times
        total_turnaround_time += task->burst;
        total_wait_time += total_turnaround_time - task->burst;

        // Move to the next task
        current = current->next;
        num_tasks++;
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


