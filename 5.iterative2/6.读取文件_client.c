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
 * 2.������������Ϣǰ4�ֽ�ָʾ�ļ����ȣ�������ֽ�Ϊ���ݣ�������ݳ���Ϊ0��ʾû�и��ļ���//!(�޸ģ������ļ���ʱ�򲻷��ͳ���ֱ�ӷ�������)
 * */

//todo ������ļ���ĩβ��෢һ���س��ַ�

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
    // �����ļ����ĳ���ָʾ
    send(sock, (char *)&fileNameLen, LEN_SIZE, 0);
    // �����ļ���
    send(sock, info, fileNameLen, 0);
    // �����ļ�����
    fileLen = 0;
    recv(sock, &fileLen, LEN_SIZE, 0); // todo ʵ���ϲ�û�����õ�����Ϊ��������û��ͳ���ļ����ȵĹ��ܡ��˴���Ϊռλ���Ժ�ʵ��
    if (fileLen == -1)
    {
        fputs("no file found in server\n", stdout);
    }
    else //! �����ļ�
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