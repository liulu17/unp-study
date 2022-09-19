
#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
    char sendLine[MAXLEN],recvLine[MAXLEN];
    while(fgets(sendLine,sizeof(sendLine),stdin)) {
//        Writen(sockfd,sendLine,1);
//        sleep(1);
        Writen(sockfd,sendLine,strlen(sendLine));
        if (Readline(sockfd,recvLine,MAXLEN) == 0) {
            perror("str_cli:server terminated permaturely");
            exit(1);
        }
        fputs(recvLine,stdout);
    }

}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage:./echo_client ip port\n");
    }
    int conn_fd;


    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
//
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.0.100",&serv_addr.sin_addr);
    serv_addr.sin_port = htons(atoi(argv[1]));
//
    if((conn_fd=Socket(AF_INET,SOCK_STREAM,0)) == -1) {
        exit(1);
    }
    if(Connect(conn_fd,(SA)(&serv_addr),sizeof(serv_addr)) == -1) {
        exit(1);
    }

    printf("connect suc!\n");
    str_cli(stdin,conn_fd);

//    int conn_fd_array[5];
//    for (int i = 0; i < 5; i++) {
//        conn_fd_array[i] = Socket(AF_INET,SOCK_STREAM,0);
////        bzero(&serv_addr,sizeof(serv_addr));
//        if(Connect(conn_fd_array[i],(SA)(&serv_addr),sizeof(serv_addr)) == -1) {
//            perror("client connect error");
//            exit(1);
//        }
//    }
//    str_cli(stdin,conn_fd_array[0]);
//    exit(0);



    close(conn_fd);


}