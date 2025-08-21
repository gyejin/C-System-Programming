#include <stdio.h>
#include <stdlib.h>

int main() { 
    void *p1 = malloc(10);
    void *p2 = malloc(20);
    
    free(p1);
    free(p2);
    
    return 0;
}