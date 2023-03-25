#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#define BUF_SIZE 512
#define LEN_SIZE 4

void error(char *message);

/*
 * ����������ļ���Э��
 * 1.�ͻ����û���ȡ�����ļ�������
 * 2.�ͻ������������������ļ�����ָʾ���ļ�
 * 3.���ָ���ļ����ڣ���ô�������������͸��ͻ��ˣ���֮�Ͽ�����
 * ע��
 * 1.���������ǰ4�ֽ�ָʾ�ļ������ȣ�������ֽ�Ϊ�ļ���
 * 2.������������Ϣǰ4�ֽ�ָʾ�ļ����ȣ�������ֽ�Ϊ���ݣ�������ݳ���Ϊ0��ʾû�и��ļ���//!(�޸ģ������ļ���ʱ�򲻷��ͳ���ֱ�ӷ�������,��ȡ�ļ����ȵķ�����ʱ������)
 * */

//todo ������û��Ԥ��ʶ���ļ����ȵĹ���

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
        // ���ļ�������
        fileNameLen = 0;
        recv(clnt, &fileNameLen, LEN_SIZE, 0);
        // ���ļ���
        recvLen = 0;
        while (recvLen < fileNameLen)
        {
            recvCnt = recv(clnt, &data[recvLen], BUF_SIZE - 1, 0);
            recvLen += recvCnt;
        }
        //! ���յ����һ���ַ���\n
        data[fileNameLen - 1] = 0;
        printf("client ask for file:%s length:%d\n", data, fileNameLen - 1);

        //! ���ļ� ���з���
        fp = fopen(data, "rb");
        if (fp == NULL)
        {
            fputs("server no file found\n", stdout);
            fileLen = -1;
            send(clnt, (char *)&fileLen, LEN_SIZE, 0);
        }
        else
        {
            fileLen = 1; // todo ��ʱ����ռλ�ã�ʵ�ʳ��Ȳ�û�л�ȡ
            send(clnt, (char *)&fileLen, LEN_SIZE, 0);
            while (1)
            {
                readCnt = fread((void *)data, 1, BUF_SIZE, fp);
                // printf("read count = %d\n",readCnt);
                if (readCnt < BUF_SIZE)
                {
                    //! ��BUF_SIZE��ȡ����ʵ�ʶ�ȡ����С��BUF_SIZEʱ˵���Ѿ�����ĩβ
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