#include <stdio.h>
#include <stdlib.h>

void displayFrames(int frames[], int totalFrames) {
    for (int i = 0; i < totalFrames; i++) {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
    }
    printf("\n");
}

int isHit(int frames[], int totalFrames, int page) {
    for (int i = 0; i < totalFrames; i++) {
        if (frames[i] == page)
            return 1; // Hit
    }
    return 0; // Page fault
}

int findOptimal(int frames[], int totalFrames, int referenceString[], int currentIndex, int totalPages) {
    int farthest = -1, replaceIndex = -1;
    
    for (int i = 0; i < totalFrames; i++) {
        int j;
        for (j = currentIndex + 1; j < totalPages; j++) {
            if (frames[i] == referenceString[j]) {
                if (j > farthest) {
                    farthest = j;
                    replaceIndex = i;
                }
                break;
            }
        }
        
        if (j == totalPages) { // If the page is not found in the future
            return i;
        }
    }

    return replaceIndex; // Return the index of the page to replace
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
    for (int i = 0; i < totalFrames; i++) {
        frames[i] = -1; // Initialize frames as empty
    }

    printf("\nFrame Status After Each Page Request:\n");
    for (int i = 0; i < totalPages; i++) {
        int page = referenceString[i];
        printf("Page %d: ", page);

        if (isHit(frames, totalFrames, page)) {
            printf("Hit\n");
        } else {
            printf("Page Fault\n");

            // Find the optimal page to replace
            int replaceIndex = findOptimal(frames, totalFrames, referenceString, i, totalPages);
            frames[replaceIndex] = page;
            pageFaults++;
        }

        displayFrames(frames, totalFrames); // Display the current frame status
    }

    printf("\nTotal Page Faults: %d\n", pageFaults);
    return 0;
}

