#include <stdio.h>

int main(void) {
    // char input_char; // getchar의 반환형이 int이므로 int로 받음
    // printf("문자 하나를 입력하세요: ");
    
    // // getchar 함수 호출 (인자 없음)
    // input_char = getchar();
    
    // printf("입력한 문자: %d", input_char);
    // putchar(input_char); // 입력받은 문자를 출력
    // char my_char = 'A';
    // putchar(my_char); // 화면에 A가 출력됨
    // putchar(65);      // 아스키 코드 65에 해당하는 'A'가 출력됨

    // int my_char = 66;
    // putc(my_char, stdout);

    // char my_string[20]; // 1. 문자열을 저장할 공간 (매개변수 str)
    
    // printf("이름을 입력하세요: ");
    
    // // 2. fgets 함수 호출
    // // 인자: my_string (저장할 곳), 20 (최대 크기), stdin (키보드)
    // fgets(my_string, 20, stdin);
    
    // printf("당신의 이름은: %s", my_string);

    // int arr[5] = {0};
    // int i = 0;

    // // arr[0]에 1을 넣으려는 의도였지만...
    // arr[i++] = i; 

    // // 컴파일러의 해석에 따라 두 가지 결과가 나올 수 있다.
    // // 1) arr[0] = 1;  (i++보다 arr[i]가 먼저 평가됨)
    // // 2) arr[0] = 0;  (i가 0인 상태에서 arr[0]에 i(0)을 할당 후, i가 1로 증가) -> 이 경우는 드물다.
    // // 하지만 가장 위험한 시나리오는...
    // // 3) arr[1] = 0;  (i++가 먼저 평가되어 i가 1이 된 후, arr[i]가 평가됨)

    // printf("i = %d\n", i); // i는 확실히 1이 된다.
    // printf("arr[0] = %d\n", arr[0]); // 보통 1이 나오지만 보장할 수 없음
    // printf("arr[1] = %d\n", arr[1]);
    
    // return 0;

    // int i = 3;
    
    // // 컴파일러는 오른쪽 인자부터 처리할까? 왼쪽부터 할까? 자기 마음대로!
    // printf("%d %d\n", i, i++); 
    // // 어떤 컴파일러는 "4 3"을 출력하고,
    // // 다른 컴파일러는 "3 3"을 출력할 수 있다.
    // // 결과는 아무도 보장 못 함!

    // i = 3;
    // printf("%d %d\n", i++, i++);
    // // 이것 역시 "4 3" 또는 "3 4" 등 예측 불가능한 결과를 낳는다.

    int x = 5;
    int y = 10;
    int sum = x++ + --y; // x: 6, y:9, sum: 5+9=14
    printf("x=%d, y=%d, sum=%d\n", x, y, sum); // x=6, y=9, sum=14

    return 0;
}