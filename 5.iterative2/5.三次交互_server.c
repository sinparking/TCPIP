#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define LEN_SIZE 4

/*
    �յ�����֮��Ϳͻ��˽������ν���
*   ���ݸ�ʽΪ ǰ4�ֽ�Ϊ���ݳ��ȣ�����Ϊ����
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

    int recvLen, recvCnt;
    int recvDataLen;
    char recvData[BUF_SIZE] = {0};
    int sendDataLen, i, j;
    char sendData[BUF_SIZE] = {0};

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

        for (j = 0; j < 3; j++)
        {
            // ��ȡ���ݳ���
            sendDataLen = 0;
            recv(clnt, &recvDataLen, LEN_SIZE, 0);
            recvLen = 0;
            // ��ȡ����
            while (recvLen < recvDataLen)
            {
                recvCnt = recv(clnt, &recvData[recvLen], BUF_SIZE - 1, 0);
                if (recvCnt == -1)
                    error("read error");
                recvLen += recvCnt;
            }
            recvData[recvDataLen] = 0;
            printf("call from client %d:%s", i + 1, recvData);

            // ������ len+data
            printf("server call %d:", j + 1);
            // scanf("%s", sendData);
            fgets(sendData, BUF_SIZE, stdin);
            putchar('\n');
            sendDataLen = strlen(sendData);
            send(clnt, (char *)&sendDataLen, LEN_SIZE, 0);
            send(clnt, sendData, sendDataLen, 0);
        }

        printf("close connection...\n");
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
