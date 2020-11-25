#define _GNU_SOURCE             /* See feature_test_macros(7) */#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/sched.h>
#include <malloc.h>


int variable=3;

int thread(void *p) 
{
    int tid;
    
    
    printf("\nsoy el hijo\n");
    sleep(5);
    variable++;
    tid = syscall(SYS_gettid);
    printf("\nPID y TID del hijo:%d %d\n",getpid(),tid);
    printf("\nEn el hijo la variable vale:%d\n",variable);
    return 0;
    
}

int main() 
{
    void **stack;
    int i, tid;
    
    
    stack = (void **)malloc(15000);
    if (!stack)
        return -1;
    
    i = clone(thread, (char*) stack + 15000, CLONE_FILES|CLONE_FS, NULL); //|CLONE_VM|CLONE_THREAD|CLONE_SIGHAND
    sleep(5);
 
    if (i == -1)
        perror("clone");
    tid = syscall(SYS_gettid);
    printf("\nPID y TID del padre:%d %d\n ",getpid(),tid);
    printf("\nEn el padre la variable vale:%d\n",variable);
    
    return 0;
    
}
