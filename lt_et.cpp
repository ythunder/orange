/*************************************************************************
	> File Name: lt_et.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年07月27日 星期三 15时35分15秒
 ************************************************************************/

#include "LT.cpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/epoll.h>

#define MAX_EVENT_NUMBER 1024

int main(int argc, char *argv[])
{
	if(argc <= 2) {
		printf("usage:%s ip_address port_number\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port =  htons(port);
	
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(socket >= 0);

	int ret = bind(listenfd, (struct sockaddr* )&address, sizeof(address) );
	assert(ret != -1);

	ret = listen(listenfd, 5);
	assert(ret != -1);

    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert(ret != -1);
    addfd( epollfd, listenfd, true );

    while(1)
    {
        int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if( ret < 0 )
        {
            printf( "epollfd failure\n" );
            break;
        }
       // lt( events, ret, epollfd, listenfd );
        et( events, ret, epollfd, listenfd );
    }
    close( listenfd );
    return 0; 
}
