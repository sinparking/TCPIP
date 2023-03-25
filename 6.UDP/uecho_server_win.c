#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#define BUF_SIZE 1024

void error(char *msg);

int main(int argc, char const *argv[])
{
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        error("WSAStartup error");

    if (argc != 2)
        error("argc :./file <port>");

    char message[BUF_SIZE];
    SOCKET serv;
    SOCKADDR_IN serv_adr, your_adr;
    int adr_len, str_len;
    serv = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv == INVALID_SOCKET)
        error("socket error");
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if (bind(serv, (SOCKADDR *)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
        error("bind error");

    while (1)
    {
        adr_len = sizeof(your_adr);
        str_len = recvfrom(serv, message, BUF_SIZE, 0, (SOCKADDR *)&your_adr, &adr_len);
        if (str_len == -1)
            error("recvfrom error");
        message[str_len] = 0;
        printf("recvLen = %d, message = %s", str_len, message);
        sendto(serv, message, str_len, 0, (SOCKADDR *)&your_adr, adr_len);
    }

    closesocket(serv);
    WSACleanup();
    return 0;
}

void error(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}