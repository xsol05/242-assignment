/*
 * COSC242 Assignment 2020
 * Authors: Maaha Ahmad, Magdeline Huang, Kate Truman
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include <stdio.h>

/*
 * Functions that are implemented in mylib.c
 */
extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *, int, FILE *);

#endif
