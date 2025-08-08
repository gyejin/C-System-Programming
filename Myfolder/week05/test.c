#include <stdio.h>

int main(){
    void** ptr = 0x00000000FFFF8392; // 예시로 든 값 입니다.
    char* name = (char*) *ptr;
    printf("%s", name);
    void** ptr2 = 0x00000000FFFF3821; // 마찬가지
    int* number = (int*) *ptr2;
    printf("%p", *number);

    return 0;
}
