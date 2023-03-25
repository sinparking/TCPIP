#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char *message);

/*
 ! 0. -lws2_32
 * 1. int WSAStartUp(WORD wVersionRequested, LPWSADATA lpWSAData); ³õÊ¼»¯
 * 2. int WSACleanup(void); ×¢Ïú¿â
 * 3. SOCKET socket(int af, int type, int protocal);
 * 4. int bind(SOCKET s, const struct sockaddr * name, int namelen);
 * 5. int listen(SOCKET s, int backlog);
 * 6. SOCKET accept(SOCKET s, struct sockaddr * addr, int * addrlen);
 * 7. int connect(SOCKET s, const struct sockaddr * name, int namelen);
 * 8. closesocket(SOCKET s);
 * */

int main(int argc, char const *argv[])
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;

    int szClntAddr;
    char message[] = "hello world";
    if (argc != 2)
    {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error!");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if (bind(hServSock, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error!");

    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error!");

    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR *)&clntAddr, &szClntAddr);
    if (hClntSock == INVALID_SOCKET)
        ErrorHandling("accept() error!");

    send(hClntSock, message, sizeof(message), 0);
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup;
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}