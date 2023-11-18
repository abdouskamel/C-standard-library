#ifndef STDIO_OPEN_H
#define STDIO_OPEN_H

#include "types.h"

#define FOPEN_MAX
#define FILENAME_MAX

#define BUFSIZ 512
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fclose(FILE *stream);

void setbuf(FILE *stream, char *buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);
int fflush(FILE *stream);

#endif
