#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <WinSock2.h>

/*
* WSAStringToAddress �� WSAAddressToString�Ĺ���ͬinet_ntoa��inet_addr,����֧�ֶ���Э�� P55���������ض�ƽ̨
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
    //* WSAStringToAddress, ��strAddr�ַ���ת��ΪAF_INET�ĵ�ַ����servAddr�У�size�Ǳ�ʾservAddr�Ĵ�С�ı����ĵ�ַ
    WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR *)&servAddr, &size);

    size = sizeof(strAddrBuf);
    //* WSAAddressToString, ��servAddr�ĵ�ַת��Ϊ�ַ��������strAddrBuf�У���Ҫָ��servAddr��strAddrBuf�Ĵ�С
    WSAAddressToString((SOCKADDR *)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);

    printf("Second conv result = %s \n", strAddrBuf);

    WSACleanup();
    return 0;
}
