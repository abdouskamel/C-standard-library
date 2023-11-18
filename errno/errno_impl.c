#include "errno_impl.h"

static int the_errno = 0;

int *get_errno()
{
    return (&the_errno);
}
