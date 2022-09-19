//
// Created by Apple on 2022/9/15.
//

#ifndef TEST_WRAPPER_H
#define TEST_WRAPPER_H
extern char ** environ;
int Socket(int,int,int);
int Listen(int,int);
int Bind(int,struct sockaddr*,socklen_t);
int Accept(int , struct sockaddr * , socklen_t * );
int Connect(int fd, const struct sockaddr * serv, socklen_t len);
typedef  struct sockaddr* SA;
ssize_t readn(int fd, void *vptr, size_t n);
ssize_t Readn(int fd, void *ptr, size_t nbytes);
ssize_t writen(int fd, const void *vptr, size_t n);
void Writen(int fd, void *ptr, size_t nbytes);
static ssize_t my_read(int fd, char *ptr);
ssize_t readline(int fd, void *vptr, size_t maxlen);
ssize_t readlinebuf(void **vptrptr);
ssize_t Readline(int fd, void *ptr, size_t maxlen);

typedef void (* SigFunc)(int);

SigFunc Signal(int signo, SigFunc handler);
void sig_chld(int signo);
#endif //TEST_WRAPPER_H
