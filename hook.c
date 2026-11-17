#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static long (*real_syscall)(long, ...);

static void dumpfile(const char *path, const void *p, unsigned long n)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    const char *c = p;
    unsigned long done = 0;
    while (done < n) {
        ssize_t w = write(fd, c + done, n - done);
        if (w <= 0) break;
        done += w;
    }
    close(fd);
}

long syscall(long number, ...)
{
    va_list ap;
    va_start(ap, number);
    long a1 = va_arg(ap, long);
    long a2 = va_arg(ap, long);
    long a3 = va_arg(ap, long);
    long a4 = va_arg(ap, long);
    long a5 = va_arg(ap, long);
    long a6 = va_arg(ap, long);
    va_end(ap);

    if (!real_syscall) real_syscall = dlsym(RTLD_NEXT, "syscall");

    if (number == 105 && a1 > 0x1000 && a2 > 1000) {
        dumpfile("/data/local/tmp/grabbed_init.ko", (void *)a1, (unsigned long)a2);
    }
    if (number == 273 && a1 >= 0) {
        dumpfile("/data/local/tmp/finit_seen.txt", "finit_module\n", 12);
    }
    return real_syscall(number, a1, a2, a3, a4, a5, a6);
}
