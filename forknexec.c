//
// Created by wonjaeyeon on 10/14/23.
//
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    const char *args[] = {"echo", "NEWPROC_CALL", 0};
    int ret;
    printf(1, "Test forknexec syscall\n");
    printf(1, "%s\n", args[0]);
    ret = forknexec(args[0], args);
    printf(1, "returned: %d\n", ret);
    exit();
}