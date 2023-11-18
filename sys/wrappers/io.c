#include "io.h"
#include <sys/syscall.h>

int open(const char *filename, int flags, mode_t mode)
{
    syscall_32ret(SYS_open, int);
    return (ret);
}

int close(int fd)
{
    syscall_32ret(SYS_close, int);
    return (ret);
}

ssize_t read(int fd, void *buf, size_t count)
{
    syscall_64ret(SYS_read, ssize_t);
    return (ret);
}

ssize_t write(int fd, const void *buf, size_t count)
{
    syscall_64ret(SYS_write, ssize_t);
    return (ret);
}
