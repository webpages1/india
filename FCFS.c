




#include <stdio.h>

typedef struct {
    int arrivalTime, burstTime, finishTime, turnAroundTime, waitingTime, processed;
} Process;

void calculateFCFS(Process p[], int n) {
    int currentTime = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;

        // Find the next process to execute based on arrival time
        for (int i = 0; i < n; i++) {
            if (!p[i].processed && p[i].arrivalTime <= currentTime) {
                if (idx == -1 || p[i].arrivalTime < p[idx].arrivalTime) {
                    idx = i;
                }
            }
        }

        // If no process has arrived yet, increment the current time
        if (idx == -1) {
            currentTime++;
        } else {
            p[idx].finishTime = currentTime + p[idx].burstTime;
            p[idx].turnAroundTime = p[idx].finishTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnAroundTime - p[idx].burstTime;
            p[idx].processed = 1;
            completed++;
            currentTime = p[idx].finishTime;
        }
    }
}

void displayTable(Process p[], int n) {
    printf("\nPID\tArrival\tBurst\tFinish\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", i + 1, p[i].arrivalTime, p[i].burstTime, p[i].finishTime, p[i].turnAroundTime, p[i].waitingTime);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].processed = 0;
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &p[i].arrivalTime, &p[i].burstTime);
    }

    calculateFCFS(p, n);
    displayTable(p, n);

    return 0;
}