#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>

#define MAX 100

// Function prototypes
void process_submenu();
void file_submenu();
void communication_submenu();

// Message queue structure
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

void process_submenu() {
    int choice;
    pid_t pid;
    
    while (1) {
        printf("\n--- Process Related System Calls ---\n");
        printf("1. fork()\n");
        printf("2. exit()\n");
        printf("3. wait()\n");
        printf("4. exec()\n");
        printf("5. kill()\n");
        printf("6. Go back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                pid = fork();
                if (pid == -1) {
                    printf("Fork failed\n");
                } else if (pid == 0) {
                    printf("Child process (PID: %d) created.\n", getpid());
                    exit(0);
                } else {
                    printf("Parent process (PID: %d), child PID: %d\n", getpid(), pid);
                }
                break;
                
            case 2:
                printf("Exiting process with exit() call...\n");
                exit(0);  // Exits current process (main program will stop here)
                break;
                
            case 3:
                printf("Parent process waiting for child to exit...\n");
                wait(NULL);
                printf("Child process exited.\n");
                break;
                
            case 4: 
                pid = fork();
                if (pid == 0) {
                    printf("In child, executing 'ls'...\n");
                    execl("/bin/ls", "ls", NULL);
                } else {
                    wait(NULL);
                    printf("exec() completed.\n");
                }
                break;

            case 5:
                printf("Enter process ID to kill: ");
                scanf("%d", &pid);
                if (kill(pid, SIGKILL) == 0) {
                    printf("Process %d killed.\n", pid);
                } else {
                    printf("Failed to kill process.\n");
                }
                break;

            case 6:
                return;  // Exit submenu
                
            default:
                printf("Invalid choice, try again.\n");
        }
    }
}

void file_submenu() {
    int choice, fd;
    char buf[100];
    off_t offset;
    
    while (1) {
        printf("\n--- File Related System Calls ---\n");
        printf("1. open() and close()\n");
        printf("2. read() and write()\n");
        printf("3. link() and unlink()\n");
        printf("4. stat()\n");
        printf("5. lseek()\n");
        printf("6. chmod()\n");
        printf("7. Go back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                fd = open("testfile.txt", O_CREAT | O_WRONLY, 0644);
                if (fd < 0) {
                    printf("Failed to open file\n");
                } else {
                    printf("File opened with descriptor %d\n", fd);
                    close(fd);
                    printf("File closed.\n");
                }
                break;
                
            case 2:
                fd = open("testfile.txt", O_CREAT | O_WRONLY, 0644);
                write(fd, "Hello, file operations!", 24);
                close(fd);
                printf("Data written to file and closed.\n");
                
                fd = open("testfile.txt", O_RDONLY);
                read(fd, buf, 100);
                printf("Data read from file: %s\n", buf);
                close(fd);
                break;
                
            case 3:
                link("testfile.txt", "linkfile.txt");
                printf("Link created as 'linkfile.txt'\n");
                unlink("linkfile.txt");
                printf("Link removed.\n");
                break;
                
            case 4:
                {
                    struct stat st;
                    stat("testfile.txt", &st);
                    printf("File size: %ld bytes\n", st.st_size);
                    printf("Last modified: %ld\n", st.st_mtime);
                }
                break;
                
            case 5:
                fd = open("testfile.txt", O_RDWR);
                if (fd < 0) {
                    printf("Failed to open file\n");
                    break;
                }
                offset = lseek(fd, 10, SEEK_SET);
                write(fd, "SEEK", 4);
                printf("lseek set file offset to %ld and wrote 'SEEK'.\n", offset);
                close(fd);
                break;

            case 6:
                chmod("testfile.txt", 0777);
                printf("File permissions changed to 777.\n");
                break;
                
            case 7:
                return;  // Exit submenu
                
            default:
                printf("Invalid choice, try again.\n");
        }
    }
}

void communication_submenu() {
    int choice, pipefds[2];
    char write_msg[] = "Hello from pipe!";
    char read_msg[20];
    int msgid;
    struct msg_buffer message;
    
    while (1) {
        printf("\n--- Communication Related System Calls ---\n");
        printf("1. pipe()\n");
        printf("2. fifo() (Named Pipe)\n");
        printf("3. msgget(), msgsnd(), msgrcv() (Message Queue)\n");
        printf("4. socket()\n");
        printf("5. Go back to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                if (pipe(pipefds) == -1) {
                    printf("Pipe creation failed\n");
                } else {
                    // Writing to pipe
                    write(pipefds[1], write_msg, sizeof(write_msg));
                    
                    // Reading from pipe
                    read(pipefds[0], read_msg, sizeof(read_msg));
                    printf("Message from pipe: %s\n", read_msg);
                    
                    close(pipefds[0]);
                    close(pipefds[1]);
                }
                break;
                
            case 2:
                printf("Creating a named FIFO...\n");
                mkfifo("myfifo", 0666);
                printf("Named pipe 'myfifo' created.\n");
                unlink("myfifo");  // Cleanup the FIFO
                printf("Named pipe 'myfifo' removed.\n");
                break;
                
            case 3:
                // Message Queue Example
                key_t key = ftok("progfile", 65);
                msgid = msgget(key, 0666 | IPC_CREAT);
                message.msg_type = 1;

                printf("Enter a message to send: ");
                scanf("%s", message.msg_text);
                msgsnd(msgid, &message, sizeof(message), 0);
                printf("Message sent.\n");

                msgrcv(msgid, &message, sizeof(message), 1, 0);
                printf("Message received: %s\n", message.msg_text);

                msgctl(msgid, IPC_RMID, NULL);  // Destroy message queue
                break;

            case 4:
                // Socket example (for demonstration purposes, no actual network code)
                printf("Creating socket...\n");
                int sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd == -1) {
                    printf("Socket creation failed.\n");
                } else {
                    printf("Socket created successfully.\n");
                    close(sockfd);
                }
                break;

            case 5:
                return;  // Exit submenu
                
            default:
                printf("Invalid choice, try again.\n");
        }
    }
}

int main() {
    int choice;
    
    while (1) {
        printf("\n--- Main Menu ---\n");
        printf("1. Process Related System Calls\n");
        printf("2. File Related System Calls\n");
        printf("3. Communication Related System Calls\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                process_submenu();
                break;
                
            case 2:
                file_submenu();
                break;
                
            case 3:
                communication_submenu();
                break;
                
            case 4:
                printf("Exiting program...\n");
                exit(0);
                
            default:
                printf("Invalid choice, try again.\n");
        }
    }
    
    return 0;
}
