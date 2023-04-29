/* test_02.c: allocate chunks and free middle */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Constants */

#define SIZE 1<<10

/* Main Execution */

int main(int argc, char *argv[]) {
    // test realloc
    fputs("p0 malloc 12 bytes\n", stderr);
    char *p0 = malloc(12);
    char *t = p0;
    fputs("p0 realloc 14bytes\n", stderr);
    p0 = realloc(p0, 14);
    assert(t == p0);

    p0 = realloc(p0, 234);
    assert(t != p0);

    free(p0);
    p0 = realloc(NULL, 12);
    for (int i = 0; i < 12; ++i)
        p0[i] = 'a' + i;
    
    p0 = realloc(p0, 23);
    for (int i = 0; i < 12; ++i)
        assert(p0[i] == 'a' + i);
    free(p0);

    // test calloc
    assert(!calloc(0, 12));
    assert(!calloc(12, 0));
    assert(!calloc((size_t)(-1), 109812));

    p0 = calloc(12, sizeof(int));
    for (int i = 0; i < 12; ++i)
        assert(p0[i] == 0);
    free(p0);
    

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
