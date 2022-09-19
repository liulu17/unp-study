//
// Created by Apple on 2022/9/14.
//
#include "unp.h"

int main()
{
    int conn_fd;
    time_t ticks;
    char buff[1024];

    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.0.100",&serv_addr.sin_addr);
    serv_addr.sin_port = htons(8887);

    if((conn_fd=Socket(AF_INET,SOCK_STREAM,0)) == -1) {
        exit(1);
    }
    if(Connect(conn_fd,(SA)(&serv_addr),sizeof(serv_addr)) == -1) {
        exit(1);
    }

    printf("connect suc!\n");

    int nread;
    while ( (nread = read(conn_fd,buff,sizeof(buff))) > 0) {
        // printf("read error\n");
        buff[nread] = 0;
        if (fputs(buff,stdout) == EOF) {
            printf("fputs error!\n");
        }
    }

    close(conn_fd);
    sleep(1);

}