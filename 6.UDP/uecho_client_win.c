#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#define BUF_SIZE 1024

void error(char *msg);

// todo 已连接的socket connected，在sendto之后不会清除分配的IP PORT
// ? 在conncet之后用sendto发送就收不到了，是代码问题吗？ 是代码问题,sendto填的是serv_adr（是destination

int main(int argc, char const *argv[])
{
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        error("WSAStartup error");

    if (argc != 3)
        error("argc :./file <IP> <port>");

    char message[BUF_SIZE];
    SOCKET sock;
    SOCKADDR_IN serv_adr, your_adr;
    int adr_len, str_len;
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
        error("socket error");
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while (1)
    {
        if (connect(sock, (SOCKADDR *)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
            error("connect error");
        fputs("Enter message (q/Q quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        adr_len = sizeof(your_adr);
        // sendto(sock, message, strlen(message), 0, (SOCKADDR *)&serv_adr, sizeof(serv_adr));
        sendto(sock, message, strlen(message), 0, NULL, 0); //! 已连接的时候，第五个参数可以为NULL，第六个参数为0
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (SOCKADDR *)&your_adr, &adr_len);
        // send(sock, message, strlen(message), 0);
        // str_len = recv(sock, message, BUF_SIZE, 0);
        if (str_len == -1)
            error("recvfrom error");
        printf("echo from server:%s\n", message);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

void error(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}