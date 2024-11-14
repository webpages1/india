#include <stdio.h>

void displayFrames(int frames[], int totalFrames) {
    for (int i = 0; i < totalFrames; i++) {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
    }
    printf("\n");
}

int findLRU(int time[], int totalFrames) {
    int min = time[0], pos = 0;
    for (int i = 1; i < totalFrames; i++) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

int isHit(int frames[], int totalFrames, int page) {
    for (int i = 0; i < totalFrames; i++) {
        if (frames[i] == page) return i;
    }
    return -1;
}

int main() {
    int totalFrames, totalPages, pageFaults = 0;

    printf("Enter the total number of frames: ");
    scanf("%d", &totalFrames);

    printf("Enter the total number of pages in the reference string: ");
    scanf("%d", &totalPages);

    int referenceString[totalPages];
    printf("Enter the reference string:\n");
    for (int i = 0; i < totalPages; i++) {
        scanf("%d", &referenceString[i]);
    }

    int frames[totalFrames];
    int time[totalFrames];
    for (int i = 0; i < totalFrames; i++) {
        frames[i] = -1; // Initialize frames as empty
        time[i] = 0;    // Initialize time array
    }

    printf("\nFrame Status After Each Page Request:\n");
    for (int i = 0; i < totalPages; i++) {
        int page = referenceString[i];
        printf("Page %d: ", page);

        int pos = isHit(frames, totalFrames, page);

        if (pos != -1) { // Hit case
            printf("Hit\n");
            time[pos] = i; // Update the time of the page that was accessed
        } else { // Page Fault case
            printf("Page Fault\n");
            int lruIndex = findLRU(time, totalFrames); // Find LRU page
            frames[lruIndex] = page; // Replace LRU page with the new page
            time[lruIndex] = i; // Update time for the new page
            pageFaults++;
        }

        displayFrames(frames, totalFrames); // Display current frame status
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);
    return 0;
}