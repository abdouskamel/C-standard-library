#include "io.h"
#include "open.h"
#include <sys/wrappers/io.h>

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    if (size == 0 || count == 0)
        return (0);

    size_t tsize = size * count;

    stream->last_op = OUTPUT_OP;

    // No buffering, just write data to the file
    if (stream->buff_mode == _IONBF)
    {
        ssize_t ret = write(stream->fd, ptr, tsize);
        return (ret >= 0 ? ret : 0);
    }

    // Buffer is full, write everything to the file
    if (stream->buff_end + tsize >= stream->buff_size)
    {
        if (write(stream->fd, stream->buffer, stream->buff_end) == -1)
            return (0);

        // Clear buffer
        stream->buff_end = 0;

        ssize_t ret = write(stream->fd, ptr, tsize);
        return (ret >= 0 ? ret : 0);
    }

    // Buffer is not full, and we have a line buffered stream, so we write to the buffer.
    // If we find a \n, we put everything to the file
    if (stream->buff_mode == _IOLBF)
    {
        int i;

        // Store the position of the character following the last \n in user string
        int lf = -1;

        for (i = 0; i < tsize; ++i, ++stream->buff_end)
        {
            char c = ((char *)ptr)[i];
            stream->buffer[stream->buff_end] = c;

            if (c == '\n')
                lf = stream->buff_end + 1;
        }

        // We have a \n, so we flush data to the file
        if (lf != -1)
        {
            if (write(stream->fd, stream->buffer, lf) == -1)
                return (0);

            // Copy characters following last \n to the buffer
            stream->buff_end = 0;
            for (i = lf; i < tsize; ++i, ++stream->buff_end)
                stream->buffer[stream->buff_end] = ((char *)ptr)[i];
        }
    }

    // Here, it's a fully buffered stream, so we write everything to our buffer
    else
    {
        int i;
        for (i = 0; i < tsize; ++i, ++stream->buff_end)
            stream->buffer[stream->buff_end] = ((char *)ptr)[i];
    }

    // Everything is ok
    return (tsize);
}

size_t fread(void *ptr, size_t size, size_t count, FILE *stream)
{
    if (size == 0 || count == 0)
        return (0);

    size_t tsize = size * count;

    stream->last_op = INPUT_OP;

    // No buffering, just read data from the file
    ssize_t ret = read(stream->fd, ptr, tsize);
    return (ret >= 0 ? ret : 0);
}
