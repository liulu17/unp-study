#include "unp.h"

void print_client(struct sockaddr_in * client) {
    char client_addr[100];
//    inet_ntop(AF_INET,&client->sin_addr,client_addr,sizeof(client_addr));

    printf("client ip is %s\n",inet_ntop(AF_INET,&client->sin_addr,client_addr,sizeof(client_addr)));
    printf("client port is %d\n",ntohs(client->sin_port));
}

void process_connect(int conn_fd) {
        char buff[1024];
        time_t ticks = time(NULL);
        printf("time is %s\n",ctime(&ticks));
        snprintf(buff,sizeof(buff),"%s\n",ctime(&ticks));
        int nwrite;
        if ((nwrite=write(conn_fd,buff,strlen(buff))) == -1) {
            perror("write error!\n");
        }
        printf("write %d bytes to client\n",nwrite);
}



int main()
{
    int connections = 0;
    printf("my pid is %d\n",getpid());
    int listen_fd,conn_fd;
    time_t ticks;
    char buff[1024];

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len = (socklen_t)sizeof(client_addr);
    bzero(&serv_addr,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8887);

    listen_fd =  Socket(AF_INET,SOCK_STREAM,0);

    Bind(listen_fd,(SA)(&serv_addr),sizeof(serv_addr));

    Listen(listen_fd,100);
    printf("time server now is listening on 0.0.0.0 on port 8887\n");
    for(;;) {
        conn_fd = Accept(listen_fd,(SA)&client_addr,&client_len);
        connections++;
        printf("the %dth connect get!\n",connections);
        print_client(&client_addr);

        if (fork() == 0) {
            close(listen_fd);
            process_connect(conn_fd);
            close(conn_fd);
            exit(0);
        }

        close(conn_fd);
    }

    close(listen_fd);

}