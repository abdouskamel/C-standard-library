#include "processes.h"
#include <sys/syscall.h>

void _exit(int status)
{
    syscall_noret(SYS_exit);
}
