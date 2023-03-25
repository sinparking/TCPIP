#include <WinSock2.h>
#include <iostream>
#include <string>
using std::string;

void ErrorHandling(string message);

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    //...
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("error WSAStartup()");
    //...
    WSACleanup();
    return 0;
}

void ErrorHandling(string message)
{
    std::cerr << message << "\n"
              << std::endl;
    exit(1);
}
//##################################################################
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OP_SIZE 4


/*
    收到连接之后和客户端进行三次交互
*   数据格式为 前4字节为数据长度，后面为数据    
*/

void error(char *message);

int main(int argc, char const *argv[])
{
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        error("WSAStartup error");

    if (argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    SOCKET serv, clnt;
    SOCKADDR_IN servAddr, clntAddr;
    int clntLen;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    serv = socket(PF_INET, SOCK_STREAM, 0);
    if (serv == INVALID_SOCKET)
        error("socket error");
    if (bind(serv, (SOCKADDR *)&servAddr, sizeof(servAddr)) == -1)
        error("bind error");
    if (listen(serv, 5) == -1)
        error("listen error");

    int opCnt, i;
    char opInfo[BUF_SIZE];
    int recvLen;
    int recvCnt;

    clntLen = sizeof(clntAddr);
    for (i = 0; i < 5; i++)
    {
        clnt = accept(serv, (SOCKADDR *)&clntAddr, &clntLen);
        if (clnt == INVALID_SOCKET)
            error("accept error");

        printf("connect %d for %s:%d\n",
               i + 1,
               inet_ntoa(clntAddr.sin_addr),
               clntAddr.sin_port);

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

