#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

static long (*real_syscall)(long, ...);
static int  (*real_open)(const char *, int, ...);
static int  (*real_openat)(int, const char *, int, ...);
static ssize_t (*real_read)(int, void *, size_t);

#define MAXFD 1024
static char dumping[MAXFD];
static int  dumpfd[MAXFD];

static void wopen(const char *p, const void *b, unsigned long n)
{
    int fd = open(p, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fd < 0) return;
    const char *c = b; unsigned long d = 0;
    while (d < n) { ssize_t w = write(fd, c+d, n-d); if (w<=0) break; d+=w; }
    close(fd);
}

static void logpath(const char *p)
{
    if (!p) return;
    int fd = open("/data/local/tmp/open.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
    if (fd < 0) return;
    write(fd, p, strlen(p));
    write(fd, "\n", 1);
    close(fd);
}

int open(const char *path, int flags, ...)
{
    if (!real_open) real_open = dlsym(RTLD_NEXT, "open");
    mode_t m = 0;
    if (flags & O_CREAT) { va_list ap; va_start(ap, flags); m = va_arg(ap, mode_t); va_end(ap); }
    logpath(path);
    return real_open(path, flags, m);
}

int openat(int dirfd, const char *path, int flags, ...)
{
    if (!real_openat) real_openat = dlsym(RTLD_NEXT, "openat");
    mode_t m = 0;
    if (flags & O_CREAT) { va_list ap; va_start(ap, flags); m = va_arg(ap, mode_t); va_end(ap); }
    logpath(path);
    return real_openat(dirfd, path, flags, m);
}

ssize_t read(int fd, void *buf, size_t n)
{
    if (!real_read) real_read = dlsym(RTLD_NEXT, "read");
    ssize_t r = real_read(fd, buf, n);
    if (r > 0 && fd >= 0 && fd < MAXFD) {
        if (!dumping[fd] && r >= 4 && memcmp(buf, "\x7f""ELF", 4) == 0) {
            dumping[fd] = 1;
            dumpfd[fd] = open("/data/local/tmp/grabbed.bin", O_WRONLY|O_CREAT|O_TRUNC, 0644);
        }
        if (dumping[fd] && dumpfd[fd] >= 0) {
            int save = dumpfd[fd];
            const char *c = buf; size_t d = 0;
            while (d < (size_t)r) { ssize_t w = write(save, c+d, r-d); if (w<=0) break; d+=w; }
        }
    }
    return r;
}

long syscall(long number, ...)
{
    va_list ap; va_start(ap, number);
    long a1=va_arg(ap,long), a2=va_arg(ap,long), a3=va_arg(ap,long);
    long a4=va_arg(ap,long), a5=va_arg(ap,long), a6=va_arg(ap,long);
    va_end(ap);
    if (!real_syscall) real_syscall = dlsym(RTLD_NEXT, "syscall");
    if (number == 105 && a1 > 0x1000 && a2 > 1000)
        wopen("/data/local/tmp/grabbed_init.ko", (void *)a1, (unsigned long)a2);
    return real_syscall(number, a1, a2, a3, a4, a5, a6);
}
