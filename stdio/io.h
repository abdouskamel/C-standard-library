#ifndef STDIO_IO_H
#define STDIO_IO_H

#include "types.h"

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);

#endif
