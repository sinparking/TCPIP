#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OP_SIZE 4

/*
!   坑在64行，用于接收char[]的 int 每次都需要初始化
* */

void error(char *message);

int computing(int *a, int n, char operator);

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
    int result;

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

        opCnt = 0; //! 为甚要初始化,不初始化就会报错
        recv(clnt, &opCnt, 1, 0);
        printf("opcnt = %d\n", opCnt);
        recvLen = 0;
        while (recvLen < (opCnt * OP_SIZE + 1))
        {
            recvCnt = recv(clnt, &opInfo[recvLen], BUF_SIZE - 1, 0);
            printf("a=%d,b=%d\n", recvCnt, recvLen);
            recvLen += recvCnt;
        };

        //! 这里运算符出错
        result = computing((int *)opInfo, opCnt, opInfo[recvCnt-1]);
        send(clnt, (char *)&result, RLT_SIZE, 0);

        fputs("connect close...\n",stdout);
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

int computing(int *a, int n, char operator)
{
    int i, res = a[0];
    switch (operator)
    {
    case '+':
        for (i = 1; i < n; i++)
            res += a[i];
        break;
    case '-':
        for (i = 1; i < n; i++)
            res -= a[i];
        break;
    case '*':
        for (i = 1; i < n; i++)
            res *= a[i];
        break;
    default:
        break;
    }
    return res;
}