#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void error(char *message);

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    
    //! ’‚¿Ô «!=0
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        error("WSAStartup error");
    SOCKET serv;
    SOCKET clnt;
    SOCKADDR_IN servAddr;
    SOCKADDR_IN clntAddr;
    int clntAddrLen;

    char message[] = "hello zr, this is my first server&client!";
    serv = socket(PF_INET, SOCK_STREAM, 0);
    if (serv == INVALID_SOCKET)
        error("socket error");
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if (bind(serv, (SOCKADDR *)&servAddr, sizeof(servAddr)) == -1)
        error("bind() error");
    if (listen(serv, 5) == -1)
        error("listen() error");
    clntAddrLen = sizeof(clntAddr);
    clnt = accept(serv, (SOCKADDR *)&clntAddr, &clntAddrLen);
    if (clnt == INVALID_SOCKET)
        error("accept() error");

    send(clnt, message, sizeof(message), 0);

    closesocket(clnt);
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