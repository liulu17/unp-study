//
// Created by Apple on 2022/9/15.
//


#include "unp.h"
#include "wrapper.h"
int Socket(int domain, int type, int protocal)
{
    int sock;
    if ((sock = socket(domain,type,protocal)) == -1) {
        perror("create socket error!");
    }
    return sock;
}

int Listen(int fd, int backlog)
{
    int conn_fd;
    if ((conn_fd = listen(fd,backlog)) == -1) {
        perror("listen error!");
    }
    return conn_fd;
}

int Bind(int fd,struct sockaddr * serv_addr,socklen_t len) {
    int bind_res;
    if ((bind_res= bind(fd,serv_addr,len)) == -1) {
        perror("bind error!");
    }
    return bind_res;
}

int Accept(int fd, struct sockaddr * client, socklen_t * len) {
    int accpet_res;
    if((accpet_res= accept(fd,client,len)) == -1) {
        perror("accept error!");
    }
    return accpet_res;
}

int Connect(int fd, const struct sockaddr * serv, socklen_t len) {
    int res;
    if ((res = connect(fd,serv,len)) == -1) {
        perror("connect error!");
    }
    return res;
}


ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
    size_t	nleft;
    ssize_t	nread;
    char	*ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;		/* and call read() again */
            else
                return(-1);
        } else if (nread == 0)
            break;				/* EOF */

        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft);		/* return >= 0 */
}

ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t		n;

    if ( (n = readn(fd, ptr, nbytes)) < 0)
        perror("readn error");
    return(n);
}


ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
    size_t		nleft;
    ssize_t		nwritten;
    const char	*ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;		/* and call write() again */
            else
                return(-1);			/* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}

void
Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        perror("writen error");
}

static int	read_cnt;
static char	*read_ptr;
static char	read_buf[MAXLEN];
static ssize_t
my_read(int fd, char *ptr)
{
    if (read_cnt <= 0) {
        again:
        if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR)
                goto again;
            return(-1);
        } else if (read_cnt == 0)
            return(0);
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}


ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t	n, rc;
    char	c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ( (rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;	/* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return(n - 1);	/* EOF, n - 1 bytes were read */
        } else
            return(-1);		/* error, errno set by read() */
    }

    *ptr = 0;	/* null terminate like fgets() */
    return(n);
}

ssize_t
readlinebuf(void **vptrptr)
{
    if (read_cnt)
        *vptrptr = read_ptr;
    return(read_cnt);
}
ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t		n;

    if ( (n = readline(fd, ptr, maxlen)) < 0)
        perror("readline error");
    return(n);
}



SigFunc Signal(int signo, SigFunc handler) {
    struct sigaction act, oact;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
//        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo,&act,&oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}


void sig_chld(int signo) {
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1,&stat,WNOHANG)) > 0)
        printf("child %d terminated\n",pid);
    return;

}