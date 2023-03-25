#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define LEN_SIZE 4

void error(char *message);

/*
    收到连接之后和客户端进行三次交互
*   数据格式为 前4字节为数据长度，后面为数据
*/

int main(int argc, char const *argv[])
{
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        error("WSAStartup error");

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    SOCKET sock;
    SOCKADDR_IN addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        error("socket error");
    if (connect(sock, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR)
        error("connect error");
    else
        fputs("connecting...\n", stdout);

    int recvLen, recvCnt;
    int recvDataLen;
    char recvData[BUF_SIZE] = {0};
    int sendDataLen, i;
    char sendData[BUF_SIZE] = {0};

    for (i = 0; i < 3; i++)
    {
        printf("client call %d:", i + 1);
        // scanf("%s", sendData);
        fgets(sendData,BUF_SIZE,stdin);

        sendDataLen = strlen(sendData);
        // 发数据长度
        send(sock, (char *)&sendDataLen, LEN_SIZE, 0);
        // 发数据
        send(sock, sendData, sendDataLen, 0);

        // 读数据长度
        recvDataLen = 0;
        recv(sock, &recvDataLen, LEN_SIZE, 0);
        recvLen = 0;
        // 读数据
        while (recvLen < recvDataLen)
        {
            recvCnt = recv(sock, &recvData[recvLen], BUF_SIZE - 1, 0);
            if (recvCnt == -1)
                error("read error");
            recvLen += recvCnt;
        }
        recvData[recvDataLen] = 0;
        printf("call from server %d:%s\n", i + 1, recvData);
    }

    printf("close connection...\n");
    closesocket(sock);
    WSACleanup();
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}