#include <stdio.h>

// 🌍 1. 전역 변수 (Global Variable)
// - 프로그램 전체에서 접근 가능
// - 프로그램 시작 시 생성되어 종료 시 소멸
int g_global_var = 100;

// 🔐 2. 정적 전역 변수 (Static Global Variable)
// - 이 파일('main.c') 안에서만 접근 가능
// - 프로그램 시작 시 생성되어 종료 시 소멸
static int s_global_var = 200;


void demo_function() {
    // 🥤 3. 지역 변수 (Local Variable)
    // - 이 함수 안에서만 접근 가능
    // - 함수 호출 시 생성되어 함수 종료 시 소멸 (매번 리셋)
    int local_var = 0;

    // 텀블러 4. 정적 지역 변수 (Static Local Variable)
    // - 이 함수 안에서만 접근 가능
    // - 프로그램 시작 시 생성되어 종료 시 소멸 (값을 계속 기억)
    static int s_local_var = 0;

    printf("--- 함수 내부에서 출력 ---\n");
    printf("1. 전역 변수        g_global_var: %d\n", ++g_global_var);
    printf("2. 정적 전역 변수   s_global_var: %d\n", ++s_global_var);
    printf("3. 지역 변수        local_var   : %d\n", ++local_var);
    printf("4. 정적 지역 변수   s_local_var : %d\n", ++s_local_var);
}


int main() {
    printf("======= 첫 번째 함수 호출 =======\n");
    demo_function();

    printf("\n======= 두 번째 함수 호출 =======\n");
    demo_function();

    printf("\n--- main 함수에서 출력 ---\n");
    printf("1. 전역 변수        g_global_var: %d\n", g_global_var);
    printf("2. 정적 전역 변수   s_global_var: %d\n", s_global_var);
    
    // 아래 두 줄은 컴파일 에러가 발생합니다. (Scope 밖이라서!)
    // printf("3. 지역 변수        local_var: %d\n", local_var);
    // printf("4. 정적 지역 변수   s_local_var: %d\n", s_local_var);

    return 0;
}