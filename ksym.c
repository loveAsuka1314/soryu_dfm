#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/string.h>
#include "ksym.h"

typedef unsigned long (*kln_t)(const char *name);
static kln_t kln;

static int ksym_probe_pre(struct kprobe *p, struct pt_regs *regs)
{
    return 0;
}

unsigned long soryu_ksym_probe(const char *name)
{
    struct kprobe kp;
    unsigned long addr = 0;

    memset(&kp, 0, sizeof(kp));
    kp.symbol_name = name;
    kp.pre_handler = ksym_probe_pre;
    if (register_kprobe(&kp) == 0) {
        addr = (unsigned long)kp.addr;
        unregister_kprobe(&kp);
    }
    return addr;
}

unsigned long soryu_ksym_kln_addr(void)
{
    return (unsigned long)kln;
}

int soryu_ksym_init(void)
{
    unsigned long a;

    a = soryu_ksym_probe("kallsyms_lookup_name.cfi_jt");
    if (!a)
        a = soryu_ksym_probe("kallsyms_lookup_name");
    if (!a) {
        pr_err("soryu: kln unresolved\n");
        return -ENOENT;
    }
    kln = (kln_t)a;
    return 0;
}

static __nocfi unsigned long ksym_call(const char *name)
{
    if (!kln)
        return 0;
    return kln(name);
}

unsigned long soryu_ksym_raw(const char *name)
{
    char buf[KSYM_NAME_LEN];
    unsigned long a;

    if (!kln)
        return 0;
    if (snprintf(buf, sizeof(buf), "%s.cfi_jt", name) < sizeof(buf)) {
        a = ksym_call(buf);
        if (a)
            return a;
    }
    return ksym_call(name);
}
