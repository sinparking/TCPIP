#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#define BUF_SIZE 512
#define LEN_SIZE 4

void error(char *message);

/*
 * 请求服务器文件的协议
 * 1.客户从用户获取传输文件的名字
 * 2.客户端请求服务器传输该文件名所指示的文件
 * 3.如果指定文件存在，那么服务器将它发送给客户端，反之断开连接
 * 注：
 * 1.传输的数据前4字节指示文件名长度，后面的字节为文件名
 * 2.服务器回送信息前4字节指示文件长度，后面的字节为数据，如果数据长度为0表示没有该文件。//!(修改，接收文件的时候不发送长度直接发送数据,获取文件长度的方法暂时不考虑)
 * */

//todo 服务器没有预先识别文件长度的功能

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        error("WSAStartop error");
    if (argc != 2)
    {
        printf("Usage: %s <PORT>", argv[0]);
        exit(1);
    }
    SOCKET serv, clnt;
    SOCKADDR_IN servAddr, clntAddr;
    int clntAddrLen;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if ((serv = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        error("socket error");
    if (bind(serv, (SOCKADDR *)&servAddr, sizeof(servAddr)) == -1)
        error("bind error");
    if (listen(serv, 5) == -1)
        error("listen error");

    clntAddrLen = sizeof(clntAddr);

    char data[BUF_SIZE] = {0};
    int fileLen, fileNameLen;
    int recvLen, recvCnt;
    int readCnt;
    FILE *fp;
    // accept
    int i;
    for (i = 0; i < 5; i++)
    {
        if ((clnt = accept(serv, (SOCKADDR *)&clntAddr, &clntAddrLen)) == INVALID_SOCKET)
            error("connect error");
        else
            printf("%d:connecting to %s:%d...\n", i + 1, inet_ntoa(clntAddr.sin_addr), clntAddr.sin_port);
        // 读文件名长度
        fileNameLen = 0;
        recv(clnt, &fileNameLen, LEN_SIZE, 0);
        // 读文件名
        recvLen = 0;
        while (recvLen < fileNameLen)
        {
            recvCnt = recv(clnt, &data[recvLen], BUF_SIZE - 1, 0);
            recvLen += recvCnt;
        }
        //! 接收的最后一个字符是\n
        data[fileNameLen - 1] = 0;
        printf("client ask for file:%s length:%d\n", data, fileNameLen - 1);

        //! 打开文件 进行发送
        fp = fopen(data, "rb");
        if (fp == NULL)
        {
            fputs("server no file found\n", stdout);
            fileLen = -1;
            send(clnt, (char *)&fileLen, LEN_SIZE, 0);
        }
        else
        {
            fileLen = 1; // todo 暂时用于占位置，实际长度并没有获取
            send(clnt, (char *)&fileLen, LEN_SIZE, 0);
            while (1)
            {
                readCnt = fread((void *)data, 1, BUF_SIZE, fp);
                // printf("read count = %d\n",readCnt);
                if (readCnt < BUF_SIZE)
                {
                    //! 以BUF_SIZE读取，当实际读取长度小于BUF_SIZE时说明已经读到末尾
                    send(clnt, data, readCnt, 0);
                    break;
                }
                send(clnt, data, BUF_SIZE, 0);
            }
        }

        fputs("closing....\n", stdout);
        fclose(fp);
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