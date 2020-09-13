/**
 * COSC242 Assignment 2020
 * Authors: Maaha Ahmad, Magdeline Huang, Kate Truman
 */

#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include <assert.h>
#include <ctype.h>
#include <getopt.h>

/**
 * Dynamically allocates a block of memory with
 * the specified size
 * 
 * @param s is the size of the memory to be allocated
 * 
 * @return result is the variable to which memory was 
 * allocated
 * 
 */
void *emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Dynamically re-allocates memory that was already 
 * allocated by malloc to create extra memory when needed
 * 
 * @param p is the pointer to the variable
 * @param s is the size of the extra memory to be allocated
 * 
 * @return p is the variable to which memory was 
 * re-allocated
 * 
 */
void *erealloc(void *p, size_t s) {
    p = realloc(p, s);
    if (NULL == p) {
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

/**
 * Reads words from the file and converts them into an 
 * integer value which is returned
 * 
 * @param s is the word read in from the file
 * @param limit checks if character count has not exceeded
 * @param stream is the file to get words from
 * 
 * @return an integer value 
 * 
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);
    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c);
    if (EOF == c) {
        return EOF;
    }
    else if (--limit > 0) {
        /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        }
        else if ('\'' == c) {limit++;
        }
        else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}
