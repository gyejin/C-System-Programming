#include <stdio.h>
#include <string.h>

int main(){
    char *a = "j";
    char b[10] = "ju";

    //strcpy(b, a);
    //strcat(b, a);

    //printf("%s\n", strstr(b, a));
    printf("%d\n", strcmp(b, a));


    return 0;
}