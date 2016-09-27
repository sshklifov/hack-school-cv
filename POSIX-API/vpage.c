#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <errno.h>
#include <string.h> 
#include <stdbool.h>

void _assert (bool statement)
{
    if (statement == true) return;

    fprintf (stderr, "Unexpected error: %s\n", strerror (errno));

    exit (EXIT_FAILURE);
}

int main ()
{
    void* p = sbrk (0);
    _assert (p != (void*) -1);

    printf ( "Current programme break: %p\n", p);

    // Allocate some memory and return programme break again
    void* end = (char*) p + 20;
    _assert (brk (end) == 0);
    printf ("New break: %p\n", end);

    // Test
    for (int* it = (int*) p; it != (int*)end; it++)
    {
        *it = 0;
        printf ("Iterating trough allocated array: %p\n", it);
    }

    //Break
    printf ("Accessing out-of-bounds address (1023th int)\n");
    int* seg = (int*) p;
    seg[1023] = 0xdeadbeef; // 4KiB page
    printf ("SIGSEGV not genereated!\n");
    
    printf ("Accessing 1024th int (should generate SIGSEGV)\n");
    seg[1024] = 0xdeadbeef; // rip
    printf ("Programme did not abort!?");
    
    return 0;
}