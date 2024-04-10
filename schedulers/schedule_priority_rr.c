#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;

void add(char *name, int priority, int burst) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->task = malloc(sizeof(struct task));
    new_node->task->name = name;
    new_node->task->priority = priority;
    new_node->task->burst = burst;
    new_node->next = NULL;

    if (!head) {
        head = new_node;
        return;
    }

    struct node *prev = NULL;
    struct node *current = head;

    while (current && current->task->priority >= priority) {
        prev = current;
        current = current->next;
    }

    if (!prev) { // Insert at the beginning
        new_node->next = head;
        head = new_node;
    } else { // Insert in the middle or end
        prev->next = new_node;
        new_node->next = current;
    }
}

void schedule() {
    struct node *current = head;
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    int num_tasks = 0;

    // Iterate over each task in the list
    while (current != NULL) {
        num_tasks++;

        // Calculate response time
        total_response_time += total_waiting_time;

        // Run the task
        run(current->task, current->task->burst);

        // Calculate turnaround time
        total_turnaround_time += current->task->burst;

        // Update waiting time for the next tasks
        total_waiting_time += current->task->burst;

        // Move to the next task
        current = current->next;
    }

    // Calculate average turnaround time
    float avg_turnaround_time = total_turnaround_time / num_tasks;

    // Calculate average waiting time
    // Since we're using round-robin, waiting time is the same as the turnaround time
    float avg_waiting_time = total_waiting_time / num_tasks;

    // Calculate average response time
    float avg_response_time = total_response_time / num_tasks;

    // Output results
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("Average waiting time: %.2f\n", avg_waiting_time);
    printf("Average response time: %.2f\n", avg_response_time);
}
