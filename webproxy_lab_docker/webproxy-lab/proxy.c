#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

void doit(int fd);
void parse_uri(char *uri, char *hostname, char *port, char *path);

int main(int argc, char **argv)
{
  int listenfd, connfd;   //소켓 변호표 담을 변수
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;   //접속한 클라이언트 정보 담을 그릇

  /* Check command line args */
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  //지정된 포트의 문(소켓)을 열고 듣기 시작
  listenfd = Open_listenfd(argv[1]);
  //무한 루프 돌며 손님 계속 기다림
  while (1)
  {
    clientlen = sizeof(clientaddr);
    //손님이 오면 연결을 수락하고, 통신용 새 소켓(connfd) 생성
    connfd = Accept(listenfd, (SA *)&clientaddr,
                    &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE,
                0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    //실제 요청 처리는 doit 함수에게 넘김
    doit(connfd);
    //통신 끝나면 연결 끊음
    Close(connfd);
  }
}

void doit(int fd)
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], port[MAXLINE], path[MAXLINE];
    int clientfd;
    rio_t rio, server_rio;

    //요청 라인 읽고 파싱
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;
    sscanf(buf, "%s %s %s", method, uri, version);
    
    //URI에서 기본 hostname, port, path 추출
    parse_uri(uri, hostname, port, path);

    //클라이언트의 요청 헤더를 읽으며 Host 헤더 찾기
    Rio_readlineb(&rio, buf, MAXLINE);
    while(strcmp(buf, "\r\n")) {
        //Host 헤더를 찾으면, hostname과 port를 정확히 분리해서 저장
        if (strncmp(buf, "Host:", 5) == 0) {
            char *host_ptr = buf + 6; //"Host: " 다음부터 시작
            char *port_ptr = strchr(host_ptr, ':');
            
            if (port_ptr) {
                *port_ptr = '\0'; //hostname과 port 분리
                sscanf(host_ptr, "%s", hostname);
                sscanf(port_ptr + 1, "%s", port);
            } else {
                sscanf(host_ptr, "%s", hostname);
            }
        }
        Rio_readlineb(&rio, buf, MAXLINE);
    }

    //최종적으로 알아낸 hostname과 port로 목적지 서버에 연결
    clientfd = Open_clientfd(hostname, port);
    if (clientfd < 0) {
        fprintf(stderr, "Connection to %s:%s failed\n", hostname, port);
        return;
    }
    Rio_readinitb(&server_rio, clientfd);

    //목적지 서버로 보낼 요청 메시지 재구성 후 전송
    //요청 라인 전송
    snprintf(buf, MAXLINE, "%s %s HTTP/1.0\r\n", method, path);
    Rio_writen(clientfd, buf, strlen(buf));

    //필수 헤더들 전송
    snprintf(buf, MAXLINE, "Host: %s\r\n", hostname);
    Rio_writen(clientfd, buf, strlen(buf));
    Rio_writen(clientfd, user_agent_hdr, strlen(user_agent_hdr));
    Rio_writen(clientfd, "Connection: close\r\n", strlen("Connection: close\r\n"));
    Rio_writen(clientfd, "Proxy-Connection: close\r\n", strlen("Proxy-Connection: close\r\n"));
    Rio_writen(clientfd, "\r\n", 2); //헤더의 끝

    //서버의 응답을 클라이언트로 전달
    size_t n;
    while((n = Rio_readnb(&server_rio, buf, MAXLINE)) > 0) {
        Rio_writen(fd, buf, n);
    }

    Close(clientfd);
}

//URI를 파싱해서 hostname, port, path를 추출하는 함수
void parse_uri(char *uri, char *hostname, char *port, char *path)
{
    //"http://" 부분 건너뛰기
    char *ptr = strstr(uri, "//");
    ptr = ptr ? ptr + 2 : uri;

    //path 시작 위치 찾기
    char *path_ptr = strstr(ptr, "/");
    if (path_ptr) {
        strcpy(path, path_ptr); //path 복사
        *path_ptr = '\0';       //호스트 이름 부분과 분리
    } else {
        strcpy(path, "/"); //path가 없으면 "/"가 기본값
    }

    //port 번호가 있는지 확인
    char *port_ptr = strstr(ptr, ":");
    if (port_ptr) {
        strcpy(port, port_ptr + 1); //port 복사
        *port_ptr = '\0';           //호스트 이름 부분과 분리
    } else {
        strcpy(port, "80"); //port가 없으면 "80"이 기본값
    }

    //남은 부분은 hostname
    strcpy(hostname, ptr);
}