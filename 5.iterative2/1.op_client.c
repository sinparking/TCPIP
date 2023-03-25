#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OP_SIZE 4

void error(char *message);

/*
第一个字节指明数据个数，后序直接指明数据，最后一个字节表示运算符
todo 应用层协议
    1.客户端连接服务器后以 1字节 整数形式传递等待计算的数字个数
    2.客户端想服务器传递的每个整数型数据占4字节
    3.传递整数型数据后传递运算符。运算符占 1字节
    4.字符仅有+，-，*
    5.服务器以4字节整数型向客户端传回运算结果
    6.客户端得到运算结果之后终止服务器连接
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

    char opinfo[BUF_SIZE];
    char op;
    int opcnt, i, index = 1;
    int result;
    fputs("input operand count:", stdout);
    scanf("%d", &opcnt);
    opinfo[0] = (char)opcnt;
    for (i = 0; i < opcnt; ++i)
    {
        printf("Operand %d:", i + 1);
        scanf("%d", (int *)&opinfo[index]);
        index += OP_SIZE;
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);

    scanf("%c", &opinfo[index]);
    send(sock, opinfo, index + 1, 0);
    recv(sock, &result, RLT_SIZE, 0);
    printf("result is %d\n", result);

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