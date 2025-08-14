#include <stdio.h>

int global_int; // 전역 변수 (0으로 자동 초기화)
double global_double; // 전역 변수 (0.0으로 자동 초기화)

void check_static() {
    static int static_int; // 정적 지역 변수 (0으로 자동 초기화)
    printf("정적 변수: %d\n", static_int);
    static_int++;
}

int main() {
    printf("전역 정수: %d\n", global_int);
    printf("전역 실수: %f\n", global_double);
    check_static(); // 처음 호출
    check_static(); // 두 번째 호출 (값 유지)
    return 0;
}