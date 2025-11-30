/*
 * file_syscalls_demo.c
 *
 * Interactive C program to demonstrate basic UNIX-like file system calls.
 * Users can select operations from a menu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      /* For O_RDWR, O_CREAT, etc. */
#include <sys/types.h>
#include <sys/stat.h>   /* For permission modes */

/* Windows/UNIX compatibility */
#ifdef _WIN32
    #include <io.h>
    #ifndef S_IRUSR
    #define S_IRUSR _S_IREAD
    #endif
    #ifndef S_IWUSR
    #define S_IWUSR _S_IWRITE
    #endif
#else
    #include <unistd.h>
#endif

void print_menu() {
    printf("\n=== File Operation Menu ===\n");
    printf("1. Create/Open File (open)\n");
    printf("2. Write to File (write)\n");
    printf("3. Read from File (read)\n");
    printf("4. Append to File (lseek + write)\n");
    printf("5. Close File (close)\n");
    printf("6. Delete File (unlink)\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    char filename[256] = "example_syscalls.txt"; /* Default filename */
    int fd = -1; /* File Descriptor, -1 means not open */
    int choice;
    char buffer[1024];
    ssize_t bytes;
    
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            /* Clear input buffer if invalid input */
            while (getchar() != '\n');
            continue;
        }
        /* Consume newline left by scanf */
        getchar();

        switch (choice) {
            case 1: /* Create / Open */
                if (fd != -1) {
                    printf("File is already open (fd: %d). Close it first.\n", fd);
                } else {
                    printf("Enter filename to open/create: ");
                    if (scanf("%255s", filename) != 1) {
                        printf("Invalid filename.\n");
                        break;
                    }
                    /* O_CREAT | O_RDWR: Create if missing, open for read/write */
                    /* Note: Not using O_TRUNC here to allow opening existing files without wiping */
                    fd = open(filename, O_CREAT | O_RDWR, 0644);
                    if (fd == -1) perror("open failed");
                    else printf("File '%s' opened successfully. FD: %d\n", filename, fd);
                }
                break;

            case 2: /* Write */
                if (fd == -1) {
                    printf("File is not open.\n");
                } else {
                    printf("Enter text to write: ");
                    if (fgets(buffer, sizeof(buffer), stdin)) {
                        /* Remove newline if present */
                        buffer[strcspn(buffer, "\n")] = 0;
                        bytes = write(fd, buffer, strlen(buffer));
                        if (bytes == -1) perror("write failed");
                        else printf("Wrote %ld bytes.\n", (long)bytes);
                    }
                }
                break;

            case 3: /* Read */
                if (fd == -1) {
                    printf("File is not open.\n");
                } else {
                    /* Reset to beginning to read whole file, or just read from current pos? 
                       Let's reset to start for a clear 'read file' demo */
                    lseek(fd, 0, SEEK_SET);
                    
                    memset(buffer, 0, sizeof(buffer));
                    bytes = read(fd, buffer, sizeof(buffer) - 1);
                    if (bytes == -1) {
                        perror("read failed");
                    } else if (bytes == 0) {
                        printf("File is empty.\n");
                    } else {
                        printf("\n--- Content (%ld bytes) ---\n%s\n-------------------------\n", (long)bytes, buffer);
                    }
                }
                break;

            case 4: /* Append */
                if (fd == -1) {
                    printf("File is not open.\n");
                } else {
                    if (lseek(fd, 0, SEEK_END) == -1) {
                        perror("lseek failed");
                    } else {
                        printf("Enter text to append: ");
                        if (fgets(buffer, sizeof(buffer), stdin)) {
                            buffer[strcspn(buffer, "\n")] = 0;
                            /* Add a newline for better formatting on append */
                            write(fd, "\n", 1); 
                            bytes = write(fd, buffer, strlen(buffer));
                            if (bytes == -1) perror("write failed");
                            else printf("Appended %ld bytes.\n", (long)bytes);
                        }
                    }
                }
                break;

            case 5: /* Close */
                if (fd == -1) {
                    printf("File is not open.\n");
                } else {
                    if (close(fd) == -1) perror("close failed");
                    else {
                        printf("File closed.\n");
                        fd = -1;
                    }
                }
                break;

            case 6: /* Unlink */
                /* It's good practice to close before unlinking, though not strictly required by OS */
                if (fd != -1) {
                    close(fd);
                    fd = -1;
                    printf("File closed automatically before deletion.\n");
                }
                if (unlink(filename) == -1) perror("unlink failed");
                else printf("File '%s' deleted.\n", filename);
                break;

            case 7: /* Exit */
                if (fd != -1) close(fd);
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
