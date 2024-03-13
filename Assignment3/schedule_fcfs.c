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
int num_tasks = 0;

// Function to add a task to the list
void add(char *name, int priority, int burst) {
    // Allocate memory for the new node
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the task
    new_node->task = malloc(sizeof(struct task));
    if (new_node->task == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node); // Free previously allocated memory
        exit(EXIT_FAILURE);
    }

    // Set task properties
    new_node->task->name = strdup(name);
    if (new_node->task->name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node->task); // Free previously allocated memory
        free(new_node);
        exit(EXIT_FAILURE);
    }
    new_node->task->priority = priority;
    new_node->task->burst = burst;
    new_node->next = NULL;

    // Add the new node to the list
    if (head == NULL) {
        head = new_node;
        last = new_node;
    } else {
        last->next = new_node;
        last = new_node;
    }
    num_tasks++;
}

// Function to invoke the scheduler
void schedule() {
    if (head == NULL) {
        printf("No tasks to schedule\n");
        return;
    }

    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;

    struct node *current_task = head;

    // Iterate through the list of tasks
    while (current_task != NULL) {
        // Run the current task
        run(current_task->task, current_task->task->burst);

        // Update turnaround time
        total_turnaround_time += current_task->task->burst;
        // Update waiting time
        total_waiting_time += total_turnaround_time - current_task->task->burst;
        // Update response time (excluding the first task)
        if (current_task != head) {
            total_response_time += total_turnaround_time - current_task->task->burst;
        }

        // Move to the next task
        current_task = current_task->next;
    }

    // Calculate averages
    float avg_turnaround_time = (float)total_turnaround_time / num_tasks;
    float avg_waiting_time = (float)total_waiting_time / num_tasks;
    float avg_response_time = (float)total_response_time / (num_tasks - 1);

    // Print averages
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Response Time: %.2f\n", avg_response_time);
}
