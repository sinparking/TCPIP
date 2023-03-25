#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

/*
 * ���º���������windows���������ð汾
 * gethostbyaddr
 * gethostbyname
 */

//* IP��IP��ַ
int main(int argc, char const *argv[])
{
    WSADATA wsadata;
    int i;
    struct hostent *host;
    SOCKADDR_IN addr;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (argc != 2)
    {
        printf("error argc!\n");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    //! ���������ת������һ��������ָ��in_addr�ṹ���ָ��
    host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);

    printf("official name:%s \n", host->h_name);
    for (i = 0; host->h_aliases[i]; i++)
        printf("alias %d:%s\n", i + 1, host->h_aliases[i]);
    printf("Address type = %s\n", host->h_addrtype == PF_INET ? "AF_INET" : "AF_INET6");
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP: %d:%s\n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));

    WSACleanup();
    return 0;
}
