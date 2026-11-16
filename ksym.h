/* soryu kernel symbol resolver */
#ifndef SORYU_KSYM_H
#define SORYU_KSYM_H
unsigned long soryu_ksym_raw(const char *name);
int soryu_ksym_init(void);
#endif
