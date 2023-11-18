#include <stdarg.h>
#include <unistd.h>

void func(int a, ...)
{
    va_list vl;
    va_start(vl, a);

    write(1, &a, 4);

    int p = va_arg(vl, int);
    write(1, &p, 4);
}

int main(int argc, char **argv, char **envp)
{
    func(1, 2, 3);
    func(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    return (0);
}
