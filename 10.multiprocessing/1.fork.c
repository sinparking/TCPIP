#include <stdio.h>
#include <unistd.h>

//this code is for linux

int gval = 10;
int main(int argc, char const *argv[])
{
    pid_t pid;
    int lval = 20;
    gval++, lval += 5;  


    pid = fork();
    return 0;
}
