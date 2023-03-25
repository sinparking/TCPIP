#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024

void error(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 3)
        error("argc error");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        error("error WSAStartup()");

    SOCKET sock;
    SOCKADDR_IN addr;
    char buffer[BUF_SIZE];
    int strLen;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        error("socket error");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (SOCKADDR *)&addr, sizeof(addr)) == -1)
        error("connect error");
    else
        puts("connecting.....");

    while (1)
    {
        fputs("input message(Q/q to quit):", stdout);
        fgets(buffer, BUF_SIZE, stdin);
        if (!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n"))
            break;
        send(sock, buffer, strlen(buffer), 0);
        strLen = recv(sock, buffer, BUF_SIZE - 1, 0);
        printf("echo from server:%s", buffer);
        printf("echo length = %d\n", strLen);
    }

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