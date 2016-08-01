/*************************************************************************
	> File Name: close_connect.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月01日 星期一 08时03分10秒
 ************************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include "lst_timer.h"

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define TIMESLOT 5

static int pipefd[2];
static sort_timer_lst time_lst;    //代码lst_timer中的升序链表管理定时器
static int epollfd = 0;

/*套接字设置为非阻塞*/
int setsockbloking( int fd )
{
    int old_option = fcntl( fd, F_GETEL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETEL, new_option );
    return old_option;
}

/*将fd添加到事件表*/
void addfd( int epollfd, int fd )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setsockbloking( fd );
}

/*信号处理函数,将信号发送到管道口*/
void sig_handler( int sig )
{
    int save_errno = errno;
    int msg = sig;
    send( pipefd[1], (char*)&msg, 1, 0 );
    errno = save_errno;
}

/*设置信号新处理方式*/
void sif_handler( intn sig )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof(sa) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset( &sa.sa_mask );
    int ret = sigaction( sig, sa, NULL );
    assert( ret != -1 );
}


/**/
