#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Global variable for the head of the linked list
struct node *head = NULL;

// Function to add a new task to the priority-based scheduler
void add(char *name, int priority, int burst) {
    // Create a new node and allocate memory for it
    struct node *new_node = malloc(sizeof(struct node));
    // Allocate memory for the task associated with the node
    new_node->task = malloc(sizeof(struct task));
    // Assign task properties
    new_node->task->name = name;
    new_node->task->burst = burst;
    new_node->task->priority = priority;
    new_node->next = NULL;

    // Check if the list is empty or if the new task has higher priority than the head
    if (!head || priority > head->task->priority) {
        // If so, insert the new node at the beginning
        new_node->next = head;
        head = new_node;
    } else {
        // Otherwise, find the appropriate position in the list based on priority
        struct node *temp = head;
        while (temp->next && priority <= temp->next->task->priority) {
            temp = temp->next;
        }
        // Insert the new node after the current position
        new_node->next = temp->next;
        temp->next = new_node;
    }
}

// Function to invoke the priority-based scheduler
void schedule() {
    int num_tasks = 0;
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    struct node *temp = head;

    // Traverse the list and schedule each task
    while (temp != NULL) {
        num_tasks++;
        run(temp->task, temp->task->burst);
        // Update turnaround time with the burst time of the current task
        total_turnaround_time += temp->task->burst;
        // Update waiting time with the cumulative turnaround time
        total_waiting_time += total_turnaround_time;
        // Update response time with the cumulative turnaround time only if there are more than one tasks
        if (num_tasks > 1) {
            total_response_time += total_turnaround_time;
        }
        // Move to the next task in the list
        temp = temp->next;
    }

    // Calculate and display average turnaround, waiting, and response times
    printf("Average turnaround time: %.2f\n", total_turnaround_time / num_tasks);
    printf("Average waiting time: %.2f\n", total_waiting_time / num_tasks);
    printf("Average response time: %.2f\n", total_response_time / num_tasks);
}