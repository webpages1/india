#include <stdio.h>

typedef struct {
    int arrivalTime, burstTime, remainingTime, finishTime, turnAroundTime, waitingTime, completed;
} Process;

// Function for SJF Non-Preemptive Scheduling
void calculateSJFNonPreemptive(Process p[], int n) {
    int completed = 0, currentTime = 0;

    while (completed < n) {
        int shortest = -1;

        // Find the process with the shortest burst time in the ready queue
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrivalTime <= currentTime) {
                if (shortest == -1 || p[i].burstTime < p[shortest].burstTime) {
                    shortest = i;
                }
            }
        }

        // If no process is ready, increment the current time
        if (shortest == -1) {
            currentTime++;
        } else {
            p[shortest].finishTime = currentTime + p[shortest].burstTime;
            p[shortest].turnAroundTime = p[shortest].finishTime - p[shortest].arrivalTime;
            p[shortest].waitingTime = p[shortest].turnAroundTime - p[shortest].burstTime;
            p[shortest].completed = 1;
            currentTime = p[shortest].finishTime;
            completed++;
        }
    }
}

// Function for SJF Preemptive Scheduling
void calculateSJFPreemptive(Process p[], int n) {
    int completed = 0, currentTime = 0, minIndex = -1;

    // Run until all processes are completed
    while (completed < n) {
        int newMinIndex = -1;

        // Find the process with the shortest remaining time in the ready queue
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {
                if (newMinIndex == -1 || p[i].remainingTime < p[newMinIndex].remainingTime) {
                    newMinIndex = i;
                }
            }
        }

        // Check if we need to switch processes
        if (newMinIndex != -1) {
            // Switch only if new process has strictly shorter remaining time than the current process
            if (minIndex == -1 || p[newMinIndex].remainingTime < p[minIndex].remainingTime) {
                minIndex = newMinIndex;
            }

            // Execute the current selected process for 1 time unit
            p[minIndex].remainingTime--;
            currentTime++;

            // If the process finishes, calculate its finish time and mark it completed
            if (p[minIndex].remainingTime == 0) {
                p[minIndex].finishTime = currentTime;
                p[minIndex].turnAroundTime = p[minIndex].finishTime - p[minIndex].arrivalTime;
                p[minIndex].waitingTime = p[minIndex].turnAroundTime - p[minIndex].burstTime;
                completed++;
                minIndex = -1; // Reset the currently executing process
            }
        } else {
            // No process is ready, so we increment the current time
            currentTime++;
        }
    }
}

// Function to display the results table
void displayTable(Process p[], int n) {
    printf("\nPID\tArrival\tBurst\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", i + 1, p[i].arrivalTime, p[i].burstTime, p[i].finishTime, p[i].turnAroundTime, p[i].waitingTime);
    }
}

int main() {
    int n, choice;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &p[i].arrivalTime, &p[i].burstTime);
        p[i].remainingTime = p[i].burstTime;
        p[i].completed = 0;
    }

    printf("Choose Scheduling Type:\n1. SJF Non-Preemptive\n2. SJF Preemptive\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        calculateSJFNonPreemptive(p, n);
    } else if (choice == 2) {
        calculateSJFPreemptive(p, n);
    } else {
        printf("Invalid choice! Exiting program.\n");
        return 1;
    }

    displayTable(p, n);

    return 0;
}
