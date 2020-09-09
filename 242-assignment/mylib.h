#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include <stdio.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *, int, FILE *);
#endif
