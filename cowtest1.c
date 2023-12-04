#include "types.h"
#include "stat.h"
#include "user.h"
#define META_DATA_SIZE 68 // 68 bytes for meta data size

int
main(int argc, char **argv) {
    int before_fork_mem, after_fork_mem;
    int pid;

    printf(1, "TEST1: ");

    before_fork_mem = freemem();

    pid = fork();
    if (pid < 0) {
        // fork failed
        printf(1, "fork failed\n");
        exit();
    } else if (pid == 0) {
        after_fork_mem = freemem();
        if (before_fork_mem - after_fork_mem == META_DATA_SIZE) {
            printf(1, "PASS : Only Used Memory in Meta Data\n");
        } else
            printf(1, "FAIL : Used Memory out of Meta Data Size\n");
        exit();
    } else {
        wait();
    }
    exit();
}