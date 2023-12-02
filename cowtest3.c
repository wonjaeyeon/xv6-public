#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFFER_SIZE 100 // 100 bytes for buffer size (100 chars)

int main(int argc, char **argv) {
    char *buffer;
    int before, after;
    int pid;

    buffer = malloc(BUFFER_SIZE);
    if (buffer == 0) {
        printf(1, "Memory allocation failed\n");
        exit();
    }

    // Initialize buffer to ensure it's allocated
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 'a';
    }

    printf(1, "TEST3: ");

    pid = fork();
    if (pid == 0) {
        // Child process
        before = freemem();

        // Modify part of the buffer
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = 'b';
        }

        after = freemem();

        // Compare memory usage
        if (before != after) {
            printf(1, "PASS : Memory usage changed\n");
        } else {
            printf(1, "FAIL : Memory usage unchanged\n");
        }

        free(buffer);
        exit();
    } else {
        // Parent process
        wait();
        free(buffer);
    }

    exit();
}
