#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *file;
    char buff[300];
    file = fopen("recv_whereisthefile.txt","rb");
    fseek(file,0,SEEK_END);
    int f_len = ftell(file);
    printf("%d\n",f_len);

    // strcpy(buff,"file_from_server.txt");
    // file = fopen("file_from_server.txt","rb");
    // if(file == NULL)
    //     printf("fuck!\n");
    // printf("%d",strlen("file_from_server.txt"));
    // printf("%d",strlen(buff));
    // fclose(file);
    return 0;
}
