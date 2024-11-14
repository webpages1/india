#include <stdio.h>

typedef struct {
    int arrivalTime, burstTime, remainingTime, finishTime, turnAroundTime, waitingTime, priority, completed;
} Process;

// Function for Priority Non-Preemptive Scheduling
void calculatePriorityNonPreemptive(Process p[], int n) {
    int completed = 0, currentTime = 0;

    while (completed < n) {
        int highestPriorityIndex = -1;

        // Find the process with the highest priority in the ready queue
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrivalTime <= currentTime) {
                if (highestPriorityIndex == -1 || p[i].priority < p[highestPriorityIndex].priority) {
                    highestPriorityIndex = i;
                }
            }
        }

        // If no process is ready, increment the current time
        if (highestPriorityIndex == -1) {
            currentTime++;
        } else {
            // Execute the selected process until it completes
            p[highestPriorityIndex].finishTime = currentTime + p[highestPriorityIndex].burstTime;
            p[highestPriorityIndex].turnAroundTime = p[highestPriorityIndex].finishTime - p[highestPriorityIndex].arrivalTime;
            p[highestPriorityIndex].waitingTime = p[highestPriorityIndex].turnAroundTime - p[highestPriorityIndex].burstTime;
            p[highestPriorityIndex].completed = 1;
            currentTime = p[highestPriorityIndex].finishTime;
            completed++;
        }
    }
}

// Function for Priority Preemptive Scheduling
void calculatePriorityPreemptive(Process p[], int n) {
    int completed = 0, currentTime = 0, currentProcess = -1;

    while (completed < n) {
        int highestPriorityIndex = -1;

        // Find the highest priority process in the ready queue
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {
                if (highestPriorityIndex == -1 || p[i].priority < p[highestPriorityIndex].priority) {
                    highestPriorityIndex = i;
                }
            }
        }

        if (highestPriorityIndex != -1) {
            // Only switch if new process has a higher priority
            if (currentProcess == -1 || p[highestPriorityIndex].priority < p[currentProcess].priority) {
                currentProcess = highestPriorityIndex;
            }

            // Execute the current process for 1 time unit
            p[currentProcess].remainingTime--;
            currentTime++;

            // If the process finishes, update finish time and mark it completed
            if (p[currentProcess].remainingTime == 0) {
                p[currentProcess].finishTime = currentTime;
                p[currentProcess].turnAroundTime = p[currentProcess].finishTime - p[currentProcess].arrivalTime;
                p[currentProcess].waitingTime = p[currentProcess].turnAroundTime - p[currentProcess].burstTime;
                completed++;
                currentProcess = -1; // Reset the current process to allow for priority comparison
            }
        } else {
            // No process is ready, so increment the current time
            currentTime++;
        }
    }
}

// Function to display the results table
void displayTable(Process p[], int n) {
    printf("\nPID\tArrival\tBurst\tPriority\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t\t%d\n", i + 1, p[i].arrivalTime, p[i].burstTime, p[i].priority, p[i].finishTime, p[i].turnAroundTime, p[i].waitingTime);
    }
}

int main() {
    int n, choice;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time, burst time, and priority for process %d: ", i + 1);
        scanf("%d %d %d", &p[i].arrivalTime, &p[i].burstTime, &p[i].priority);
        p[i].remainingTime = p[i].burstTime;
        p[i].completed = 0;
    }

    printf("Choose Scheduling Type:\n1. Priority Non-Preemptive\n2. Priority Preemptive\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        calculatePriorityNonPreemptive(p, n);
    } else if (choice == 2) {
        calculatePriorityPreemptive(p, n);
    } else {
        printf("Invalid choice! Exiting program.\n");
        return 1;
    }

    displayTable(p, n);

    return 0;
}
