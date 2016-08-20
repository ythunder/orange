/*************************************************************************
	> File Name: EPOLLONESHORT.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月29日 星期五 08时30分13秒
 ************************************************************************/

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

struct fds
{
    int epollfd;
    int sockfd;
};

/*将文件描述符设为非阻塞*/
int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETEL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETEL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd, bool oneshot )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if( oneshot )
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctr(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking( fd );
}

/*重置fd上的事件，这样操作后，即使fd上的EPOLLONESHOT事件被注册，单操作系统仍然会触发fd上的EPOLLIN事件按，且仅触发一次*/
void reset_oneshot( int epolfd, int fd )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctr( epollfd, EPOLL_CTL_MOD, fd, &event );
}

/*工作线程*/
void* worker( void* arg )
{
    int sockfd = ( (fds* )arg ) -> sockfd;
    int epollfd = ( (fds* )arg ) -> epollfd;
    printf( "start new thread to receive data on fd: %d\n", sockfd );

    char buf[BUFFER_SIZE];
    memset( buf, '\0', BUFFER_SIZE);

    while( 1 )
    {
        int ret = recv( sockfd, buf, BUFFER_SIZE, 0 );
        if(ret == 0)
        {
            close( sockfd );
            printf( "foreiner closed the connection\n" );
            break;
        }
        else if( ret < 0 )
        {
            if( errno == EAGAIN )
            {
                reset_oneshot( epollfd, sockfd );
                printf( "read later\n" );
                break;
            }
        }
        else
        {
            printf( "get content: %s\n", buf );
            sleep( 5 );
        }
    }
    printf( "end thread receiving data on fd : %d\n", sockfd );
}

int main( int argc, char *argv[] )
{
    if( argc <= 2 )
    {
        printf("usage: %s ip_address port_number\n", argv[0]);
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi( argv[2] );

    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    ret = bind( listenfd, (struct sockaddr*)&address, sizeof(address) );
    assert( ret != -1 );

    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd, false );   //listenfd 不设置EPOLLONESHOT，否则应用程序只能处理一个客户端连接

    while( 1 )
    {
        int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if( ret < 0 )
        {
            printf( "epoll failure\n" );
            break;
        }
        for( int i=0; i<ret; i++ )
        {
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd)     /*有新的客户端请求连接时表示为监听套接字发生事件*/
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, (struct sockaddr* )&client_address, &client_addrlength );
                addfd( epollfd, connfd, true );
            }
            else if( event[i].events & EPOLLIN )
            {
                pthread_t thread;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd = epollfd;
                fds_for_new_worker.sockfd = sockfd;
                pthread_create( &thread, NULL, worker, (void* )&fds_for_new_worker );
            }
            else 
            {
                printf( "something else happened\n" );
            }
        }
    }
    close( listenfd );
    return 0;
}
