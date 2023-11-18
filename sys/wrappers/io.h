#ifndef SYS_WRAPPERS_IO_H
#define SYS_WRAPPERS_IO_H

#include <stdio.h>
#include <stdint.h>

typedef int64_t ssize_t;
typedef uint32_t mode_t;

/* Flags for open syscall */
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 64
#define O_TRUNC 512
#define O_APPEND 1024

#define is_read_mode(flags) (((flags) & 0x01) == 0)

/* Modes for open syscall */
#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXG 0070
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXO 0007
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001

int open(const char *filename, int flags, mode_t mode);
int close(int fd);

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

#endif
