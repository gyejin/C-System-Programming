#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

void *malloc(size_t size) {
    //진짜 malloc 함수의 주소를 찾음
    void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");

    //printf 사용 시 '버퍼' 메모리 공간을 사용 => malloc 동적할당 사용
    //printf("%zu 바이트 메모리 할당 요청!\n", size);
    char buf[256];
    sprintf(buf, "%zu 바이트 메모리 할당 요청!\n", size);       //버퍼에 문자열을 만듦
    write(STDOUT_FILENO, buf, strlen(buf));                  //표준 출력에 버퍼의 내용을 씀

    //기존 malloc 함수를 호출
    void *ptr = real_malloc(size);

    //할당된 주소 출력
    sprintf(buf, "할당된 주소: %p\n", ptr);
    write(STDOUT_FILENO, buf, strlen(buf));
    
    return ptr;
}

void free(void *ptr) {
    //기존 free 함수 주소를 찾음
    void (*real_free)(void *) = dlsym(RTLD_NEXT, "free");

    //해제할거라는 안내문 출력
    char buf[256];
    sprintf(buf, "%p 주소 메모리 해제 요청!\n", ptr);
    write(STDOUT_FILENO, buf, strlen(buf));

    //기존 free 함수를 호출
    real_free(ptr);
}