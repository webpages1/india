#include <stdio.h>
#include <stdlib.h>

typedef struct PCB {
    int pid, priority, AT, BT, FT, WT, TT;
} PCB;

typedef struct Node {
    PCB* process;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

void enqueue(Queue* queue, PCB* process) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->process = process;
    newNode->next = NULL;
    if (queue->rear) {
        queue->rear->next = newNode;
    }
    queue->rear = newNode;
    if (!queue->front) {
        queue->front = newNode;
    }
}

PCB* dequeue(Queue* queue) {
    if (!queue->front) return NULL;
    Node* temp = queue->front;
    PCB* process = temp->process;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL;
    }
    free(temp);
    return process;
}

int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

void startUserExecution(PCB* processes, int* copyOfBT, int totalProcesses, int timeQuantum) {
    Queue queue = {NULL, NULL};
    int completed = 0, time = 0;

    while (completed < totalProcesses) {
        for (int i = 0; i < totalProcesses; i++) {
            PCB* currentProcess = &processes[i];
            if (currentProcess->AT <= time && currentProcess->BT > 0) {
                Node* temp = queue.front;
                int found = 0;
                while (temp) {
                    if (temp->process == currentProcess) {
                        found = 1;
                        break;
                    }
                    temp = temp->next;
                }
                if (!found) {
                    enqueue(&queue, currentProcess);
                }
            }
        }
        
        if (isQueueEmpty(&queue)) {
            time++;
        } else {
            PCB* currentProcess = dequeue(&queue);
            int executionTime = currentProcess->BT < timeQuantum ? currentProcess->BT : timeQuantum;
            int temp = time + executionTime;

            while (time < temp) {
                currentProcess->BT--;
                time++;
                for (int i = 0; i < totalProcesses; i++) {
                    PCB* process = &processes[i];
                    if (process->AT <= time && process->BT > 0 && process != currentProcess) {
                        Node* temp = queue.front;
                        int found = 0;
                        while (temp) {
                            if (temp->process == process) {
                                found = 1;
                                break;
                            }
                            temp = temp->next;
                        }
                        if (!found) {
                            enqueue(&queue, process);
                        }
                    }
                }
            }
            
            if (currentProcess->BT == 0) {
                currentProcess->FT = time;
                currentProcess->TT = currentProcess->FT - currentProcess->AT;
                completed++;
            } else {
                enqueue(&queue, currentProcess);
            }
        }
    }

    for (int i = 0; i < totalProcesses; i++) {
        processes[i].BT = copyOfBT[i];
        processes[i].WT = processes[i].TT - processes[i].BT;
    }
}

int main() {
    int totalNumberOfProcesses;
    printf("Enter total number of processes:\n");
    scanf("%d", &totalNumberOfProcesses);

    PCB* processes = (PCB*)malloc(totalNumberOfProcesses * sizeof(PCB));
    int* copyOfBT = (int*)malloc(totalNumberOfProcesses * sizeof(int));

    for (int i = 0; i < totalNumberOfProcesses; i++) {
        processes[i].pid = i + 1;
        printf("Enter priority of process with pid %d:\n", processes[i].pid);
        scanf("%d", &processes[i].priority);
        printf("Enter Arrival Time of process with pid %d:\n", processes[i].pid);
        scanf("%d", &processes[i].AT);
        printf("Enter Burst Time of process with pid %d:\n", processes[i].pid);
        scanf("%d", &processes[i].BT);
        processes[i].FT = processes[i].WT = processes[i].TT = 0;
        copyOfBT[i] = processes[i].BT;
    }

    int timeQuantum;
    printf("Enter time quantum:\n");
    scanf("%d", &timeQuantum);

    startUserExecution(processes, copyOfBT, totalNumberOfProcesses, timeQuantum);

    printf("\nProcess details after execution:\n");
    for (int i = 0; i < totalNumberOfProcesses; i++) {
        printf("[pid:%d, priority:%d, AT:%d, BT:%d, FT:%d, TT:%d, WT:%d]\n",
               processes[i].pid, processes[i].priority, processes[i].AT, copyOfBT[i],
               processes[i].FT, processes[i].TT, processes[i].WT);
    }

    free(processes);
    free(copyOfBT);
    return 0;
}
