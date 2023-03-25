#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024

void error(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 2)
        error("argc error");
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        error("WSAStartup error");

    SOCKET serv, clnt;
    SOCKADDR_IN sAddr, cAddr;
    int cAddrLen;
    char buffer[BUF_SIZE];
    int bufLen = 0;

    serv = socket(PF_INET, SOCK_STREAM, 0);
    if (serv == INVALID_SOCKET)
        error("socket error");
    memset(&sAddr, 0, sizeof(sAddr));
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
    sAddr.sin_port = htons(atoi(argv[1]));

    if (bind(serv, (SOCKADDR *)&sAddr, sizeof(sAddr)) == -1)
        error("bind error");
    if (listen(serv, 5) == -1)
        error("bind error");
    cAddrLen = sizeof(cAddr);
    for (size_t i = 0; i < 5; i++)
    {
        clnt = accept(serv, (SOCKADDR *)&cAddr, &cAddrLen);
        if (clnt == INVALID_SOCKET)
            error("accept error");
        else
            printf("connecting for client %d\n", i + 1);
        printf("from IP:port = %s:%d\n",inet_ntoa(cAddr.sin_addr),cAddr.sin_port);
        while ((bufLen = recv(clnt, buffer, BUF_SIZE , 0)) > 0)
        {
            buffer[bufLen] = 0;
            printf("receive from client:%s", buffer);
            printf("receive length = %d\n", bufLen);
            send(clnt, buffer, bufLen, 0);
        }
        puts("connecting close....");
        closesocket(clnt);
    }

    closesocket(serv);
    WSACleanup();
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}