#include <sys/unistd.h>
#include <unistd.h>
#include <errno.h>

static int can_read_cnt;
static char *read_ptr;
static char buff[1024];


ssize_t  readn(int fd, void *vptr,size_t n)
{
    ssize_t nread;
    ssize_t nleft = n;
    char * ptr = vptr;
    while (nleft > 0) {
        if ((nread = read(fd,ptr,nleft)) < 0) {
            if (errno = EINTR) {
                nread = 0;
                continue;
            } else  {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        ptr   += nread;
    }
    return (n-nleft);
}


ssize_t writen(int fd,void * vptr,size_t n)
{
    ssize_t nleft = n;
    ssize_t nwrite;
    char * ptr = vptr;
    while (nleft > 0) {
        if ((nwrite = write(fd,ptr,nleft)) < 0) {
            if (errno == EINTR)
                continue;
        } else if (nwrite == 0) {
            continue;
        } else {
            nleft -= nwrite;
            ptr += nwrite;
        }
    }

    return n-nleft;
}


static ssize_t my_read(int fd,char *ptr)
{
    if (can_read_cnt <= 0) {
        again:
        if ((can_read_cnt = read(fd,buff,1024)) < 0) {
            if (errno == EINTR) {
                goto again;
            } else {
                return (-1);
            }
        } else if (can_read_cnt == 0) {
            return(0);
        }
        read_ptr = buff;
    }
    can_read_cnt--;
    *ptr = *read_ptr;
    read_ptr++;
}

ssize_t readlinebuf(void **ptr)
{
    if (can_read_cnt)
        *ptr = read_ptr;
    return can_read_cnt;
}


ssize_t readline(int fd, void * ptr,size_t maxlen)
{


    ssize_t n,nc;
    ssize_t  nread = 0;
    for ( n = 1; n < maxlen; ++n) {
        if ((nc = my_read(fd,ptr)) == 1) {
            ptr++;
            nread++;
            if (nc == '\n')
                break;
        } else if (nc == 0) {
            break;
        } else if (nc == -1) {
            return -1;
        }
    }
    *(char*)ptr = 0;

    return nread;


}