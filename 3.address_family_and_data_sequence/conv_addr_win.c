#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <WinSock2.h>

/*
* WSAStringToAddress 和 WSAAddressToString的功能同inet_ntoa和inet_addr,但是支持多种协议 P55，依赖于特定平台
* */

int main(int argc, char const *argv[])
{
    char *strAddr = "203.211.218.102:9190";

    char strAddrBuf[50];
    SOCKADDR_IN servAddr;
    int size;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    size = sizeof(servAddr);
    //* WSAStringToAddress, 将strAddr字符串转换为AF_INET的地址放入servAddr中，size是表示servAddr的大小的变量的地址
    WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR *)&servAddr, &size);

    size = sizeof(strAddrBuf);
    //* WSAAddressToString, 将servAddr的地址转换为字符串存放在strAddrBuf中，需要指明servAddr和strAddrBuf的大小
    WSAAddressToString((SOCKADDR *)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);

    printf("Second conv result = %s \n", strAddrBuf);

    WSACleanup();
    return 0;
}
