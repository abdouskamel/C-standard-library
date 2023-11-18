#include "open.h"
#include <sys/wrappers/io.h>
#include <mem/malloc.h>

FILE *fopen(const char *filename, const char *mode)
{
    FILE *file = malloc(sizeof(FILE));

    /* Boolean indicating if the file is opened in binary mode */
    int bin_mode = 0;

    /* Check open mode */
    switch (*(mode++))
    {
    case 'r':
        file->flags = O_RDONLY;
        break;

    case 'w':
        file->flags = (O_WRONLY | O_CREAT | O_TRUNC);
        break;

    case 'a':
        file->flags = O_APPEND;
        break;

    default:
        free(file);
        return (NULL);
    }

    if (*mode == '+')
    {
        file->flags |= O_RDWR;
        ++mode;
    }

    if (*(mode++) == 'b')
    {
        bin_mode = 1;

        /* + can be either at the middle, or at the end of mode */
        if (*mode == '+')
            file->flags |= O_RDWR;
    }

    /* Open the file */
    file->fd = open(filename, file->flags, S_IRWXU);
    if (file->fd < 0)
    {
        free(file);
        return (NULL);
    }

    int ret;

    file->buffer = NULL;
    file->self_allocated_buff = 1;

    /* In bin mode, we use fully buffered streams. */
    if (bin_mode)
        ret = setvbuf(file, NULL, _IOFBF, BUFSIZ);

    /* In text mode, we use line buffered streams. */
    else
        ret = setvbuf(file, NULL, _IOLBF, BUFSIZ);

    if (ret == -1)
    {
        free(file);
        return (NULL);
    }

    file->last_op = NO_IO_OP;
    return (file);
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    return (NULL);
}

int fclose(FILE *stream)
{
    fflush(stream);

    int ret = close(stream->fd);
    if (stream->self_allocated_buff)
        free(stream->buffer);

    free(stream);

    return (ret);
}

void setbuf(FILE *stream, char *buffer)
{
    if (stream->self_allocated_buff)
        free(stream->buffer);

    stream->buffer = buffer;

    if (buffer == NULL)
        stream->buff_mode = _IONBF;

    else
        stream->buff_mode = _IOFBF;

    stream->self_allocated_buff = 0;
    stream->buff_pos = 0;
    stream->buff_end = 0;
}

int setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
    if (stream->self_allocated_buff)
        free(stream->buffer);

    stream->buff_mode = mode;

    if (mode == _IONBF)
    {
        stream->buffer = NULL;
        stream->self_allocated_buff = 0;
        return (0);
    }

    stream->buff_size = size;

    /* If buffer is NULL, we let the library allocate it */
    if (buffer == NULL)
    {
        stream->buffer = malloc(size);
        if (stream->buffer == NULL)
            return (-1);

        stream->self_allocated_buff = 1;
    }

    else
    {
        stream->buffer = buffer;
        stream->self_allocated_buff = 0;
    }

    stream->buff_pos = 0;
    stream->buff_end = 0;

    return (0);
}

int fflush(FILE *stream)
{
    if (stream == NULL)
    {
        /* TODO: Flush all open streams */
    }

    /* We do nothing if there's no buffering,
       or if no IO operation has been done */
    if (stream->buff_mode == _IONBF ||
        stream->last_op == NO_IO_OP)
        return (0);

    /* The buffer is an input buffer, just clear it. */
    if (stream->last_op == INPUT_OP)
    {
        stream->buff_pos = 0;
        stream->buff_end = 0;
    }

    /* The buffer is an output buffer,
       get all its content written to the file */
    else
    {
        if (write(stream->fd, stream->buffer + stream->buff_pos,
                  stream->buff_end - stream->buff_pos) == -1)
            return (EOF);

        stream->buff_pos = 0;
        stream->buff_end = 0;
    }

    return (0);
}
