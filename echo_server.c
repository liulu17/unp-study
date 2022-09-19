//
// Created by Apple on 2022/9/17.
//
#include "unp.h"

void print_client(struct sockaddr_in * client) {
    char client_addr[100];
    printf("client ip is %s\n",inet_ntop(AF_INET,&client->sin_addr,client_addr,sizeof(client_addr)));
    printf("client port is %d\n",ntohs(client->sin_port));
}

void str_echo(int conn_fd) {
    char buff[1024];

    int nwrite, nread;
    long arg1,arg2;

again:
    while ( (nread = read(conn_fd,buff,1024)) > 0) {
        fprintf(stdout,"i read from client %s\n",buff);
//        Writen(conn_fd,buff,nread);
        if (sscanf(buff,"%ld%ld",&arg1,&arg2) == 2) {
            printf("%ld---%ld\n",arg1,arg2);
            snprintf(buff,sizeof(buff),"res is %ld\n",arg1+arg2);
        } else {
            snprintf(buff,sizeof(buff),"input error\n");

        }
        Writen(conn_fd,buff,strlen(buff));
    }


    if (nread < 0 && errno == EINTR)
        goto again;
    else if (nread < 0)
        perror("str_echo error.");
}



int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage:./echo_server port\n");
    }
    int connections = 0;
    printf("my pid is %d\n",getpid());
    int listen_fd,conn_fd;
    time_t ticks;
    char buff[1024];

    struct sockaddr_in serv_addr,client_addr,my_addr;
    socklen_t client_len = (socklen_t)sizeof(client_addr);
    socklen_t my_addr_len = (socklen_t)sizeof(my_addr);

    bzero(&serv_addr,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    listen_fd =  Socket(AF_INET,SOCK_STREAM,0);

    Bind(listen_fd,(SA)(&serv_addr),sizeof(serv_addr));

    Listen(listen_fd,100);

    Signal(SIGCHLD,sig_chld); // 处理子进程变成僵尸进程


    if (getsockname(listen_fd,(SA)&my_addr,&my_addr_len) == -1) {
        perror("get my address error");
    }
    printf("time server now is listening on %s on port %d\n",inet_ntop(AF_INET,&serv_addr.sin_addr,buff,sizeof(buff)),ntohs(my_addr.sin_port));
    bzero(buff,sizeof(buff));
    for(;;) {
        if((conn_fd = accept(listen_fd,(SA)&client_addr,&client_len)) == -1) { // 处理被中断情况
            if (errno == EINTR)
                continue;
            else
                break;
        }
        connections++;
        printf("the %dth connect get!\n",connections);
        print_client(&client_addr);

        if (fork() == 0) {
            close(listen_fd);
            str_echo(conn_fd);
            close(conn_fd);
            exit(0);
        }

        close(conn_fd);
    }

    printf("Server shutdown\n");
    close(listen_fd);

}