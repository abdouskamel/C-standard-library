extern int mem_init(void);
extern int main(int argc, char **argv, char **envp);
extern void _exit(int code);

/*
 * Get argc, argv and envp from stack then call main().
 * Exit the process with main return value.
 */
void _start() __attribute__((noreturn));

void _start()
{
    int argc;
    char **argv;

    /*
     * argc is at (%rsp).
     * argv is located 8 bytes after argc.
     */
    asm("mov (%%rsp), %0\n"
        "lea 8(%%rsp), %1"
        : "=r"(argc), "=r"(argv));

    /*
     * argv is an array of #argc elements.
     * We skip all those elements, plus the NULL value at the end of argv
     * to reach envp.
     */
    char **envp = argv + argc + 1;

    if (mem_init() == -1)
        _exit(0xff);

    int ret = main(argc, argv, envp);
    _exit(ret);
}
