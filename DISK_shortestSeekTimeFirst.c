#include <stdio.h>
#include <stdlib.h>

int find_nearest_request(int requests[], int n, int current_head, int visited[]) {
    int min_distance = __INT_MAX__;
    int nearest_index = -1;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) { // Check if the request hasn't been visited
            int distance = abs(requests[i] - current_head);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_index = i;
            }
        }
    }

    return nearest_index;
}

void sstf_disk_scheduling(int requests[], int n, int head_start) {
    int seek_time = 0;
    int current_head = head_start;
    int visited[n];
    int seek_sequence[n + 1];
    
    // Initialize visited array
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    
    // Start the seek sequence with the initial head position
    seek_sequence[0] = head_start;

    printf("Input Table:\n");
    printf("Request Order | Request Position\n");
    for (int i = 0; i < n; i++) {
        printf("      %d       |       %d\n", i + 1, requests[i]);
    }

    printf("\nOutput Table:\n");
    printf("Step | From | To | Seek Time\n");

    // Process requests
    for (int i = 0; i < n; i++) {
        int nearest_index = find_nearest_request(requests, n, current_head, visited);
        if (nearest_index == -1) break;

        int seek_step = abs(requests[nearest_index] - current_head);
        seek_time += seek_step;
        printf(" %2d   |  %3d  | %3d |    %d\n", i + 1, current_head, requests[nearest_index], seek_step);

        // Update the current head position, mark as visited, and update seek sequence
        current_head = requests[nearest_index];
        visited[nearest_index] = 1;
        seek_sequence[i + 1] = current_head;
    }

    printf("\nTotal Seek Time: %d\n", seek_time);
}

int main() {
    int n, head_start;

    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    int requests[n];
    printf("Enter the disk requests:\n");
    for (int i = 0; i < n; i++) {
        printf("Request %d: ", i + 1);
        scanf("%d", &requests[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head_start);

    sstf_disk_scheduling(requests, n, head_start);

    return 0;
}