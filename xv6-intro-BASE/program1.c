#include "types.h"
#include "user.h"
#include "syscall.h"

int main(void)
{
    reset_syscall_count();
    int pid = fork();
    wait();
    if(pid == 0) {
        printf(1, "Fork syscall count: %d\n", get_syscall_count(0)); 
        printf(1, "Wait syscall count: %d\n", get_syscall_count(1));
        printf(1, "Exit syscall count: %d\n", get_syscall_count(2));
        
    }
    exit();
}
