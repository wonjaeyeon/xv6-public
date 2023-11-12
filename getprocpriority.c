#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    int pid, priority;

    // Check for correct number of arguments
    if(argc < 2) {
        printf(2, "Usage: getprocpriority pid\n");
        exit();
    }

    // Convert PID from string to int
    pid = atoi(argv[1]);

    // Get the priority of the process
    priority = get_proc_priority(pid);

    // Check for valid return value (error handling)
    if(priority == -1) {
        printf(2, "Error: Could not retrieve priority for PID %d\n", pid);
        exit();
    }

    // Display the priority
    printf(1, "Priority of PID %d is %d\n", pid, priority);

    exit();
}