#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int before_fork_mem, after_fork_mem;
    int pid;

    printf(1, "TEST2: ");

    // check memory usage before fork()
    before_fork_mem = freemem();

    // call fork() for CoW test
    pid = fork();
    if (pid < 0) {
        // fork failed
        printf(1, "fork failed\n");
        exit();
    } else if (pid == 0) {
        // child process: exit
        exit();
    } else {
        // parent process: wait for child
        wait();

        // check memory usage after fork()
        after_fork_mem = freemem();

        // compare memory usage
        if (before_fork_mem == after_fork_mem) {
            printf(1, "PASS : Memory usage unchanged after fork()\n");
        } else {
            printf(1, "FAIL : Memory usage changed after fork()\n");
        }
    }

    exit();
}