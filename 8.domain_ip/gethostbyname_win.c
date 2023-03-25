#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

/*
 * 以下函数可能在windows有其它更好版本
 * gethostbyaddr
 * gethostbyname
 */

//* 域名到IP地址
int main(int argc, char const *argv[])
{
    WSADATA wsadata;
    int i;
    struct hostent *host;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (argc != 2)
    {
        printf("error argc!\n");
        exit(1);
    }
    host = gethostbyname(argv[1]);

    printf("official name:%s \n", host->h_name);
    for (i = 0; host->h_aliases[i]; i++)
        printf("alias %d:%s\n", i + 1, host->h_aliases[i]);
    printf("Address type = %s\n", host->h_addrtype == PF_INET ? "AF_INET" : "AF_INET6");
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP: %d:%s\n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));

    WSACleanup();
    return 0;
}
