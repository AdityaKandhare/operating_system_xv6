#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscall.h"

// defined count variables for fork, exit and wait
int fork_syscall_count = 0;
int exit_syscall_count = 0;
int wait_syscall_count = 0;

int sys_fork(void)
{
    fork_syscall_count++;   // incrementing the count
    return fork();
}

int sys_exit(void)
{
    exit_syscall_count++;  // incrementing the count
    exit();
    return 0;  // not reached
}

int sys_wait(void)
{
    wait_syscall_count++;   // incrementing the count
    return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_shutdown(void)
{
  /* Either of the following will work. Does not harm to put them together. */
  outw(0xB004, 0x0|0x2000); // working for old qemu
  outw(0x604, 0x0|0x2000); // working for newer qemu

  return 0;
  
}

// sys_get_syscall_count() to get the syscall count
int sys_get_syscall_count(void)
{
  int n;
  argint(0, &n);
  if(n == (0)) return fork_syscall_count;
  if(n == (1)) return wait_syscall_count;
  if(n == (2)) return exit_syscall_count;
  return -1; // Invalid syscall type
}

// reset the syscall count
int sys_reset_syscall_count(void)
{
    fork_syscall_count = wait_syscall_count = exit_syscall_count =0;
    return 0; // Placeholder return value
}

