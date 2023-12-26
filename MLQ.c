#include <stdio.h>
#include <stdlib.h>

// Process structure
typedef struct Process {
    int process_id;
    int burst_time;
    int remaining_burst_time; // To keep track of the remaining burst time
    int arrival_time;
    int priority; // Priority level (1 for high, 2 for medium, 3 for low)
    struct Process* next;
} Process;

// Linked List Node structure
typedef struct LinkedListNode {
    Process* data;
    struct LinkedListNode* next;
} LinkedListNode;

// Function to create a new process node
Process* createProcess(int process_id, int burst_time, int arrival_time, int priority) {
    Process* newProcess = (Process*)malloc(sizeof(Process));
    newProcess->process_id = process_id;
    newProcess->burst_time = burst_time;
    newProcess->remaining_burst_time = burst_time;
    newProcess->arrival_time = arrival_time;
    newProcess->priority = priority;
    newProcess->next = NULL;
    return newProcess;
}

// Function to create a new linked list node
LinkedListNode* createNode(Process* data) {
    LinkedListNode* newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to append a process to the end of the linked list
void appendProcess(LinkedListNode** head, Process* process) {
    LinkedListNode* newNode = createNode(process);

    if (*head == NULL) {
        *head = newNode;
    } else {
        LinkedListNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to perform Round Robin scheduling for a specific queue
void roundRobinScheduling(LinkedListNode** head, int time_quantum) {
    if (*head == NULL) {
        return;
    }

    LinkedListNode* current = *head;

    while (current != NULL) {
        Process* currentProcess = current->data;

        // Simulate execution of the process for the time quantum
        int time_executed = (currentProcess->remaining_burst_time < time_quantum) ? currentProcess->remaining_burst_time : time_quantum;

        printf("Executing process %d from Queue %d for time quantum %d ms.\n", currentProcess->process_id, currentProcess->priority, time_executed);

        // Update remaining burst time
        currentProcess->remaining_burst_time -= time_executed;

        // If the process is not finished, move it to the end of the queue
        if (currentProcess->remaining_burst_time > 0) {
            printf("Process %d from Queue %d not finished. Moving to the end of the queue.\n", currentProcess->process_id, currentProcess->priority);
            appendProcess(head, createProcess(currentProcess->process_id, currentProcess->remaining_burst_time, 0, currentProcess->priority));
        }

        // Move to the next process in the queue
        LinkedListNode* nextNode = current->next;
        current = nextNode;
    }
}

// Function to free the memory allocated for the linked list
void freeLinkedList(LinkedListNode** head) {
    LinkedListNode* current = *head;
    while (current != NULL) {
        LinkedListNode* temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    *head = NULL;
}

// Function to perform First In First Out (FIFO) scheduling for a specific queue
void fifoScheduling(LinkedListNode** head) {
    if (*head == NULL) {
        return;
    }

    LinkedListNode* current = *head;
    Process* currentProcess = current->data;

    printf("Executing process %d from Queue %d using First In First Out (FIFO) scheduling.\n", currentProcess->process_id, currentProcess->priority);

    // Move to the next process in the queue
    LinkedListNode* nextNode = current->next;
    free(current->data);
    free(current);
    *head = nextNode;
}

// Function to perform scheduling for the lower-priority queue
void lowerPriorityScheduling(LinkedListNode** head) {
    while (*head != NULL) {
        fifoScheduling(head);
    }
}


int main() {
    // Example processes with burst times, arrival times, and priorities
    Process* process1 = createProcess(1, 4, 0, 1); // Priority 1 (high)
    Process* process2 = createProcess(2, 6, 1, 2); // Priority 2 (medium)
    Process* process3 = createProcess(3, 5, 2, 1); // Priority 1 (high)
    Process* process4 = createProcess(4, 3, 3, 2); // Priority 2 (medium)
    Process* process5 = createProcess(5, 7, 4, 3); // Priority 3 (low)
    Process* process6 = createProcess(6, 10, 5, 3); // Priority 3 (low)

    // Create three linked lists for three priority queues
    LinkedListNode* highPriorityQueue = NULL;
    LinkedListNode* mediumPriorityQueue = NULL;
    LinkedListNode* lowPriorityQueue = NULL;

    // Append processes to their respective priority queues
    appendProcess(&highPriorityQueue, process1);
    appendProcess(&mediumPriorityQueue, process2);
    appendProcess(&highPriorityQueue, process3);
    appendProcess(&mediumPriorityQueue, process4);
    appendProcess(&lowPriorityQueue, process5);
    appendProcess(&lowPriorityQueue, process6);

    // Set the time quantum to 2 milliseconds
    int time_quantum = 2;

    // Perform Round Robin scheduling for high-priority queue
    roundRobinScheduling(&highPriorityQueue, time_quantum);

    // Perform Round Robin scheduling for medium-priority queue
    roundRobinScheduling(&mediumPriorityQueue, time_quantum);

    // Perform FIFO scheduling for low-priority queue
    //fifoScheduling(&lowPriorityQueue);
    lowerPriorityScheduling(&lowPriorityQueue);


    // Free the allocated memory for the linked lists
    freeLinkedList(&highPriorityQueue);
    freeLinkedList(&mediumPriorityQueue);
    freeLinkedList(&lowPriorityQueue);

    return 0;
}
