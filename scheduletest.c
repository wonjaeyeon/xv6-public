//#include "types.h"
//#include "user.h"
//
//int main(void) {
//    int pid = fork();
//    if(pid == 0) {
//        // Child process
//        set_proc_priority(getpid(), 2); // Set high priority
//        for(;;) {
//            printf(1, "Child process running\n");
//            yield();
//        }
//    } else {
//        // Parent process
//        set_proc_priority(getpid(), 10); // Set low priority
//        for(;;) {
//            printf(1, "Parent process running\n");
//            yield();
//        }
//    }
//    exit();
//}
// tell me how to fix this code