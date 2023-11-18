#ifndef SYS_SYSCALL_H
#define SYS_SYSCALL_H

#include <errno.h>

#define SYS_read 0
#define SYS_write 1
#define SYS_open 2
#define SYS_close 3
#define SYS_brk 12

#define SYS_exit 60

// Use this macro for syscalls that return a 32-bit value.
#define syscall_32ret(SYS_num, ret_type) \
    ret_type ret;                        \
    asm("mov %1, %%eax\n"                \
        "syscall\n"                      \
        "mov %%eax, %0"                  \
        : "=r"(ret)                      \
        : "n"(SYS_num));                 \
    if (ret < 0)                         \
    {                                    \
        errno = -ret;                    \
        return (-1);                     \
    }

// Use this macro for syscalls that return a 64-bit value.
#define syscall_64ret(SYS_num, ret_type) \
    ret_type ret;                        \
    asm("mov %1, %%eax\n"                \
        "syscall\n"                      \
        "mov %%rax, %0"                  \
        : "=r"(ret)                      \
        : "n"(SYS_num));                 \
    if (ret < 0)                         \
    {                                    \
        errno = -ret;                    \
        return (-1);                     \
    }

// Use this macro for syscalls that return a 64-bit value, and if you want the wrapper not to set errno.
#define syscall_64ret_noerrno(SYS_num, ret_type) \
    ret_type ret;                                \
    asm("mov %1, %%eax\n"                        \
        "syscall\n"                              \
        "mov %%rax, %0"                          \
        : "=r"(ret)                              \
        : "n"(SYS_num));

// Use this macro for syscalls that don't return anything. */
#define syscall_noret(SYS_num) \
    asm("mov %0, %%eax\n"      \
        "syscall\n"            \
        :                      \
        : "n"(SYS_num));

#endif
