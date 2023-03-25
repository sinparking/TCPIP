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
 * 2.服务器回送信息前4字节指示文件长度，后面的字节为数据，如果数据长度为0表示没有该文件。//!(修改，接收文件的时候不发送长度直接发送数据)
 * */

//todo 传输的文件名末尾会多发一个回车字符

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        error("WSAStartop error");
    if (argc != 3)
    {
        printf("Usage: %s <IP> <PORT>", argv[0]);
        exit(1);
    }
    SOCKET sock;
    SOCKADDR_IN addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        error("socket error");
    if (connect(sock, (SOCKADDR *)&addr, sizeof(addr)) == INVALID_SOCKET)
        error("connect error");
    else
        fputs("connecting....\n", stdout);

    char info[BUF_SIZE] = {0};
    int recvLen, recvCnt;
    int fileNameLen, fileLen;
    int readCnt;
    FILE *fp;

    fputs("Enter file name:", stdout);
    fgets(info, BUF_SIZE, stdin);
    fileNameLen = strlen(info);
    // 发送文件名的长度指示
    send(sock, (char *)&fileNameLen, LEN_SIZE, 0);
    // 发送文件名
    send(sock, info, fileNameLen, 0);
    // 接收文件长度
    fileLen = 0;
    recv(sock, &fileLen, LEN_SIZE, 0); // todo 实际上并没有用用到，因为服务器还没用统计文件长度的功能。此处仅为占位，以后实现
    if (fileLen == -1)
    {
        fputs("no file found in server\n", stdout);
    }
    else //! 接收文件
    {
        char title[30] = "recv_";
        info[fileNameLen-1] = 0;
        strcat(title,info);
        fp = fopen(title, "wb");
        while ((readCnt = recv(sock, info, BUF_SIZE, 0)) != 0)
        {
            fwrite((void *)info, 1, readCnt, fp);
            printf("read count = %d\n", readCnt);
        }
    }

    fputs("closing....\n", stdout);
    fclose(fp);
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