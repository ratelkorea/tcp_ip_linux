#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;  // 소켓 디스크립터를 저장할 변수
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;  // 소켓 생성 정보를 저장하는 구조체 변수
    socklen_t clnt_adr_sz;

    if(argc != 2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);                                   // 프로세스 종료 코드 남기기
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);   // 소켓생성, (IPv4, TCP,)
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_sock, 0, sizeof(serv_adr));      // 구조체 초기화
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)   // ip , port 생성
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)             // 연결준비 완성
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);            //  client 정보를 담을 구조체의 크기

    for(i =0 ; i < 5 ; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_adr, clnt_adr_sz);    // 연결
        if(clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i + 1);

        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) //  수신한 바이트수 반환
            write(clnt_sock, message, str_len);                    //   client 에 전송

        close(clnt_sock);                                          //   연결 종료 
        
    }
    close(serv_sock);
    return 0;

}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}