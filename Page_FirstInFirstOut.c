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
        if (frames[i] == page) return 1;
    }
    return 0;
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
    
    int index = 0; // Points to the frame to be replaced in FIFO order
    
    printf("\nFrame Status After Each Page Request:\n");
    for (int i = 0; i < totalPages; i++) {
        int page = referenceString[i];
        printf("Page %d: ", page);
        
        if (isHit(frames, totalFrames, page)) {
            printf("Hit\n");
        } else {
            printf("Page Fault\n");
            frames[index] = page; // Replace the page at the FIFO index
            index = (index + 1) % totalFrames; // Move index to the next frame in FIFO
            pageFaults++;
        }
        
        displayFrames(frames, totalFrames); // Display current frame status
    }
    
    printf("\nTotal Page Faults: %d\n", pageFaults);
    return 0;
}