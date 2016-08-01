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
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
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
void sif_handler( int sig )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof(sa) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset( &sa.sa_mask );
    int ret = sigaction( sig, sa, NULL );
    assert( ret != -1 );
}


/*定时器处理任务*/
void timer_handler()
{
    timer_lst.tick();
    alarm( TIMESLOT );
}


/*定时器回调函数，他删除非活动连接socket上的注册事件，并关闭之*/
void cb_func( client_data* user_data )
{
    epoll_ctl( epollfd, EPOLL_CTL_DEL, user_data -> sockfd, 0 );   //将注册fd删除
    close( user_data -> sockfd );
    printf( "close fd %d\n", user_data -> sockfd);
}

int main( int argc, char *argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n",  basename( argv[0] ));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof(address) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    ret = bind(listenfd, (struct sockaddr*)&address, sizeof( address ));
    assert( ret != -1 );
    
    ret = listen(listenfd, 5);
    assert( ret != -1 );

    /*创建事件表将listenfd加入*/
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert(epollfd != -1);
    addfd( epollfd, listenfd );

    /*创建管道套接字并加入事件表*/
    ret = socketpair( PF_UNIX, SOCK_STREAM, 0, pipefd );
    assert( ret != -1 );
    setsockbloking( pipefd[1] );
    addfd( epollfd, pipefd[0] );

    addsig( SIGALRM );
    addsig( SIGTERM );
    bool stop_server = false;

    while( !stop_server )
    {
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if( (number < 0) && (errno != EINTR))
        {
            printf( "epoll failure!\n" );
            break;
        }
        for( int i=0; i<number; i++ )
        {
            int sockfd = event[i].data.fd;
            /*处理客户端连接*/
            if( sockfd == listenfd )
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, (struct sockaddr*)&client_address, &client_addrlength );
                addfd( epollfd, connfd );    //将套接字添加到事件表
                /*客户端数据结构*/
                users[connfd].address = client_address;    //客户端socket地址
                users[connfd].sockfd = connfd;    //客户端socket
                util_timer* timer = new util_timer;  //定时器
                timer -> user_data = &users[connfd];
                timer -> cb_func = cb_func;
                timer_t cur = time( NULL );
                timer -> expire = cur + 3 * TIMESLOT;
                users[connfd].timer = timer;
                timer_lst.add_timer( timer );   //将定时器加入链表
            }
            else if( ( sockfd == pipefd[0] ) && ( events[i].events & EPOLLIN ) ) 
            {
                int sig;
                char signals[1024];
                ret = recv( pipefd[0], signals, sizeof( signals ), 0 );
                if( ret < 0 )
                {
                    continue;
                }
                else if( ret == 0 )
                {
                    continue;
                }
                else
                {
                    for( int i=0; i<ret; i++ )   
                    {
                        switch( signals[i] )
                        {
                        case SIGALRM: timeout = true; break;
                        case SIGTERM: stop_server = true;
                        }
                    }
                }
            }
            else if( event[i].events & EPOLLIN )
            {
                memset( users[sockfd].buf, '\0', BUFFER_SIZE );
                ret = recv( sockfd, users[sockfd].buf, NUFFER_SIZE-1, 0 );
                printf( "get %d bytes of client data %s from %d\n", ret, users[sockfd].buf, sockfd );

                util_timer* timer = users[sockfd].timer;
                
                /*如果发生读错误。关闭连接，并删除定时器*/
                if( ret < 0 )
                {
                    if( errno != EAGAIN )
                    {
                        cb_func( &users[sockfd] );
                        if( timer )
                        {
                            timer_lst.del_timer( timer );
                        }
                    }
                }
                else if( ret == 0)
                {
                    cb_func( &users[sockfd] );
                    if( timer )
                    {
                        timer_lst.del_timer( timer );
                    }
                }
                else
                {
                    if( timer ) 
                    {
                        timer_t cur = time( NULL );
                        timer -> expire = cur + 3 * TIMERSLOT;
                        printf( "adjust timer once!\n" );
                        timer_lst.adjust_timer( timer );
                    }
                }
            }
        }
        if( timerout )
        {
            timer_handler();
            timerout = false;
        }
    }
    close( listenfd );
    close( pipefd[1] );
    close( pipefd[0] );
    delete [] users;
    return 0;
}
