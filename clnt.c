#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void error(char *message);

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        error("WSAStartup");

    SOCKET sock;
    SOCKADDR_IN addr;
    char message[30];
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        error("socket");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (SOCKADDR *)&addr, sizeof(addr)) == -1)
        error("connect");
    recv(sock, message, sizeof(message), 0);
    printf("I receive from you, hellow, %s\n", message);
    
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