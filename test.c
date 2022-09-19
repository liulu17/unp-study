//
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <errno.h>
//#include <sys/types.h>
//#include <sys/uio.h>
//#include <sys/fcntl.h>
//
//int main(void)
//{
//    char buff[10];
//    char content[1024];
//    FILE *fp = fopen("../readme","r");
//    int c;
//    while ((c=fgetc(fp)) != EOF) {
//        fputc(c,stdout);
//        fputc(10,stdout);
//    }
//    if (feof(fp)) {
//        fprintf(stdout,"end of file.\n");
//    }
//    if (ferror(fp)) {
//        fprintf(stdout,"read error.\n");
//    }
//    return 0;
//
//}
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>



int main(void)
{
    if (fork() == 0) {
        sleep(1);
        exit(0);
    }
    if (fork() == 0) {
//        sleep(2);
        exit(2);
    }
    sleep(3);
    int state;
    pid_t pid = wait(&state);
    printf("child process exit with code %d\n",state);
    return 0;

}