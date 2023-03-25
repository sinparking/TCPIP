#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define LEN_SIZE 4
#define BUF_SIZE 1024

void error(char *message);

int main(int argc, char const *argv[])
{
    int serv, clnt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;

    if (argc != 2)
    {
        printf("Usage: %s <PORT>\n", argv[0]);
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (-1 == (serv = socket(PF_INET, SOCK_STREAM, 0)))
        error("socket error");
    if (-1 == bind(serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error("bind error");
    if (-1 == listen(serv, 5))
        error("listen error");
    clnt_addr_len = sizeof(clnt_addr);

    char data[BUF_SIZE]={0};
    int file_name_len;
    int file, read_cnt;
    int send_len;
    int recv_len, recv_cnt;
    while (1)
    {
        if (-1 == (clnt = accept(serv, (struct sockaddr *)&clnt_addr, &clnt_addr_len)))
            error("accept error");
        else
            printf("connecting for %s:%d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
        // 读取文件名长度
        file_name_len = 0;
        read(clnt, &file_name_len, LEN_SIZE);
        // 读取文件名
        recv_len = 0;
        while (recv_len < file_name_len)
        {
            recv_cnt = read(clnt, data, file_name_len);
            recv_len += recv_cnt;
        }
        data[file_name_len-1] = 0;//todo 这里暂时因为client的原因传输的时候多发送了一个回车
        printf("%s:%d\n", data, file_name_len);
        if (-1 == (file = open(data, O_RDONLY)))
        {
            printf("No file found:%s\n", data);
            send_len = -1;
            write(clnt, (char *)&send_len, LEN_SIZE);
        }
        else
        {
            printf("file found,sending....\n");
            send_len = 1; //todo 还没有实现读取文件的长度
            write(clnt, (char *)&send_len, LEN_SIZE);
            while (0 != (read_cnt = read(file, data, BUF_SIZE)))
            {
                printf("%d\n",read_cnt);
                if (read_cnt < BUF_SIZE)
                {
                    write(clnt, data, read_cnt);
                    break;
                }
                write(clnt, data, BUF_SIZE);
            }
        }
        fputs("closing....\n", stdout);
        close(file);
        close(clnt);
    }

    close(serv);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}